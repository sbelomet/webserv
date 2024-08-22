/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgosselk <lgosselk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 11:35:54 by lgosselk          #+#    #+#             */
/*   Updated: 2024/08/22 10:54:20 by lgosselk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include <map>
# include <vector>
# include <cstdlib>
# include <unistd.h>
# include <iostream>
# include "Webserv.hpp"
# include <arpa/inet.h>
# include "MapConfig.hpp"

class Config;

class Server
{
    private:
        std::vector<int>			_sockets;
        std::map<int, Config *>		_servers;
		std::vector<struct sockaddr_in>	_serverAddr;
    public:
		Server( void );
		~Server( void );
		Server( Server const &copy );

		Server const &operator=( Server const &copy );

		/* getters - setters */

		std::vector<int> const &getSockets(void) const;
		void setSockets(std::vector<int> const &sockets);

		std::map<int, Config *> const &getServers(void) const;
		void setServers(std::map<int, Config *> const &servers);

		std::vector<struct sockaddr_in> const &getServerAddr(void) const;
		void setServerAddr(std::vector<struct sockaddr_in> const &serverAddrs);

		/* insert to containers methods */

		void	pushSocket( int const &socket );
		void	pushServerAddr( sockaddr_in const &socket_address );
		void	insertServer( int const &index, Config *&config );

		/* get individual */
		
		Config		*&getConfigFromServer( int const &index );
		int const	&getSocketFromSockets( size_t const &index );
		sockaddr_in	&getSockaddrFromServerAddr( size_t const &index );

		/* Methods */

		void	bindServers( void );
		void	listeningServers( void );
		void	createServer( Config *&config );
		void	createSockets( Config *&config );
};

#endif