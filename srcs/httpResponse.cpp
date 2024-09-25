/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   httpResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgosselk <lgosselk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 15:01:36 by lgosselk          #+#    #+#             */
/*   Updated: 2024/09/25 15:40:21 by lgosselk         ###   ########.fr       */
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
        setHeader(copy.getHeader());
		setFd(copy.getFd());
		setIsOk(copy.getFd());
		setPath(copy.getPath());
		setIsCgi(copy.getIsCgi());
		setMethod(copy.getMethod());
		setToRedir(copy.getToRedir());
		setFilePath(copy.getFilePath());
		setMimeType(copy.getMimeType());
		setBodysize(copy.getBodysize());
		setAutoindex(copy.getAutoindex());
		setRequestStatusCode(copy.getRequestStatusCode());
    }
	return (*this);
}

HttpResponse::HttpResponse( httpRequest const &request, int const &fd ):
    _header(HttpHeader()), _fd(fd), _isOk(true), _isCgi(false), _toRedir(false),
	_filePath(std::string()), _mimeType("text/html"), _bodySize(0),
	_autoindex(false), _requestStatusCode(request.getStatusCode())
{
	if (getRequestStatusCode() == 200)
	{
		setPath(request.getPath());	
		setMethod(request.getMethod());
		getHeader().updateStatus(200);
		getHeader().setProtocol(request.getVersion());
		std::map<std::string, std::string>	headers = request.getHeaders();
		getHeader().setAcceptTypefiles(headers["accept"]);
		getHeader().modifyHeadersMap("Connection: ", headers["connection"]);
	}
	else
	{
		setPath("/");	
		setMethod("GET");
		getHeader().updateStatus(400);
		getHeader().setProtocol("HTTP/1.1");
		std::map<std::string, std::string>	headers = request.getHeaders();
		getHeader().setAcceptTypefiles("*/*");
		getHeader().modifyHeadersMap("Connection: ", "close");
	}
}

/*  */

HttpHeader &HttpResponse::getHeader( void )
{
    return (_header);
}

