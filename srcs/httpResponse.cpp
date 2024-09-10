/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   httpResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbelomet <sbelomet@42lausanne.ch>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 15:01:36 by lgosselk          #+#    #+#             */
/*   Updated: 2024/09/10 10:42:22 by sbelomet         ###   ########.fr       */
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
    _header(HttpHeader()), _config(config), _fd(-1), _isOk(true), _host(std::string()),
	_path(request.getPath()), _method(request.getMethod()), _toRedir(false),
	_mimeType("text/html"), _autoindex(false), _requestStatusCode(request.getStatusCode()),
	_maxClientBodySize(1024 * 1024)
{
    getHeader().setProtocol(request.getVersion());
	std::map<std::string, std::string>	headers = request.getHeaders();
	setHost(headers["Host"]);
	getHeader().setAcceptTypefiles(headers["Accept"]);
	getHeader().modifyValuePair("Connection", headers["Connection"]);
	getHeader().updateStatus(request.getStatusCode());
}

/*  */

HttpHeader &HttpResponse::getHeader( void )
{
    return (_header);
}

int const &HttpResponse::getFd( void ) const
{
	return (_fd);
}

bool const &HttpResponse::getIsOk( void ) const
{
	return (_isOk);
}

void	HttpResponse::setIsOk( bool const &isOk )
{
	_isOk = isOk;
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

std::string const &HttpResponse::getHost( void ) const
{
	return (_host);
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

void	HttpResponse::setHost( std::string const &host )
{
	_host = host;
}

void	HttpResponse::setPath( std::string const &path )
{
	_path = path;
}

void	HttpResponse::setConfig( Config * const &config )
{
	_config = config;
}

std::string const &HttpResponse::getMimeType( void ) const
{
	return (_mimeType);
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

size_t const &HttpResponse::getMaxClientBodySize( void ) const
{
	return (_maxClientBodySize);
}

void	HttpResponse::setMimeType( std::string const &mimeType )
{
	_mimeType = mimeType;
}

void	HttpResponse::setRequestStatusCode( short const &requestStatusCode )
{
	_requestStatusCode = requestStatusCode;
}

void	HttpResponse::setMaxClientBodySize( size_t const &maxClientBodySize )
{
	_maxClientBodySize = maxClientBodySize;
}

/**
 * If path is a directory check for redirections and return 1 if not found, else 0
*/
int	HttpResponse::checkPathRedir( Location *location )
{
	if (_path.find_last_of('.') == std::string::npos
		|| _path.find_last_of('/') < _path.find_last_of('.'))
	{
		if (!location->getIndex().empty())
		{
			_toRedir = true;
		}
		else if (!location->getIndex().empty())
		{
			_toRedir = true;
		}
		else if (!location->getReturn().path.empty())
		{
			_toRedir = true;
		}
		else if (location->getAutoindexSet())
			_autoindex = location->getAutoindex();
		else
			return 1;
	}
	return 0;
}

void	HttpResponse::buildResponsePath( HttpResponse &response, Location *location )
{
	if (getToRedir())
	{
		if (!location->getReturn().path.empty())
			getHeader().modifyValuePair("Location", location->getReturn().path);
		else
			getHeader().modifyValuePair("Location", location->getIndex());
		getHeader().updateStatus(301);
		return ;
	}
	if (getAutoindex())
	{

	}
	if (getMethod() == "DELETE" )
	{
		// check complete path and open file to test
	}
	(void)response;
}

/**
 * Extracts the extension of a file from a path, returns "default" if no extension is found
*/
std::string	HttpResponse::extractPathExtension( std::string const &path )
{
	size_t	dot = path.find_last_of('.');
	if (dot != std::string::npos && dot + 1 < path.size())
		return path.substr(dot + 1);
	else
		return "default";
}

void	HttpResponse::updateHeader( short const &statusCode )
{
	HttpHeader	header = getHeader();
	if (statusCode != 200)
		header.modifyValuePair("Connection", "close");
	else
		header.modifyValuePair("Connection", "keep-alive");
	// TO DO
}