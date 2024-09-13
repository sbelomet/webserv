/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   httpResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbelomet <sbelomet@42lausanne.ch>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 15:01:36 by lgosselk          #+#    #+#             */
/*   Updated: 2024/09/13 14:06:34 by sbelomet         ###   ########.fr       */
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

HttpResponse::HttpResponse( Config *&config, httpRequest const &request, int const &fd ):
    _header(HttpHeader()), _config(config), _fd(fd), _isOk(true), _host(std::string()),
	_path(request.getPath()), _isCgi(false), _method(request.getMethod()), _toRedir(false),
	_filePath(std::string()), _mimeType("text/html"), _bodySize(0), _autoindex(false),
	_requestStatusCode(request.getStatusCode()), _maxClientBodySize(1024 * 1024)
{
    getHeader().setProtocol(request.getVersion());
	std::map<std::string, std::string>	headers = request.getHeaders();
	setHost(headers["host"]);
	getHeader().setAcceptTypefiles(headers["accept"]);
	getHeader().modifyHeadersMap("Connection: ", headers["connection"]);
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

size_t const &HttpResponse::getBodysize( void ) const
{
	return (_bodySize);
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

bool const &HttpResponse::getIsCgi( void ) const
{
	return (_isCgi);
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

void	HttpResponse::setIsCgi( bool const &isCgi )
{
	_isCgi = isCgi;
}

void	HttpResponse::setConfig( Config * const &config )
{
	_config = config;
}

std::string const &HttpResponse::getFilePath( void ) const
{
	return (_filePath);
}

std::string const &HttpResponse::getMimeType( void ) const
{
	return (_mimeType);
}

void	HttpResponse::setHeader( HttpHeader const &header )
{
	_header = header;
}

void	HttpResponse::setBodysize( size_t const &bodySize )
{
	_bodySize = bodySize;
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

void	HttpResponse::setFilePath( std::string const &filePath )
{
	_filePath = filePath;
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
	std::string const	rootPath = concatenateRoot(location, getPath());
	if (isDirectory(rootPath))
	{
		if (!location->getIndex().empty())
			return (0);
		else if (!location->getReturn().path.empty())
		{
			setToRedir(true);
			return (0);
		}
		else if (location->getAutoindexSet())
		{
			setAutoindex(location->getAutoindex());
			return (0);
		}
		else
			return (1);
	}
	return (0);
}

bool	HttpResponse::checkPath( Location *location,
	std::string const &rootPath )
{
	int	fd;
	if (isDirectory(rootPath))
	{
		if (getPath() == "/")
		{
			setFilePath(rootPath + "/" + getConfig()->getIndex());
			return (true);
		}
		else if (!location->getIndex().empty())
		{
			setFilePath(rootPath + "/" + location->getIndex());
			return (true);
		}
		else if (!getAutoindex())
		{
			getHeader().updateStatus(403);
			return (false);
		}
		return (false);
	}
	if ((fd = open(rootPath.c_str(), O_RDWR)) == -1)
	{
		close(fd);
		getHeader().updateStatus(404);
		return (false);
	}
	close(fd);
	setFilePath(rootPath);
	return (true);
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
		return (root + path);
	return (root);
}

bool	HttpResponse::treatResponsePath( Location *location )
{
	if (getToRedir())
	{
		if (!location->getReturn().path.empty())
			getHeader().modifyHeadersMap("Location: ", location->getReturn().path);
		else
			getHeader().modifyHeadersMap("Location: ", location->getIndex());
		getHeader().updateStatus(301);
		return (true);
	}
	if (!checkPath(location,
		concatenateRoot(location, getPath())))
	{
		return (false);
	}
	if (getMethod() == "DELETE" && !getFilePath().empty())
	{
		std::cout << "DELETE" << std::endl;
		std::remove(concatenateRoot(location, getPath()).c_str());
		setFilePath("./public/deleted.html");
		return (true);
	}
	if (!location->getCgiPass().empty())
	{
		std::cout << "CGI" << std::endl;
		setIsCgi(true);
		return (true);
	}
	
	return (true);
}

bool	HttpResponse::sendWithBody( void )
{
	std::cout << "file path ->" << getFilePath() << std::endl;
	std::ifstream   infile(getFilePath().c_str());
	if (!infile.is_open())
		throw (Webserv::FileException());
	
	std::string	line;
	std::string	toSend = getHeader().composeHeader();
	toSend += "\n";
	while (std::getline(infile, line))
	{
		toSend += line + "\n";
	}
	std::cout << toSend << std::endl;
	if (send(getFd(), toSend.c_str(), toSend.size(), 0) < 0)
	{
		perror("send()");
		infile.close();
		return (false);
	}
	infile.close();
	return (true);
}

bool	HttpResponse::sendCgiOutput( std::string const &output )
{
	std::string	toSend = getHeader().composeCgiHeader();
	toSend += "\n";
	toSend += output;
	std::cout << toSend << std::endl;
	if (send(getFd(), toSend.c_str(), toSend.size(), 0) < 0)
	{
		perror("send()");
		return (false);
	}
	return (true);
}

bool	HttpResponse::sendHeader( void )
{
	std::string const	toSend = getHeader().composeHeader();
	std::cout << toSend << std::endl;
	if (send(getFd(), toSend.c_str(), toSend.size(), 0) < 0)
	{
		perror("send()");
		return (false);
	}
	return (true);
}

void	HttpResponse::updateHeader( void )
{
	if (getHeader().getStatusCode() != "200")
		getHeader().modifyHeadersMap("Connection: ", "close");
	getHeader().setFirstLine(getHeader().getProtocol() + " " + getHeader().getStatusCode()
		+ " " + getHeader().getInfoStatusCode() + "\n");
	if (getBodysize() != 0)
	{
		std::stringstream	ss;
		ss << getBodysize();
		std::cout << "Content-Length: " << ss.str() << std::endl;
		getHeader().modifyHeadersMap("Content-Length: ", ss.str());
	}
	else
		getHeader().modifyHeadersMap("Content-Length: ", "0");
}