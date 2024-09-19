/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   httpRequest.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgosselk <lgosselk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/29 10:31:13 by sbelomet          #+#    #+#             */
/*   Updated: 2024/09/19 13:58:07 by lgosselk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPREQUEST_HPP
# define HTTPREQUEST_HPP

# include <map>
# include <algorithm>
# include "Webserv.hpp"

class httpRequest
{
	private:
		short		_status_code;
		std::string	_method;
		std::string	_path;
		std::string	_version;
		std::map<std::string, std::string> _headers;
		std::string	_body;

		httpRequest( httpRequest const &copy );
		httpRequest const &operator=( httpRequest const &copy );

		int checkMethod( std::string::iterator &it, std::string::iterator reqEnd );
		int	checkPath( std::string::iterator &it, std::string::iterator reqEnd );
		int	checkVersion( std::string::iterator &it, std::string::iterator reqEnd );
		int	fillHeaders( std::string::iterator &it, std::string::iterator reqEnd );
		int	checkBody( std::string::iterator &it, std::string::iterator reqEnd );

	public:
		httpRequest();
		~httpRequest();

		/* Getters */
		short const			&getStatusCode( void ) const;
		std::string const	&getMethod( void ) const;
		std::string const	&getPath( void ) const;
		std::string const	&getVersion( void ) const;
		std::map<std::string, std::string>	&getHeaders( void );
		std::map<std::string, std::string> const &getHeaders( void ) const;
		std::string const	&getBody( void ) const;

		/* Methods */
		void	parseRequest( std::string request );
};

std::ostream	&operator<<( std::ostream &o, httpRequest const &req );

# endif