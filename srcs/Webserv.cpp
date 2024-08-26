/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgosselk <lgosselk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/22 10:45:42 by lgosselk          #+#    #+#             */
/*   Updated: 2024/08/22 13:49:36 by lgosselk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"

Webserv::Webserv( void )
{}

Webserv::~Webserv( void )
{}

Webserv::Webserv( Webserv const &copy )
{
	*this = copy;
}

Webserv const	&Webserv::operator=( Webserv const &copy )
{
	if (this != &copy)
	{}
	return (*this);
}

/*  */

char const	*Webserv::FileException::what() const throw()
{
	return ("Error: could not open file.");
}

char const	*Webserv::SocketException::what() const throw()
{
	return ("Error: failed to create a socket");
}

char const	*Webserv::SocketOptException::what() const throw()
{
	return ("Error: setsockopt(SO_REUSEADDR) failed");
}

char const	*Webserv::InetPtonException::what() const throw()
{
	return ("Error: Family not compatible or pAddrBuf are null/not valid");
}

char const	*Webserv::NoException::what() const throw()
{
	return ("");
}

char const	*Webserv::EpollException::what() const throw()
{
	return ("Error: failed to create epoll instance");
}