/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   httpResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgosselk <lgosselk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 15:01:36 by lgosselk          #+#    #+#             */
/*   Updated: 2024/09/03 15:48:10 by lgosselk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "httpResponse.hpp"

HttpResponse::HttpResponse( void )
{}

HttpResponse::~HttpResponse( void )
{}

HttpResponse::HttpResponse( HttpResponse const &copy )
{
	*this = copy;
}

HttpResponse const	&HttpResponse::operator=( HttpResponse const &copy )
{
	if (this != &copy)
	{
        setConfig(copy.getConfig());
        setHeader(copy.getHeader());
    }
	return (*this);
}

HttpResponse::HttpResponse( Config *&config, httpRequest const &request ):
    _header(HttpHeader()), _config(config), _method(request.getMethod()),
	_location(request.getPath()), _autoindex(false),
	_requestStatusCode(request.getStatusCode())
{
    getHeader().setProtocol(request.getVersion());
	std::map<std::string, std::string>	headers = request.getHeaders();
	getHeader().modifyValuePair("Connection", headers["Connection"]);
}

/*  */

HttpHeader &HttpResponse::getHeader( void )
{
    return (_header);
}

bool const &HttpResponse::getAutoindex( void ) const
{
	return (_autoindex);
}

Config * const &HttpResponse::getConfig( void ) const
{
    return (_config);
}

HttpHeader const &HttpResponse::getHeader( void ) const
{
	return (_header);
}

std::string const &HttpResponse::getMethod( void ) const
{
	return (_method);
}

void	HttpResponse::setConfig( Config * const &config )
{
	_config = config;
}

std::string const &HttpResponse::getLocation( void ) const
{
	return (_location);
}

void	HttpResponse::setHeader( HttpHeader const &header )
{
	_header = header;
}

void	HttpResponse::setAutoindex( bool const &autoindex )
{
	_autoindex = autoindex;
}

void	HttpResponse::setMethod( std::string const &method )
{
	_method = method;
}

short const &HttpResponse::getRequestStatusCode( void ) const
{
	return (_requestStatusCode);
}

void	HttpResponse::setLocation( std::string const &location )
{
	_location = location;
}

void	HttpResponse::setRequestStatusCode( short const &requestStatusCode )
{
	_requestStatusCode = requestStatusCode;
}

/*  */

void	HttpResponse::sendResponse( void )
{
	std::FILE	*tmpFile = std::tmpfile();
	if (tmpFile == NULL)
		throw (Webserv::NoException());
	if (getRequestStatusCode() == 400)
	{
		HttpHeader	header = getHeader();
		std::string	headersKey[7] = {"Access-Control-Allow-Origin: ", "Connection: ",
		"Content-Type: ", "Keep-Alive: ", "Transfer-Encoding: ", "X-Content-Type-Options: ",
		"Content-Length: "};
		std::string	firstLine = header.getProtocol() + " " + "400" + "Bad Request\n";
		std::fputs(firstLine.c_str(), tmpFile);
		// TO DO
	}
}