void	HttpResponse::setFd( int const &fd )
{
	_fd = fd;
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

void	HttpResponse::setPath( std::string const &path )
{
	_path = path;
}

void	HttpResponse::setIsCgi( bool const &isCgi )
{
	_isCgi = isCgi;
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

bool	HttpResponse::checkPath( Location location,
	std::string const &rootPath )
{
	int	fd;
	if (isDirectory(rootPath))
	{
		//std::cout << "IS A DIRECTORY" << std::endl;
		if (!location.getIndex().empty())
		{
			//std::cout << "HAVE A INDEX" << std::endl;
			setFilePath(rootPath + "/" + location.getIndex());
			return (true);
		}
		else if (!location.getReturn().empty())
		{
			//std::cout << "HAVE A RETURN" << std::endl;
			setToRedir(true);
			return (true);
		}
		else if (location.getAutoindex())
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
	//std::cout << "IS A FILE" << std::endl;
	setFilePath(rootPath);
	return (true);
}

std::string	const	HttpResponse::concatenateRoot( Location location,
	std::string const &path )
{
	std::string	root = location.getRoot();
	root = formatRoot(root);
	if (root == ("." + path))
		return (root);
	if (root == ".")
		return (root + path);
	if (path == "/")
		return (root);
	if (path != "/")
		return (root + path);
	return (root);
}

bool	HttpResponse::treatResponsePath( Location location )
{
	if (!checkPath(location,
		concatenateRoot(location, getPath())))
		return (false);
	if (getToRedir())
	{
		getHeader().modifyHeadersMap("Location: ", location.getReturn());
		getHeader().updateStatus(301);
		return (true);
	}
	if (getMethod() == "DELETE" && !getFilePath().empty())
	{
		std::remove(concatenateRoot(location, getPath()).c_str());
		std::cout << GREEN << "file deleted" << RESET << std::endl;
		getHeader().updateStatus(204);
		return (true);
	}
	if (!location.getCgiPass().empty())
	{
		setIsCgi(true);
		return (true);
	}
	return (true);
}

static std::string	formatTime( std::string const &time )
{
	std::vector<std::string> const	timeParts = vecSplit(time, ':');
	std::string	const				result = timeParts[0] + ":" + timeParts[1];
	return (result);
}

static std::string	formatDate( std::string const &date )
{
	std::vector<std::string> const	dateParts = vecSplit(date, ' ');
	if (dateParts.size() == 5)
	{
		std::string const	result = dateParts[2] + "-" + dateParts[1] + "-"
		+ dateParts[4] + " " + formatTime(dateParts[3]);
		return (result);
	}
	else
		return (date);
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
	time = formatDate(time);
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
		ending = name + "</a>" ;
	return (ending);
}

static	std::string	backwardRoot( std::string const &path )
{
	size_t						index;
	std::string					result = "";
	std::vector<std::string>	pathWords = vecSplit(path, '/');
	std::string					lastWord = pathWords[pathWords.size() - 1];

	for (size_t i = 0; i < pathWords.size(); i++)
	{
		if (pathWords[i] == lastWord)
		{
			index = i;
			break ;
		}	
	}
	if (index == 0)
	{
		result = "/";
		return (result);
	}
	for (size_t i = 0; i < index; i++)
	{
		result += "/" + pathWords[i];
	}
	return (result);
}

std::string	HttpResponse::buildLine( std::string const &name, int const &type )
{
	std::string	line;
	line = "\t\t\t<div id=\"anchors\">";
	if (type == DT_DIR && name == "..")
		line += "\t\t\t\t<a href=\"" + backwardRoot(getPath()) + "\" > " + addEnding(name);
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
	if (send(getFd(), toSend.c_str(), toSend.size(), 0) < 0)
	{
		perror("send()");
		return (false);
	}
	return (true);
}

bool	HttpResponse::sendWithBody( void )
{
	//std::cout << "SEND WITH BODY" << std::endl;
	std::ifstream   infile(getFilePath().c_str());
	if (!infile.is_open())
		throw (Webserv::FileException());
	setBodysize(fileSize(getFilePath()));
	std::string	line;
	updateHeader();
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
	std::string	toSend = getHeader().composeHeader();
	toSend += "\n";
	toSend += output;
	if (send(getFd(), toSend.c_str(), toSend.size(), 0) < 0)
	{
		perror("send()");
		return (false);
	}
	return (true);
}

bool	HttpResponse::sendNoNotFound( void )
{
	std::string	toSend;
	toSend = "HTTP/1.1 404 Not Found\n";
	toSend += "Content-Type: text/html\n";
	toSend += "Content-Length: 46\n";
	toSend += "\n";
	toSend += "<h1>Can't even find the 404 page damn :(</h1>\n";
	std::cout << "toSend: " << toSend << std::endl;
	if (send(getFd(), toSend.c_str(), toSend.size(), 0) < 0)
	{
		perror("send()");
		return (false);
	}
	return (true);
}

bool	HttpResponse::sendHeader( void )
{
	updateHeader();
	//std::cout << "SEND ONLY HEADER" << std::endl;
	std::string const	toSend = getHeader().composeHeader();
	//std::cout << toSend << std::endl;
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
	if (getHeader().getProtocol().empty())
		getHeader().setProtocol("HTTP/1.1");
	getHeader().setFirstLine(getHeader().getProtocol() + " " + getHeader().getStatusCode()
		+ " " + getHeader().getInfoStatusCode() + "\n");
	if (getBodysize() != 0)
	{
		std::stringstream	ss;
		ss << getBodysize();
		getHeader().modifyHeadersMap("Content-Length: ", ss.str());
	}
	else
		getHeader().modifyHeadersMap("Content-Length: ", "0");
}