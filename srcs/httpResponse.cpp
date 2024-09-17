/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   httpResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbelomet <sbelomet@42lausanne.ch>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 15:01:36 by lgosselk          #+#    #+#             */
/*   Updated: 2024/09/17 11:50:26 by sbelomet         ###   ########.fr       */
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

HttpResponse::HttpResponse( Config *config, httpRequest const &request, int const &fd ):
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

bool	HttpResponse::checkPath( Location *location,
	std::string const &rootPath )
{
	int	fd;
	if (isDirectory(rootPath))
	{
		std::cout << "IS A DIRECTORY" << std::endl;
		if (!location->getIndex().empty())
		{
			std::cout << "HAVE A INDEX" << std::endl;
			setFilePath(rootPath + "/" + location->getIndex());
			return (true);
		}
		else if (!location->getReturn().path.empty())
		{
			std::cout << "HAVE A RETURN" << std::endl;
			setToRedir(true);
			return (true);
		}
		else if (location->getAutoindex())
		{
			setAutoindex(true);
			setFilePath(rootPath);
			return (true);
		}
		getHeader().updateStatus(403);
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
	if (!checkPath(location,
		concatenateRoot(location, getPath())))
		return (false);
	if (getToRedir())
	{
		if (!location->getReturn().path.empty())
			getHeader().modifyHeadersMap("Location: ", location->getReturn().path);
		else
			getHeader().modifyHeadersMap("Location: ", location->getIndex());
		getHeader().updateStatus(301);
		return (true);
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

std::string	HttpResponse::addTimeAndSize( int const &type )
{
	struct stat	info;
	std::string line;
	std::string	time;
	std::string	size = "<div>";

	stat(getFilePath().c_str(), &info);
	time = ctime(&info.st_mtime);
	time = time.substr(0, (time.size() - 1));
	// SPLIT AND REORDER TIME IN GOOD FORMAT
	line = "<div>" + time + "</div>";
	if (type == DT_REG)
	{
		std::stringstream	ss;
		ss << info.st_size;
		size += ss.str() + "</div>";
	}
	else
		size += "- </div>";
	line += size;
	return (line);
}

static std::string	addEnding( std::string const &name )
{
	std::string		ending;
	size_t	const	space = 52;

	if (name.size() > space)
		ending = name.substr(0, space - 3) + "..></a> ";
	else if (name == "..")
		ending = name + "/" + "</a> ";
	else
		ending = name + std::string((space - name.size()), ' ') + "</a>" ;
	return (ending);
}

std::string	HttpResponse::buildLine( std::string const &name, int const &type )
{
	std::string	line;
	line = "\t\t\t<div id=\"anchors\">";
	if (type == DT_DIR && name == "..")
		line += "\t\t\t\t<a href=\"" + getPath() + "\" > " + addEnding(name); // change getPath for root ".."
	else if (type == DT_DIR)
		line += "\t\t\t\t<a href=\"" + getPath() + "/" + name + "\" > " + addEnding(name);
	else if (type == DT_REG)
		line += "\t\t\t\t<a href=\"" + getPath() + "/" + name + "\" > " + addEnding(name);
	if (name != "..")
		line += addTimeAndSize(type);
	line += "</div>\n";
	return (line);
}

std::string	HttpResponse::getDirectories(
	std::map<std::string, int> const &directoryContent )
{
	std::string	line = "";
	std::map<std::string, int>::const_iterator	it;

	for (it = directoryContent.begin(); it != directoryContent.end(); it++)
	{
		if (it->second == DT_DIR)
			line += buildLine(it->first, it->second);
	}
	return (line);
}

std::string	HttpResponse::getRegularFiles(
	std::map<std::string, int> const &directoryContent )
{
	std::string	line = "";
	std::map<std::string, int>::const_iterator	it;

	for (it = directoryContent.begin(); it != directoryContent.end(); it++)
	{
		if (it->second == DT_REG)
			line += buildLine(it->first, it->second);
	}
	return (line);
}

static	bool	getDirectoryContent(std::string const &filePath,
	std::map<std::string, int> &directoryContent )
{
	dirent	*file;
	DIR		*direc;

	direc = opendir(filePath.c_str());
	if (direc == NULL)
	{
		perror("opendir: ");
		return (false);
	}
	while ((file = readdir(direc)) != NULL)
	{
		std::string	name(file->d_name);
		if (name != ".")
		{
			if (file->d_type == DT_DIR)
				directoryContent[name] = DT_DIR;
			else if (file->d_type == DT_REG)
				directoryContent[name] = DT_REG;
		}
	}
	if (closedir(direc) == -1)
	{
		perror("closedir: ");
		return (false);
	}
	return (true);
}

bool	HttpResponse::sendAutoIndex( void )
{
	std::string					body;
	std::string					toSend;
	std::map<std::string, int>	directoryContent;

	if (!getDirectoryContent(getFilePath(), directoryContent))
		throw (Webserv::NoException());
	body = "<!DOCTYPE html>\n<html lang=\"en\">\n\t<head>\n\t\t<meta charset=\"UTF-8\">";
	body += "<link rel=\"stylesheet\" href=\"/styles/autoindex.css\">";
	body += "\n\t\t<title>Autoindex</title>\n\t</head>\n\t<body>\n";
	body += "\t\t<h1>Index of " + getPath() + "</h1>\n\t\t<section id=\"secAuto\">\n";
	body += getDirectories(directoryContent);
	body += getRegularFiles(directoryContent);
	body += "\t\t</section>\n\t</body>\n</html>";
	setBodysize(body.size());
	updateHeader();
	toSend = getHeader().composeHeader() + "\n";
	toSend += body;
	std::cout << toSend << std::endl;
	if (send(getFd(), toSend.c_str(), toSend.size(), 0) < 0)
	{
		perror("send()");
		return (false);
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
	//std::cout << toSend << std::endl;
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
	//std::cout << toSend << std::endl;
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
		//std::cout << "Content-Length: " << ss.str() << std::endl;
		getHeader().modifyHeadersMap("Content-Length: ", ss.str());
	}
	else
		getHeader().modifyHeadersMap("Content-Length: ", "0");
}