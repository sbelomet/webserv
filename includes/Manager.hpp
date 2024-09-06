/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Manager.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbelomet <sbelomet@42lausanne.ch>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/22 12:46:56 by lgosselk          #+#    #+#             */
/*   Updated: 2024/09/06 15:21:45 by sbelomet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MANAGER_HPP
# define MANAGER_HPP

# include "unistd.h"
# include "Server.hpp"
# include "Webserv.hpp"
# include <sys/epoll.h>
# include "httpRequest.hpp"
# include "httpResponse.hpp"
# include "CGI.hpp"

class Server;

class Manager
{
	private:
		MapConfig	_map_config;

		Manager( Manager const &copy );
		Manager const &operator=( Manager const &copy );

		bool	epollWaiting( Server &server );
		void	epollStarting( Server &server );
		void	readRequest( Server &server, int const &fd );
		bool	acceptConnection( Server &server, int const &index );
		void	manageResponse( Server &server, httpRequest const &request,
			int const &socketIndex );
	public:
		Manager();
		~Manager();

		/* getter */

		MapConfig	&getMapConfig( void );

		/* Methods */

		void	makeAll( Server &server, std::string const &filepath );
};

#endif