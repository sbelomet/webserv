/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   httpResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbelomet <sbelomet@42lausanne.ch>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 15:01:36 by lgosselk          #+#    #+#             */
/*   Updated: 2024/09/11 10:00:54 by sbelomet         ###   ########.fr       */
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

std::string const	HttpResponse::checkPathForDelete( Location *location )
{
	int	fd;
	std::string	path = getPath();

	if (isDirectory(path))
	{
		if (path == "/")
			return ("/" + getConfig()->getIndex());
		else if (!location->getIndex().empty())
			return (path + "/" + location->getIndex());
		else if (!getAutoindex())
		{
			getHeader().updateStatus(403);
			return (std::string());
		}
	}
	if ((fd = open(path.c_str(), O_RDWR)) == -1) // absolute path?
	{
		getHeader().updateStatus(404);
		close(fd);
		return (std::string());
	}
	close(fd);
	return (std::string());
}

static	std::string	formatRoot( std::string root )
{
	if (root == "./")
		return (".");
	if (root[0] == '.' || root[0] == '/')
	{
		if (root[0] == '.' && root[1] == '/')
			root.erase(0, 2);
		else if (root[0] == '/')
			root.erase(0, 1);
	}
	if (root[(root.size() - 1)] == '/')
		root.erase((root.size() - 1), 1);
	root = "./" + root;
	return (root);
}

std::string	const	HttpResponse::concatenateRoot( Location *location,
	std::string const &path )
{
	std::string	root;
	if (location->getRoot().empty())
		root = getConfig()->getRoot();
	else
		root = location->getRoot();
	root = formatRoot(root);
	if (root == ".")
		return (root + path);
	if (path == "/")
		return (root);
	if (path != "/")
	{
		if (root.find_last_of('/') == (root.size() - 1))
			root = root + path;
		else
			root = root + "/" + path;
		if (!(root[0] == '.' && root[1] == '/'))
			root = "./" + root;
		return (root);
	}
	return (root);
}

void	HttpResponse::buildResponsePath( Location *location )
{
	//std::string const	completePath = concatenateRoot(location, response.getPath());
	if (getToRedir())
	{
		if (!location->getReturn().path.empty())
			getHeader().modifyValuePair("Location", location->getReturn().path);
		else
			getHeader().modifyValuePair("Location", location->getIndex());
		getHeader().updateStatus(301);
		return ;
	}
	if (getMethod() == "DELETE")
	{
		std::string const newPath = checkPathForDelete(location);
		if (!newPath.empty())
		{
			std::string const	toDel = concatenateRoot(location, getPath());
			std::remove(toDel.c_str());
			// the new path is delete.html
		}
		if (!newPath.empty() && getHeader().getStatusCode() == "403")
			return ;
		else
		{
			getHeader().updateStatus(204);
			return ;
		}
	}
	if (getAutoindex())
	{

	}
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