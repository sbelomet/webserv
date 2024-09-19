/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Manager.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbelomet <sbelomet@42lausanne.ch>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/22 13:34:10 by lgosselk          #+#    #+#             */
/*   Updated: 2024/09/19 12:39:01 by sbelomet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Manager.hpp"

Manager::Manager( void )
{}

Manager::~Manager( void )
{
	std::cout << "Manager destroyed" << std::endl;
}

Manager::Manager( Manager const &copy )
{
	*this = copy;
}

Manager const	&Manager::operator=( Manager const &copy )
{
	if (this != &copy)
	{}
	return (*this);
}

/*  */

MapConfig	&Manager::getMapConfig( void )
{
	return (_map_config);
}

/*  */

bool	Manager::acceptConnection( Server &server, int const &index )
{
	sockaddr				new_addr;
	int						newSocket;
	std::vector<int> const	sockets = server.getSockets();
	socklen_t				addr_length = sizeof(new_addr);

	newSocket = accept(sockets[index], &new_addr, &addr_length);
	if (newSocket < 0 && (!(errno == EAGAIN) || !(errno == EWOULDBLOCK)))
	{
		perror("accept");
		return (false);
	}
	if (fcntl(newSocket, F_SETFL, fcntl(newSocket, F_GETFL, 0) | O_NONBLOCK) < 0)
	{
		perror("fcntl: acceptConnection()");
		return (false);
	}
	epoll_event	event;
	event.events = EPOLLIN || EPOLLET;
	event.data.fd = newSocket;
	server.insertNewConnection(newSocket, index);
	if (epoll_ctl(server.getEpollFd(), EPOLL_CTL_ADD, newSocket, &event) < 0)
	{
		perror("epoll_ctl: acceptConnection()");
		return (false);
	}
	std::cout << GREEN << "New connection created" << RESET << std::endl;
	return (true);
}

bool	Manager::epollWaiting( Server &server )
{
	int			index = -1;
	epoll_event	events[MAX_EVENTS];

	int const	ndfs = epoll_wait(server.getEpollFd(), events, MAX_EVENTS, -1);
	if (ndfs < 0)
	{
		perror("epoll_wait");
		throw (Webserv::NoException());
	}
	for (int i = 0; i < ndfs; i++)
	{
		if ((index = server.newConnection(events[i].data.fd)) != -1)
		{
			if (!acceptConnection(server, index))
				return (false);
		}
		else if ((events[i].events & EPOLLERR) || (events[i].events & EPOLLHUP)
			|| (!(events[i].events & EPOLLIN)))
		{
			std::cout << "close connection" << std::endl;
			close(events[i].data.fd);
			return (true);
		}
		else
		{
			readRequest(server, events[i].data.fd);
			std::cout << "request answered" << std::endl;
		}
		// add another else if, if add stdin to epoll for check inputs
	}
	return (true);
}

void	Manager::epollStarting( Server &server )
{
	int const	epollFd = epoll_create1(0);
	if (epollFd < 0)
		throw (Webserv::EpollException());
	server.setEpollfd(epollFd);
	std::vector<int> const	sockets = server.getSockets();
	for (size_t i = 0; i < sockets.size(); i++)
	{
		epoll_event	event;
		event.events = EPOLLIN | EPOLLET;
		event.data.fd = sockets[i];
		if (epoll_ctl(server.getEpollFd(), EPOLL_CTL_ADD, sockets[i], &event) < 0)
		{
			perror("epoll_ctl: epollStarting()");
			throw (Webserv::NoException());
		}
	}
	//epoll_event	event;
	//event.data.fd = 0;
	//event.events = EPOLLIN;
	//if (epoll_ctl(server.getEpollFd(), EPOLL_CTL_ADD, 0, &event) < 0)
	//{
	//	perror("epoll_ctl: quit");
	//	throw (Webserv::NoException());
	//}
}

