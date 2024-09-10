/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   httpHeader.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbelomet <sbelomet@42lausanne.ch>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 13:38:03 by lgosselk          #+#    #+#             */
/*   Updated: 2024/09/10 10:45:57 by sbelomet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "httpHeader.hpp"

HttpHeader::HttpHeader( void ) : _sizeHeaders(10)
{
	short const size = getSizeHeaders();
	std::string	headersKey[size] = {"DEFAULT", "Access-Control-Allow-Origin: ", "Connection: ",
		"Content-Type: ", "Date: ", "Keep-Alive: ", "Location: ", "Transfer-Encoding: ",
		"X-Content-Type-Options: ", "Content-Length: "};
	std::string	headersValue[size] = {"DEFAULT", "*", std::string(), std::string(),
		std::string(), "timeout=5, max=997", std::string(), std::string(),
		"nosniff", std::string()};
	for (short i = 0; i < size; i++)
	{
		std::pair<std::string, std::string> pair;
		pair = std::make_pair<std::string, std::string>(headersKey[i], headersValue[i]);
		pushPairToHeaders(pair);
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

void	HttpHeader::setStatusCode( std::string const &statusCode )
{
	_statusCode = statusCode;
}

void	HttpHeader::setInfoStatusCode( std::string const &infoStatusCode )
{
	_infoStatusCode = infoStatusCode;
}

std::string const &HttpHeader::getAcceptTypefiles( void ) const
{
	return (_acceptTypefiles);
}

void	HttpHeader::setAcceptTypefiles( std::string const &acceptTypefiles )
{
	_acceptTypefiles = acceptTypefiles;
}

std::pair<std::string, std::string>	const	&HttpHeader::getPairFromHeaders(
	std::string const &key ) const
{
	std::vector<std::pair<std::string, std::string> >::const_iterator	it;
	for (it = getHeaders().begin(); it != getHeaders().end(); it++)
	{
		if (it->first == key)
			return (*it);
	}
	return (getHeaders()[0]);
}

std::vector<std::pair<std::string, std::string> > &HttpHeader::getHeaders( void )
{
	return (_headers);
}

void	HttpHeader::modifyValuePair( std::string const &key, std::string const &value )
{
	std::vector<std::pair<std::string, std::string> >::iterator	it;
	for (it = getHeaders().begin(); it != getHeaders().end(); it++)
	{
		if (it->first == key)
			it->second = value;
	}
}

void	HttpHeader::pushPairToHeaders( std::pair<std::string, std::string> const &pair )
{
	_headers.push_back(pair);
}

std::vector<std::pair<std::string, std::string> > const	&HttpHeader::getHeaders( void ) const
{
	return (_headers);
}

/*  */

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
		case 302:
			setStatusCode("302");
			setInfoStatusCode("Found");
			break;
		case 307:
			setStatusCode("307");
			setInfoStatusCode("Temporary Redirect");
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
		default:
			break;
	}
}
