/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Manager.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgosselk <lgosselk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/22 12:46:56 by lgosselk          #+#    #+#             */
/*   Updated: 2024/08/22 13:54:13 by lgosselk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MANAGER_HPP
# define MANAGER_HPP

# include "Server.hpp"
# include "Webserv.hpp"
# include <sys/epoll.h>

class Server;

class Manager
{
	private:
		Manager();
		~Manager();
		Manager(Manager const &copy);
		Manager const &operator=(Manager const &copy);

	public:
		void epollStarting(Server &server);
};

#endif