/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   httpResponse.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbelomet <sbelomet@42lausanne.ch>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 15:01:20 by lgosselk          #+#    #+#             */
/*   Updated: 2024/09/25 12:58:10 by sbelomet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPRESPONSE_HPP
# define HTTPRESPONSE_HPP

# include <ctime>
# include <fcntl.h>
# include "Mime.hpp"
# include <iostream>
# include <unistd.h>
# include <dirent.h>

# include "httpHeader.hpp"
# include "httpRequest.hpp"

class HttpHeader;
class HttpRequest;

class HttpResponse
{
	private:
		HttpHeader	_header;

		int			_fd; // file descriptor to send response
		bool		_isOk; // bool to know is response is 200
		std::string	_path; // ex. -> /index.html
		bool		_isCgi; // if location have a cgi
		std::string	_method; // GET or POST or DELETE
		bool		_toRedir; // if location have a return
		std::string	_filePath; // path with root for checking and opening
		std::string	_mimeType; // ex. -> text/html, image/jpeg
		size_t		_bodySize; // size of body response
		bool		_autoindex; // listing or not directories
		short		_requestStatusCode; // status code of request at parsing
		
		HttpResponse( void );
		HttpResponse( HttpResponse const &copy );
		HttpResponse const &operator=( HttpResponse const &copy );
	public:
		~HttpResponse( void );
		HttpResponse(httpRequest const &request, int const &fd );

		/* getters - setters */

		int const	&getFd( void ) const;
		void		setFd( int const &fd );

		bool const &getIsOk( void ) const;
		void	setIsOk( bool const &path );

		bool const &getIsCgi( void ) const;
		void	setIsCgi( bool const &isCgi );

		bool const &getToRedir( void ) const;
		void	setToRedir( bool const &toRedir );

		std::string const &getPath( void ) const;
		void	setPath( std::string const &path );

		bool const &getAutoindex( void ) const;
		void	setAutoindex( bool const &autoindex );

		size_t const &getBodysize( void ) const;
		void	setBodysize( size_t const &bodySize );

		HttpHeader &getHeader( void );
		HttpHeader const &getHeader( void ) const;
		void	setHeader( HttpHeader const &header );

		std::string const &getMethod( void ) const;
		void	setMethod( std::string const &method );

		std::string const &getMimeType( void ) const;
		void	setMimeType( std::string const &mimeType );

		std::string const &getFilePath( void ) const;
		void	setFilePath( std::string const &filePath );

		short const &getRequestStatusCode( void ) const;
		void	setRequestStatusCode( short const &requestStatusCode );

		/* methods */
		bool	sendHeader( void );
		void	updateHeader( void );
		bool	sendWithBody( void );
		bool	sendNoNotFound( void );

		bool	sendAutoIndex( void );
		bool	checkPath( Location location,
			std::string const &rootPath );
		std::string	addTimeAndSize( int const &type );
		bool	treatResponsePath( Location location );
		std::string	getDirectories( std::map<std::string,
			int> const &directoryContent );
		std::string	getRegularFiles( std::map<std::string,
			int> const &directoryContent);
		bool	sendCgiOutput( std::string const &output );
		std::string	const	concatenateRoot( Location location,
			std::string const &path );
		std::string	buildLine( std::string const &name, int const &type );
};

#endif