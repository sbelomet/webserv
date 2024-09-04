/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   httpHeader.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbelomet <sbelomet@42lausanne.ch>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 13:38:03 by lgosselk          #+#    #+#             */
/*   Updated: 2024/09/04 15:34:57 by sbelomet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "httpHeader.hpp"

HttpHeader::HttpHeader( void ): _firstLine(std::string())
{
	short const size = getSizeHeaders();
	std::string	headersKey[size] = {"Access-Control-Allow-Origin: ", "Connection: ",
		"Content-Type: ", "Date: ", "Keep-Alive: ", "Location: ", "Transfer-Encoding: ",
		"X-Content-Type-Options: ", "Content-Length: "};
	std::string	headersValue[size] = {"*", std::string(), std::string(),
		std::string(), "timeout=5, max=997", std::string(), std::string(),
		"nosniff", std::string()};
	for (size_t i = 0; i < size; i++)
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

/*
* Return need to be checked, pair can be empty.
*/
std::pair<std::string, std::string>	const	HttpHeader::getPairFromHeaders(
	std::string const &key ) const
{
	std::vector<std::pair<std::string, std::string> >::const_iterator	it;
	for (it = getHeaders().begin(); it != getHeaders().end(); it++)
	{
		if (it->first == key)
			return (*it);
	}
	return (std::make_pair(std::string(), std::string()));
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
	if (statusCode == 400)
	{
		// TO DO
	}
}