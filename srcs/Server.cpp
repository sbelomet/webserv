/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgosselk <lgosselk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 11:36:10 by lgosselk          #+#    #+#             */
/*   Updated: 2024/09/25 13:37:11 by lgosselk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server( void )
{}

Server::~Server( void )
{}

Server::Server( Server const &copy )
{
	*this = copy;
}

Server const	&Server::operator=( Server const &copy )
{
	if (this != &copy)
	{
		setEpollfd(copy.getEpollFd());
		setSockets(copy.getSockets());
		setConfigs(copy.getConfigs());
		setServerAddr(copy.getServerAddr());
	}
	return (*this);
}

/* */

std::vector<Config> &Server::getConfigs(void)
{
	return (_configs);
}

int	const	&Server::getEpollFd( void ) const
{
	return (_epollFd);
}

void	Server::setEpollfd( int const &epollFd )
{
	_epollFd = epollFd;
}

std::vector<int> const &Server::getSockets(void) const
{
	return (_sockets);
}

void Server::setSockets(std::vector<int> const &sockets)
{
	_sockets = sockets;
}

std::vector<Config> const &Server::getConfigs(void) const
{
	return (_configs);
}

void Server::setConfigs(std::vector<Config> const &configs)
{
	_configs = configs;
}

std::map<int, int> const &Server::getNewConnections( void ) const
{
	return (_newConnections);
}

std::vector<sockaddr_in> const &Server::getServerAddr(void) const
{
	return (_serverAddr);
}

void Server::setServerAddr(std::vector<sockaddr_in> const &serverAddrs)
{
	_serverAddr = serverAddrs;
}

void	Server::setNewConnections( std::map<int, int> const &newConnections )
{
	_newConnections = newConnections;
}

/*  */

void	Server::pushSocket( int const &socket )
{
	_sockets.push_back(socket);
}

void	Server::pushConfig( Config const &config )
{
	_configs.push_back(config);
}

void	Server::pushServerAddr( sockaddr_in const &socket_address )
{
	_serverAddr.push_back(socket_address);
}

void	Server::insertNewConnection( int const &newConnection, int const &index )
{
	_newConnections[newConnection] = index;
}

/*  */

int const	&Server::getSocketFromSockets( size_t const &index )
{
	return (_sockets[index]);
}

sockaddr_in	&Server::getSockaddrFromServerAddr( size_t const &index )
{
	return(_serverAddr[index]);
}

int	const	&Server::getIndexSocketFromNewConnections( int const &index )
{
	return (_newConnections[index]);
}

/*  */

void	Server::createSockets( Config config )
{
	// (IPv4, TCP type socket (two way byte stream), NULL)
	int	socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_fd == -1)
		throw (Webserv::SocketException());
	int const enable = 1;
	if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
		throw (Webserv::SocketOptException());
	pushSocket(socket_fd);
	pushConfig(config);
}

void	Server::createServer( Config config )
{
	std::string const	listen = config.getListen();
	sockaddr_in	address = {};
	address.sin_family = AF_INET;
	if (!config.getHost().compare("localhost"))
		address.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	else
	{
		if (inet_pton(AF_INET, config.getHost().c_str(), &address.sin_addr) <= 0)
			throw (Webserv::InetPtonException());
	}
	address.sin_port = htons(std::atoi(listen.c_str()));
	pushServerAddr(address);
}

void	Server::bindServers( void )
{
	std::vector<int> const	sockets = getSockets();
	for (size_t i = 0; i < sockets.size(); i++)
	{
		if (bind(sockets[i], reinterpret_cast<sockaddr *>(&(getSockaddrFromServerAddr(i))),
			sizeof(getSockaddrFromServerAddr(i))) != 0)
		{
			std::cerr << RED << "Error: failed to bind socket to port -> "
				<< ntohs(getSockaddrFromServerAddr(i).sin_port) << RESET << std::endl;
			throw (Webserv::NoException());
		}
	}
}

void	Server::listeningServers( void )
{
	std::vector<int> const	sockets = getSockets();
	for (size_t i = 0; i < sockets.size(); i++)
	{
		if (fcntl(sockets[i], F_SETFL, fcntl(sockets[i], F_GETFL, 0) | O_NONBLOCK) < 0)
		{
			perror("fcntl: listeningServers()");
			throw (Webserv::NoException());
		}
		if (listen(sockets[i], SOMAXCONN) != 0)
		{
			std::cerr << RED << "Error: failed to listen socket with port -> "
				<< ntohs(getSockaddrFromServerAddr(i).sin_port) << RESET << std::endl;
			throw (Webserv::NoException());
		}
		std::cout << GREEN << "listen socket with port-> "
			<< ntohs(getSockaddrFromServerAddr(i).sin_port) << RESET << std::endl;
	}
}

int	Server::newConnection( int const &eventFd )
{
	std::vector<int> const	sockets = getSockets();
	for (size_t i = 0; i < sockets.size(); i++)
	{
		if (sockets[i] == eventFd)
			return (static_cast<int>(i));
	}
	return (-1);
}