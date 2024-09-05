/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   httpResponse.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgosselk <lgosselk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 15:01:20 by lgosselk          #+#    #+#             */
/*   Updated: 2024/09/05 10:14:47 by lgosselk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPRESPONSE_HPP
# define HTTPRESPONSE_HPP

# include <iostream>
# include "Config.hpp"
# include "httpHeader.hpp"
# include "httpRequest.hpp"

class Config;
class HttpHeader;

class HttpResponse
{
	private:
		HttpHeader	_header;
		Config		*_config;

		std::string	_path; // ex. -> /index.html
		std::string	_method; // GET or POST or DELETE
		bool		_toRedir; // return is location 
		bool		_autoindex; // listing or not directories
		short		_requestStatusCode; // status code of request parsing
		std::string	_maxClientBodySize; // TO DO
		
		HttpResponse( void );
		HttpResponse( HttpResponse const &copy );
		HttpResponse const &operator=( HttpResponse const &copy );
	public:
		~HttpResponse( void );
		HttpResponse( Config *&config, httpRequest const &request );

		/* getters - setters */

		std::string const &getPath( void ) const;
		void	setPath( std::string const &path );

		bool const &getToRedir( void ) const;
		void	setToRedir( bool const &toRedir );

		bool const &getAutoindex( void ) const;
		void	setAutoindex( bool const &autoindex );

		Config * const &getConfig( void ) const;
		void	setConfig( Config * const &config );

		HttpHeader &getHeader( void );
		HttpHeader const &getHeader( void ) const;
		void	setHeader( HttpHeader const &header );

		std::string const &getMethod( void ) const;
		void	setMethod( std::string const &method );

		short const &getRequestStatusCode( void ) const;
		void	setRequestStatusCode( short const &requestStatusCode );

		std::string const &getMaxClientBodySize( void ) const;
		void	setMaxClientBodySize( std::string const &requestStatusCode );

		/* methods */

		void	updateHeader( short const &statusCode );
};

#endif