/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   httpRequest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgosselk <lgosselk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/29 10:31:17 by sbelomet          #+#    #+#             */
/*   Updated: 2024/09/25 15:21:22 by lgosselk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "httpRequest.hpp"

httpRequest::httpRequest( void ) : _status_code(0), _method(""),
	_path(""), _version(""), _body("") {} 
httpRequest::~httpRequest( void ) {}
httpRequest::httpRequest( httpRequest const &copy ) { *this = copy; }

httpRequest const	&httpRequest::operator=( httpRequest const &copy )
{
	if (this != &copy)
	{
		_status_code = copy._status_code;
		_method = copy._method;
		_path = copy._path;
		_version = copy._version;
		_headers = copy._headers;
		_body = copy._body;
	}
	return (*this);
}

short const			&httpRequest::getStatusCode( void ) const { return (_status_code); }
std::string const	&httpRequest::getMethod( void ) const { return (_method); }
std::string const	&httpRequest::getPath( void ) const { return (_path); }
std::string const	&httpRequest::getVersion( void ) const { return (_version); }
std::map<std::string, std::string>	&httpRequest::getHeaders( void ) { return (_headers); }
std::map<std::string, std::string> const &httpRequest::getHeaders( void ) const { return (_headers); }
std::string const	&httpRequest::getBody( void ) const { return (_body); }

/**
 * Parse the request and populate the class attributes
 */
void	httpRequest::parseRequest( std::string request )
{
	//std::string		request(buffer, size);
	std::string::iterator it = request.begin();

	if (checkMethod(it, request.end()))
	{
		std::cout << "Method fucked up" << std::endl;
		return ;
	}
	if (checkPath(it, request.end()))
	{
		std::cout << "Path fucked up" << std::endl;
		return ;
	}
	if (checkVersion(it, request.end()))
	{
		std::cout << "version fucked up" << std::endl;
		return ;
	}
	if (fillHeaders(it, request.end()))
	{
		std::cout << "headers fucked up" << std::endl;
		return ;
	}
	if (checkBody(it, request.end()))
	{

		std::cout << "body fucked up" << std::endl;
		return ;
	}
	_status_code = 200;
}

/**
 * Extract and check the method of the request. Moves the iterator to the next character after the method
 */
int	httpRequest::checkMethod( std::string::iterator &it, std::string::iterator reqEnd )
{
	std::string::iterator end = std::find(it, reqEnd, ' ');
	if (*end != ' ' || *(end + 1) != '/')
	{
		_status_code = 400;
		return 1;
	}
	_method = std::string(it, end);
	if (_method != "GET" && _method != "POST" && _method != "DELETE")
	{
		_status_code = 400;
		return 1;
	}
	it = end + 1;
	return 0;
}

/**
 * Extract and check the path of the request. Moves the iterator to the next character after the path
 */
int	httpRequest::checkPath( std::string::iterator &it, std::string::iterator reqEnd )
{
	std::string::iterator end = std::find(it, reqEnd, ' ');
	if (*end != ' ' || *(end + 1) != 'H')
	{
		_status_code = 400;
		return 1;
	}
	std::string tmp(it, end);
	if (tmp.size() > 1 && *(end - 1) == '/')
	{
		_status_code = 404;
		return 1;
	}
	if (tmp.find("?") != std::string::npos)
	{
		_status_code = 501;
		return 1;
	}
	_path = tmp;
	it = end + 1;
	return 0;
}

/**
 * Extract and check the version of the request. Moves the iterator to the next line
 */
int	httpRequest::checkVersion( std::string::iterator &it, std::string::iterator reqEnd )
{
	std::string::iterator end = std::find(it, reqEnd, '\r');
	if (*end != '\r' || *(end + 1) != '\n')
	{
		_status_code = 400;
		return 1;
	}
	_version = std::string(it, end);
	if (_version.substr(0, 5) != "HTTP/")
	{
		_status_code = 400;
		return 1;
	}
	it = end + 2;
	return 0;
}

/**
 * Extract the headers from the request. Moves the iterator to the end of the headers
 */
int	httpRequest::fillHeaders( std::string::iterator &it, std::string::iterator reqEnd )
{
	std::string::iterator end = std::find(it, reqEnd, '\r');
	while (*it != '\r' && it != reqEnd)
	{
		std::string::iterator colon = std::find(it, end, ':');
		if (*colon != ':' || *(colon + 1) != ' ')
		{
			_status_code = 400;
			return 1;
		}
		std::string key(it, colon);
		strtolower(key);
		std::string value(colon + 2, end);
		_headers[key] = value;
		it = end + 2;
		end = std::find(it, reqEnd, '\r');
	}
	return 0;
}

/**
 * Extract the body from the request. Moves the iterator to the end of the body
 */
int	httpRequest::checkBody( std::string::iterator &it, std::string::iterator reqEnd )
{
	std::string::iterator end = std::find(it, reqEnd, '\r');
	if (*end != '\r' || *(end + 1) != '\n')
	{
		_status_code = 400;
		return 1;
	}
	it = end + 2;
	std::string tmp(it, reqEnd);
	/* if (_headers.find("content-type") != _headers.end() && _headers["content-type"].find("boundary=") != std::string::npos)
	{
		std::string boundary = _headers["content-type"].substr(_headers["content-type"].find("boundary=") + 9);
		boundary += "--";
		std::string::size_type pos = tmp.find(boundary);
		if (pos == std::string::npos)
		{
			_status_code = 413;
			return 1;
		}
	} */
	_body = tmp;
	return 0;
}

std::ostream	&operator<<( std::ostream &o, httpRequest const &req )
{
	o << BLUE << "Status code: " << RESET << req.getStatusCode() << std::endl;
	o << BLUE << "Method: " << RESET << req.getMethod() << std::endl;
	o << BLUE << "Path: " << RESET << req.getPath() << std::endl;
	o << BLUE << "Version: " << RESET << req.getVersion() << std::endl;
	std::map<std::string, std::string> const headers = req.getHeaders();
	for (std::map<std::string, std::string>::const_iterator it = headers.begin(); it != headers.end(); it++)
		o << BLUE << it->first << ": " << RESET << it->second << std::endl;
	o << BLUE << "Body: " << RESET << req.getBody() << std::endl;
	return (o);
}