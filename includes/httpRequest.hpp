/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   httpRequest.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbelomet <sbelomet@42lausanne.ch>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/29 10:31:13 by sbelomet          #+#    #+#             */
/*   Updated: 2024/09/05 11:16:53 by sbelomet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPREQUEST_HPP
# define HTTPREQUEST_HPP

# include <iostream>
# include <algorithm>
# include <map>
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
		bool		_haveBody;
		size_t		_bodySize;

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
		bool const			&getHaveBody( void ) const;
		size_t const		&getBodySize( void ) const;

		/* Methods */
		void	parseRequest( char *buffer, int const &size );
};

std::ostream	&operator<<( std::ostream &o, httpRequest const &req );

# endif