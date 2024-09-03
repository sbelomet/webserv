/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   httpResponse.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgosselk <lgosselk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 15:01:20 by lgosselk          #+#    #+#             */
/*   Updated: 2024/09/03 15:27:02 by lgosselk         ###   ########.fr       */
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

		std::string	_method;
		std::string _location;
		bool		_autoindex; // listing files if no index and autoindex is enable
		short		_requestStatusCode;
		
		HttpResponse( void );
		HttpResponse( HttpResponse const &copy );
		HttpResponse const &operator=( HttpResponse const &copy );
	public:
		~HttpResponse( void );
		HttpResponse( Config *&config, httpRequest const &request );

		bool const &getAutoindex( void ) const;
		void	setAutoindex( bool const &autoindex );

		Config * const &getConfig( void ) const;
		void	setConfig( Config * const &config );

		HttpHeader &getHeader( void );
		HttpHeader const &getHeader( void ) const;
		void	setHeader( HttpHeader const &header );

		std::string const &getMethod( void ) const;
		void	setMethod( std::string const &method );

		std::string const &getLocation( void ) const;
		void	setLocation( std::string const &location );

		short const &getRequestStatusCode( void ) const;
		void	setRequestStatusCode( short const &requestStatusCode );

		void	sendResponse( void );
};

#endif