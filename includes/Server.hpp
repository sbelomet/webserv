/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgosselk <lgosselk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 11:35:54 by lgosselk          #+#    #+#             */
/*   Updated: 2024/09/19 11:07:45 by lgosselk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include <map>
# include <cstdlib>
# include <fcntl.h>
# include "Webserv.hpp"
# include <arpa/inet.h>
# include "MapConfig.hpp"

class Config;

class Server
{
    private:
		int							_epollFd;
		std::vector<int>			_sockets;
		std::vector<Config>			_configs;
		std::map<int, int>			_newConnections;
		std::vector<struct sockaddr_in>	_serverAddr;

		Server( Server const &copy );
		Server const &operator=( Server const &copy );
    public:
		Server( void );
		virtual ~Server( void );
		
		/* getters - setters */

		int	const	&getEpollFd( void ) const;
		void	setEpollfd( int const &epollFd );

		std::vector<int> const &getSockets( void ) const;
		void	setSockets( std::vector<int> const &sockets );

		std::vector<Config>	&getConfigs( void );
		std::vector<Config> const &getConfigs( void ) const;
		void	setConfigs( std::vector<Config> const &configs );

		std::map<int, int> const &getNewConnections( void ) const;
		void	setNewConnections( std::map<int, int> const &newConnections );

		std::vector<struct sockaddr_in> const &getServerAddr( void ) const;
		void	setServerAddr( std::vector<struct sockaddr_in> const &serverAddrs );

		/* insert to containers methods */

		void	pushSocket( int const &socket );
		void	pushConfig( Config const &config );
		void	pushServerAddr( sockaddr_in const &socket_address );
		void	insertNewConnection( int const &newConnection, int const &index );

		/* get individual */

		int const		&getSocketFromSockets( size_t const &index );
		sockaddr_in		&getSockaddrFromServerAddr( size_t const &index );
		int	const		&getIndexSocketFromNewConnections( int const &index );

		/* Methods */

		void	bindServers( void );
		void	listeningServers( void );
		void	createServer( Config config );
		void	createSockets( Config config );
		int		newConnection( int const &eventFd );
};

#endif