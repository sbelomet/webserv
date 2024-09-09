/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   httpResponse.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbelomet <sbelomet@42lausanne.ch>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 15:01:20 by lgosselk          #+#    #+#             */
/*   Updated: 2024/09/09 12:39:48 by sbelomet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPRESPONSE_HPP
# define HTTPRESPONSE_HPP

# include <iostream>
# include "Config.hpp"
# include "httpHeader.hpp"
# include "httpRequest.hpp"
# include "Mime.hpp"

class Config;
class HttpHeader;

class HttpResponse
{
	private:
		HttpHeader	_header;
		Config		*_config;

		int			_fd;
		bool		_isOk; // bool to know is response is 200
		std::string	_path; // ex. -> /index.html
		std::string	_method; // GET or POST or DELETE
		bool		_toRedir; // return is location 
		std::string	_mimeType; // ex. -> text/html, image/jpeg
		bool		_autoindex; // listing or not directories
		short		_requestStatusCode; // status code of request parsing
		size_t		_maxClientBodySize; // max size body for response
		
		HttpResponse( void );
		HttpResponse( HttpResponse const &copy );
		HttpResponse const &operator=( HttpResponse const &copy );
	public:
		~HttpResponse( void );
		HttpResponse( Config *&config, httpRequest const &request );

		/* getters - setters */

		int const &getFd( void ) const;

		bool const &getIsOk( void ) const;
		void	setIsOk( bool const &path );

		bool const &getToRedir( void ) const;
		void	setToRedir( bool const &toRedir );

		std::string const &getPath( void ) const;
		void	setPath( std::string const &path );

		Config * const &getConfig( void ) const;
		void	setConfig( Config * const &config );

		bool const &getAutoindex( void ) const;
		void	setAutoindex( bool const &autoindex );

		HttpHeader &getHeader( void );
		HttpHeader const &getHeader( void ) const;
		void	setHeader( HttpHeader const &header );

		std::string const &getMethod( void ) const;
		void	setMethod( std::string const &method );

		short const &getRequestStatusCode( void ) const;
		void	setRequestStatusCode( short const &requestStatusCode );

		size_t const &getMaxClientBodySize( void ) const;
		void	setMaxClientBodySize( size_t const &requestStatusCode );

		/* methods */

		void	updateHeader( short const &statusCode );
		int		checkPathRedir( Location *location, Location *rootLocation );
		void	buildCheckPath( HttpResponse &response, Location *location );
		std::string	extractPathExtension( std::string const &path );
};

#endif