void	Manager::manageResponse( httpRequest const &request,
	HttpResponse &response, Config &config )
{
	if (response.getRequestStatusCode() == 400 || response.getRequestStatusCode() == 404 || response.getRequestStatusCode() == 413)
		return (response.getHeader().updateStatus(response.getRequestStatusCode()));
	std::cout << "1" << std::endl;
	Location	location = config.getSingleLocation(response.getPath());
	std::cout << "2" << std::endl;
	if (!location.isAllowedMethod(response.getMethod()))
		return (response.getHeader().updateStatus(405));
	std::cout << "3" << std::endl;
	if (!request.getBody().empty())
	{
		size_t	clientMaxBodySize = location.getMaxClientBody();
		if (clientMaxBodySize > 1)
			clientMaxBodySize = clientMaxBodySize * 1024;
		else
			clientMaxBodySize = 1024;
		clientMaxBodySize = clientMaxBodySize * 1024;
		size_t const	bodysize = request.getBody().size();
		if (bodysize > clientMaxBodySize)
			return (response.getHeader().updateStatus(413));
		response.setMaxClientBodySize(clientMaxBodySize);
	}
	std::cout << "4" << std::endl;
	if (response.treatResponsePath(location))
	{
		Mime	mime;
		std::string extension = extractPathExtension(response.getFilePath());
		std::string	mimeType = mime.getMimeType(extension);
		response.getHeader().modifyHeadersMap("Content-Type: ", mimeType);
		if (!response.getFilePath().empty())
			response.setBodysize(fileSize(response.getFilePath()));
		if (response.getToRedir())
		{
			std::cout << "REDIR" << std::endl;
			response.updateHeader();
			if (!response.sendHeader())
				throw (Webserv::NoException());
			std::cout << "HEADER SENDED" << std::endl;
		}
		else if (response.getAutoindex())
		{
			std::cout << "AUTOINDEX" << std::endl;
			if (!response.sendAutoIndex())
				throw (Webserv::NoException());
		}
		else if (response.getIsCgi())
		{
			std::cout << "CGI" << std::endl;
			CGI cgi;
			std::map<std::string, std::string> env;
			cgi.setupCGI(request, response.getFilePath(), location);
			env = cgi.getEnv();
			if (env["REDIRECT_STATUS"] != "200")
				return (response.getHeader().updateStatus(atoi(env["REDIRECT_STATUS"].c_str())));

			cgi.executeCGI(request.getBody());
			env = cgi.getEnv();
			if (env["REDIRECT_STATUS"] != "200")
				return (response.getHeader().updateStatus(atoi(env["REDIRECT_STATUS"].c_str())));

			response.updateHeader();
			std::stringstream	ss;
			ss << cgi.getOutput().size();
			response.getHeader().modifyHeadersMap("Content-Length: ", ss.str());
			if (!response.sendCgiOutput(cgi.getOutput()))
				throw (Webserv::NoException());
		}
		else
		{
			std::cout << "GET" << std::endl;
			if (response.getBodysize() > response.getMaxClientBodySize())
				return (response.getHeader().updateStatus(413));
			response.updateHeader();
			if (!response.sendWithBody())
				throw (Webserv::NoException());
			std::cout << "WITH BODY SENDED" << std::endl;
		}
	}
}

void	Manager::sendingError( HttpResponse &response, Config &config,
	std::string const &statusCode )
{
	short				code;
	std::string			location;
	std::stringstream	ss(statusCode);
	
	std::cout << "getting error page" << std::endl;
	ss >> code;
	response.getHeader().updateStatus(301);
	response.getHeader().modifyHeadersMap("Content-Type: ", "text/html");
	std::map<short, std::string>	errorPages = config.getErrorPages();
	std::map<short, std::string>::const_iterator	it = errorPages.find(code);
	if (it == errorPages.end())
	{
		location = "/error_pages/" + statusCode + ".html";
		response.getHeader().modifyHeadersMap("Location: ", location);
	}
	else
	{
		int	fd;
		location = it->second;
		std::string const	toTest = concatenateRoot(config.getRoot(), location);
		if ((fd = open(toTest.c_str(), O_RDWR)) == -1)
		{
			close(fd);
			response.getHeader().modifyHeadersMap("Location: ",
				"/error_pages/404.html");
		}
		else
		{
			close(fd);
			response.getHeader().modifyHeadersMap("Location: ",
				location);
		}
	}
	response.updateHeader();
	if (!response.sendHeader())
		throw (Webserv::NoException());
}

void	Manager::waitingForResponse( Server &server, httpRequest const &request,
	int const &fd )
{
	Config				config;
	std::vector<Config>	configs = server.getConfigs();
	int	const			socketIndex = server.getIndexSocketFromNewConnections(fd);
	
	config = configs[socketIndex];
	HttpResponse	response(request, fd);

	manageResponse(request, response, config);
	std::string const	statusCode = response.getHeader().getStatusCode();
	std::cout << "status code: " << statusCode << std::endl;
	if (statusCode != "200" && statusCode != "301")
	{
		sendingError( response, config, statusCode );
		std::cout << "DONE with status code: " << statusCode << std::endl;
	}
	else
		std::cout << "DONE" << std::endl;
	if (response.getHeader().getHeaders()["Connection: "] == "close")
	{
		epoll_ctl(server.getEpollFd(), EPOLL_CTL_DEL, fd, NULL);
		close(fd);
	}
}

/**
 * Read the request from the client, then send it to parseRequest
*/
void	Manager::readRequest( Server &server, int const &fd )
{
	char	buff[BUFFER_SIZE + 1] = {0};
	int		read_bytes = recv(fd, buff, BUFFER_SIZE, 0);
	httpRequest	request;

	if (read_bytes < 0)
	{
		perror("read failed");
		epoll_ctl(server.getEpollFd(), EPOLL_CTL_DEL, fd, NULL);
		close(fd);
		throw (Webserv::NoException());
	}
	else if (read_bytes == 0)
	{
		std::cout << YELLOW << "Timeout" << RESET << std::endl;
		epoll_ctl(server.getEpollFd(), EPOLL_CTL_DEL, fd, NULL);
		close(fd);
		return ;
	}
	buff[read_bytes] = '\0';
	std::cout << "READ: " << buff << std::endl;
	request.parseRequest(buff, read_bytes);
	std::cout << request << std::endl;
	waitingForResponse(server, request, fd);
	//int const		socketIndex = server.getIndexSocketFromNewConnections(fd);
}

void	Manager::makeAll( std::string const &filepath )
{
	Server	server;
	getMapConfig().makeAll(server, filepath);
	epollStarting(server);
	while (epollWaiting(server) != false)
		;
}
