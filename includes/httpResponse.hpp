/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   httpResponse.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbelomet <sbelomet@42lausanne.ch>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 15:01:20 by lgosselk          #+#    #+#             */
/*   Updated: 2024/09/10 10:36:01 by sbelomet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPRESPONSE_HPP
# define HTTPRESPONSE_HPP

# include "Mime.hpp"
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

		int			_fd; // file descriptor to send response
		bool		_isOk; // bool to know is response is 200
		std::string	_host;
		std::string	_path; // ex. -> /index.html
		std::string	_method; // GET or POST or DELETE
		bool		_toRedir; // if location have a return 
		std::string	_mimeType; // ex. -> text/html, image/jpeg
		bool		_autoindex; // listing or not directories
		short		_requestStatusCode; // status code of request at parsing
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

		std::string const &getHost( void ) const;
		void	setHost( std::string const &host );

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

		std::string const &getMimeType( void ) const;
		void	setMimeType( std::string const &mimeType );

		short const &getRequestStatusCode( void ) const;
		void	setRequestStatusCode( short const &requestStatusCode );

		size_t const &getMaxClientBodySize( void ) const;
		void	setMaxClientBodySize( size_t const &requestStatusCode );

		/* methods */

		void	updateHeader( short const &statusCode );
		int		checkPathRedir( Location *location );
		void	buildResponsePath( HttpResponse &response, Location *location );
		std::string	extractPathExtension( std::string const &path );
};

#endif