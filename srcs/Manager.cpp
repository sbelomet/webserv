/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Manager.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbelomet <sbelomet@42lausanne.ch>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/22 13:34:10 by lgosselk          #+#    #+#             */
/*   Updated: 2024/09/12 13:43:40 by sbelomet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Manager.hpp"

Manager::Manager( void )
{}

Manager::~Manager( void )
{}

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
	HttpResponse &response )
{
	if (response.getRequestStatusCode() == 400 || response.getRequestStatusCode() == 404)
		return (response.getHeader().updateStatus(response.getRequestStatusCode()));
	Location	*location = response.getConfig()->getSingleLocation(response.getPath());
	if (location == NULL)
		return (response.getHeader().updateStatus(404));
	if (!location->isAllowedMethod(response.getMethod()))
		return (response.getHeader().updateStatus(405));
	if (response.checkPathRedir(location) == 1)
		return (response.getHeader().updateStatus(404));
	if (!request.getBody().empty())
	{
		size_t	clientMaxBodySize = location->getMaxClientBody();
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
	if (response.treatResponsePath(location))
	{
		Mime	mime;
		std::string extension = extractPathExtension(response.getFilePath());
		std::string	mimeType = mime.getMimeType(extension);
		response.getHeader().modifyValuePair("Content-Type", mimeType);
		response.setBodysize(fileSize(response.getFilePath()));
		if (response.getToRedir())
		{
			response.updateHeader();
			if (!response.sendHeader())
				throw (Webserv::NoException());
		}
		else if (response.getAutoindex())
		{
			// check if autoindex -> send autoindex
		}
		// check is cgi -> setcontent type, update header, env, exec CGI
		// check is post
		else // get 
		{}
	}
}

void	Manager::waitingForResponse( Server &server, httpRequest const &request,
	int const &socketIndex )
{
	HttpResponse	response(server.getConfigFromServer(
					server.getSockets()[server.getSocketFromSockets(socketIndex)]),
					request);

	manageResponse(request, response);
	std::string const	statusCode = response.getHeader().getStatusCode();
	if (statusCode != "200")
	{
		response.getHeader().modifyValuePair("Content-Type", "text/html");
	}
	else
	{

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
	request.parseRequest(buff, read_bytes);
	std::cout << request << std::endl;
	waitingForResponse(server, request, server.getIndexSocketFromNewConnections(fd));
	//int const		socketIndex = server.getIndexSocketFromNewConnections(fd);

/* 	Config *a = server.getConfigFromServer(server.getSockets()
				[server.getSocketFromSockets(server.getIndexSocketFromNewConnections(fd))]);
	CGI cgi;
	cgi.fillEnv("cgi-bin/hello.py", a->getSingleLocation("/cgi-bin"));
	// check status code
	cgi.executeCGI(fd);
	// check status code */
}

void	Manager::makeAll( Server &server, std::string const &filepath )
{
	getMapConfig().makeAll(server, filepath);
	//std::cout << _map_config << std::endl;
	epollStarting(server);
	while (epollWaiting(server) != false)
		;
}

/**
 * POST:
 * 1. check content-length and max body size
 * 2. check content-type (multipart/form-data and boundary)
 * 3. put body in a file
 * 4. send file to cgi
 * 
*/