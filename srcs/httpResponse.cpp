/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   httpResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbelomet <sbelomet@42lausanne.ch>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 15:01:36 by lgosselk          #+#    #+#             */
/*   Updated: 2024/09/06 15:42:38 by sbelomet         ###   ########.fr       */
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
    _header(HttpHeader()), _config(config), _path(request.getPath()),
	_method(request.getMethod()), _toRedir(false), _autoindex(false),
	_requestStatusCode(request.getStatusCode()),
	_maxClientBodySize(std::string()), _mimeType("text/html")
{
    getHeader().setProtocol(request.getVersion());
	std::map<std::string, std::string>	headers = request.getHeaders();
	getHeader().modifyValuePair("Connection", headers["Connection"]);
	getHeader().setAcceptTypefiles(headers["Accept"]);
}

/*  */

HttpHeader &HttpResponse::getHeader( void )
{
    return (_header);
}

bool const &HttpResponse::getToRedir( void ) const
{
	return (_toRedir);
}

bool const &HttpResponse::getAutoindex( void ) const
{
	return (_autoindex);
}

Config * const &HttpResponse::getConfig( void ) const
{
    return (_config);
}

std::string const &HttpResponse::getPath( void ) const
{
	return (_path);
}

void	HttpResponse::setToRedir( bool const &toRedir )
{
	_toRedir = toRedir;
}

HttpHeader const &HttpResponse::getHeader( void ) const
{
	return (_header);
}

std::string const &HttpResponse::getMethod( void ) const
{
	return (_method);
}

void	HttpResponse::setPath( std::string const &path )
{
	_path = path;
}

void	HttpResponse::setConfig( Config * const &config )
{
	_config = config;
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

std::string const &HttpResponse::getMaxClientBodySize( void ) const
{
	return (_maxClientBodySize);
}

void	HttpResponse::setRequestStatusCode( short const &requestStatusCode )
{
	_requestStatusCode = requestStatusCode;
}

void	HttpResponse::setMaxClientBodySize( std::string const &maxClientBodySize )
{
	_maxClientBodySize = maxClientBodySize;
}

Mime const &HttpResponse::getMime( void ) const
{
	return (_mime);
}

void	HttpResponse::updateHeader( short const &statusCode )
{
	HttpHeader	header = getHeader();
	if (statusCode != 200)
		header.modifyValuePair("Connection", "close");
	else
		header.modifyValuePair("Connection", "keep-alive");
	
	
}