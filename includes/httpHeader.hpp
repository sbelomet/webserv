/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   httpHeader.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbelomet <sbelomet@42lausanne.ch>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 10:33:55 by lgosselk          #+#    #+#             */
/*   Updated: 2024/09/13 13:01:51 by sbelomet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPHEADER_HPP
# define HTTPHEADER_HPP

# include <iostream>
# include "Config.hpp"
# include <sys/socket.h>
# include "httpRequest.hpp"

class Config;

class HttpHeader
{
	private:
		std::string			_protocol;
		std::string			_firstLine;
		std::string			_statusCode; // 200, 201, 404
		short				_sizeHeaders;
		std::string			_infoStatusCode; // Created, Forbidden, etc..
		std::string			_acceptTypefiles;

		std::map<std::string, std::string>	_headers;
	public:
		HttpHeader( void );
		virtual ~HttpHeader( void );

		HttpHeader( HttpHeader const &copy );
		HttpHeader const &operator=( HttpHeader const &copy );

		/* getters - setters */

		short const &getSizeHeaders( void ) const;

		std::string const &getProtocol( void ) const;
		void	setProtocol( std::string const &protocol );

		std::string const &getFirstLine( void ) const;
		void	setFirstLine( std::string const &firstLine );

		std::string const &getStatusCode( void ) const;
		void	setStatusCode( std::string const &statusCode );

		std::string const &getInfoStatusCode( void ) const;
		void	setInfoStatusCode( std::string const &infoStatusCode );

		std::string const &getAcceptTypefiles( void ) const;
		void	setAcceptTypefiles( std::string const &acceptTypefiles );

		std::map<std::string, std::string>	&getHeaders( void );
		std::map<std::string, std::string> const	&getHeaders( void ) const;
		void	modifyHeadersMap( std::string const &key, std::string const &value );

		/* methods  */

		std::string const	composeHeader( void );
		std::string const	composeCgiHeader( void );
		void	updateStatus( short const &statusCode );
};

#endif