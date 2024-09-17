/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Manager.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbelomet <sbelomet@42lausanne.ch>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/22 12:46:56 by lgosselk          #+#    #+#             */
/*   Updated: 2024/09/17 13:53:52 by sbelomet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MANAGER_HPP
# define MANAGER_HPP

# include "CGI.hpp"
# include "unistd.h"
# include <signal.h>
# include "Server.hpp"
# include "Webserv.hpp"
# include <sys/epoll.h>
# include "httpRequest.hpp"
# include "httpResponse.hpp"

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
		void	waitingForResponse( Server &server, httpRequest const &request,
			int const &fd );
		void	manageResponse( httpRequest const &request,
			HttpResponse &response );
	public:
		Manager();
		~Manager();

		/* getter */

		MapConfig	&getMapConfig( void );

		/* Methods */

		void	makeAll( Server &server, std::string const &filepath );
};

#endif