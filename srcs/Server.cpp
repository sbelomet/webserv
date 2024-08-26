/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgosselk <lgosselk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 11:36:10 by lgosselk          #+#    #+#             */
/*   Updated: 2024/08/22 14:46:56 by lgosselk         ###   ########.fr       */
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
		setSockets(copy.getSockets());
		setServers(copy.getServers());
		setServerAddr(copy.getServerAddr());
	}
	return (*this);
}

/* */

std::vector<int> const &Server::getSockets(void) const
{
	return (_sockets);
}

void Server::setSockets(std::vector<int> const &sockets)
{
	_sockets = sockets;
}

std::map<int, Config *> const &Server::getServers(void) const
{
	return (_servers);
}

void Server::setServers(std::map<int, Config *> const &servers)
{
	_servers = servers;
}

std::vector<sockaddr_in> const &Server::getServerAddr(void) const
{
	return (_serverAddr);
}

void Server::setServerAddr(std::vector<sockaddr_in> const &serverAddrs)
{
	_serverAddr = serverAddrs;
}

void	Server::pushSocket( int const &socket )
{
	_sockets.push_back(socket);
}

void	Server::pushServerAddr( sockaddr_in const &socket_address )
{
	_serverAddr.push_back(socket_address);
}

void	Server::insertServer( int const &index, Config *&config )
{
	_servers[index] = config;
}

/*  */

Config	*&Server::getConfigFromServer( int const &index )
{
	return (_servers[index]);
}

int const	&Server::getSocketFromSockets( size_t const &index )
{
	return (_sockets[index]);
}

sockaddr_in	&Server::getSockaddrFromServerAddr( size_t const &index )
{
	return(_serverAddr[index]);
}

/*  */

void	Server::createSockets( Config *&config )
{
	std::vector<std::string> const	listen = config->getListen();
	for (size_t i = 0; i < listen.size(); i++)
	{
		// (IPv4, TCP type socket (two way byte stream), NULL)
		int	socket_fd = socket(AF_INET, SOCK_STREAM, 0);
		if (socket_fd == -1)
			throw (Webserv::SocketException());
		int const enable = 1;
		if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
			throw (Webserv::SocketOptException());
		pushSocket(socket_fd);
		insertServer(socket_fd, config);
	}
}

void	Server::createServer( Config *&config )
{
	std::vector<std::string> const	listen = config->getListen();
	for (size_t i = 0; i < listen.size(); i++)
	{
 		sockaddr_in	address = {}; // if not work use memset
		address.sin_family = AF_INET;
		if (!config->getHost().compare("localhost"))
			address.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
		else
		{
			if (inet_pton(AF_INET, config->getHost().c_str(), &address.sin_addr) <= 0)
				throw (Webserv::InetPtonException());
		}
		address.sin_port = htons(std::atoi(listen[i].c_str()));
		pushServerAddr(address);
	}
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
		if (listen(sockets[i], SOMAXCONN) != 0)
		{
			std::cerr << RED << "Error: failed to listen socket with port -> "
				<< ntohs(getSockaddrFromServerAddr(i).sin_port) << RESET << std::endl;
			close(sockets[i]);
			throw (Webserv::NoException());
		}
		std::cout << GREEN << "listen socket with port-> "
				<< ntohs(getSockaddrFromServerAddr(i).sin_port) << RESET << std::endl;
			close(sockets[i]);
	}
}