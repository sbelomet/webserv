/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Manager.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbelomet <sbelomet@42lausanne.ch>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/22 12:46:56 by lgosselk          #+#    #+#             */
/*   Updated: 2024/09/25 13:34:21 by sbelomet         ###   ########.fr       */
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

		void	manageResponse( httpRequest const &request,
			HttpResponse &response, Config &config );	
		void	readRequest( Server &server, int const &fd );
		void	sendingError( HttpResponse &response, Config &config,
			std::string const &statusCode, bool const beforeManageResponse );
		//void	sendingError( HttpResponse &response, Config &config );
		bool	acceptConnection( Server &server, int const &socketServer );
		void	forbiddenMethodGet( HttpResponse &response, Config &config );
		void	waitingForResponse( Server &server, httpRequest const &request,
			int const &fd );
	public:
		Manager();
		~Manager();

		/* getter */

		MapConfig	&getMapConfig( void );

		/* Methods */

		void	makeAll( std::string const &filepath );
};

#endif