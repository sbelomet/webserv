/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Manager.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgosselk <lgosselk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/22 12:46:56 by lgosselk          #+#    #+#             */
/*   Updated: 2024/09/04 13:28:32 by lgosselk         ###   ########.fr       */
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