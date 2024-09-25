/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   httpHeader.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbelomet <sbelomet@42lausanne.ch>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 13:38:03 by lgosselk          #+#    #+#             */
/*   Updated: 2024/09/25 14:00:06 by sbelomet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "httpHeader.hpp"

HttpHeader::HttpHeader( void ): _sizeHeaders(8)
{
	short const size = getSizeHeaders();
	std::string	headersKey[size] = {"DEFAULT", "Access-Control-Allow-Origin: ", "Connection: ",
		"Content-Type: ", "Keep-Alive: ", "Location: ", "X-Content-Type-Options: ",
		"Content-Length: "};
	std::string	headersValue[size] = {"DEFAULT", "*", std::string(), std::string(),
	"timeout=5, max=997", std::string(), "nosniff", std::string()};
	for (int i = 0; i < size; i++)
	{
		modifyHeadersMap(headersKey[i], headersValue[i]);
	}
}

HttpHeader::~HttpHeader( void )
{}

HttpHeader::HttpHeader( HttpHeader const &copy )
{
	*this = copy;
}

HttpHeader const	&HttpHeader::operator=( HttpHeader const &copy )
{
	if (this != &copy)
	{
		setProtocol(copy.getProtocol());
		setStatusCode(copy.getStatusCode());
		setInfoStatusCode(copy.getInfoStatusCode());
	}
	return (*this);
}

/*  */

short const &HttpHeader::getSizeHeaders( void ) const
{
	return (_sizeHeaders);
}

std::string const &HttpHeader::getProtocol( void ) const
{
	return (_protocol);
}

std::string const &HttpHeader::getFirstLine( void ) const
{
	return (_firstLine);
}

std::string const &HttpHeader::getStatusCode( void ) const
{
	return (_statusCode);
}

void	HttpHeader::setProtocol( std::string const &protocol )
{
	_protocol = protocol;
}

std::string const &HttpHeader::getInfoStatusCode( void ) const
{
	return (_infoStatusCode);
}

std::string const &HttpHeader::getAcceptTypefiles( void ) const
{
	return (_acceptTypefiles);
}

void	HttpHeader::setFirstLine( std::string const &firstLine )
{
	_firstLine = firstLine;
}

std::map<std::string, std::string> &HttpHeader::getHeaders( void )
{
	return (_headers);
}

void	HttpHeader::setStatusCode( std::string const &statusCode )
{
	_statusCode = statusCode;
}

void	HttpHeader::setInfoStatusCode( std::string const &infoStatusCode )
{
	_infoStatusCode = infoStatusCode;
}

void	HttpHeader::setAcceptTypefiles( std::string const &acceptTypefiles )
{
	_acceptTypefiles = acceptTypefiles;
}

std::map<std::string, std::string> const	&HttpHeader::getHeaders( void ) const
{
	return (_headers);
}

void	HttpHeader::modifyHeadersMap( std::string const &key, std::string const &value )
{
	_headers[key] = value;
}

/*  */

std::string const	HttpHeader::composeHeader( void )
{
	std::string	toSend;

	toSend = getFirstLine();
	std::map<std::string, std::string>::const_iterator it;
	for (it = getHeaders().begin(); it != getHeaders().end(); it++)
	{
		if ((it->first == "Location: " && it->second.empty())
			|| it->first == "DEFAULT" ||
			(it->first == "Content-Length: " && (it->second == "0")))
			continue ;
		toSend += it->first + it->second + "\n";
	}
	return (toSend);
}

std::string const	HttpHeader::composeCgiHeader( void )
{
	std::string	toSend;

	toSend = getFirstLine();
	std::map<std::string, std::string>::const_iterator it;
	for (it = getHeaders().begin(); it != getHeaders().end(); it++)
	{
		if ((it->first == "Location: " && it->second.empty())
			|| it->first == "DEFAULT")
			continue ;
		toSend += it->first + it->second + "\n";
	}
	return (toSend);
}

void	HttpHeader::updateStatus( short const &statusCode )
{
	switch (statusCode)
	{
		case 200:
			setStatusCode("200");
			setInfoStatusCode("Ok");
			break;
		case 204:
			setStatusCode("204");
			setInfoStatusCode("No Content");
			break;
		case 301:
			setStatusCode("301");
			setInfoStatusCode("Moved Permanently");
			break;
		case 400:
			setStatusCode("400");
			setInfoStatusCode("Bad Request");
			break;
		case 401:
			setStatusCode("401");
			setInfoStatusCode("Unauthorized");
			break;
		case 403:
			setStatusCode("403");
			setInfoStatusCode("Forbidden");
			break;
		case 404:
			setStatusCode("404");
			setInfoStatusCode("Not Found");
			break;
		case 405:
			setStatusCode("405");
			setInfoStatusCode("Method Not Allowed");
			break;
		case 406:
			setStatusCode("406");
			setInfoStatusCode("Not Acceptable");
			break;
		case 413:
			setStatusCode("413");
			setInfoStatusCode("Payload Too Large");
			break;
		case 418:
			setStatusCode("418");
			setInfoStatusCode("I'm a teapot");
			break;
		case 500:
			setStatusCode("500");
			setInfoStatusCode("Internal Server Error");
			break;
		case 501:
			setStatusCode("501");
			setInfoStatusCode("Not Implemented");
			break;
		case 502:
			setStatusCode("502");
			setInfoStatusCode("Bad Gateway");
			break;
		case 503:
			setStatusCode("503");
			setInfoStatusCode("Service Unavailable");
			break;
		case 504:
			setStatusCode("504");
			setInfoStatusCode("Gateway Timeout");
			break;
		default:
			break;
	}
	return ;
}
