/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Manager.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgosselk <lgosselk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/22 13:34:10 by lgosselk          #+#    #+#             */
/*   Updated: 2024/08/22 14:36:16 by lgosselk         ###   ########.fr       */
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

void	Manager::epollStarting( Server &server )
{
	epoll_event	event, events[4096];

	int const	epollFd = epoll_create1(0);
	if (epollFd < 0)
		throw (Webserv::EpollException());
	
}