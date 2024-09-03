/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   httpHeader.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgosselk <lgosselk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 10:33:55 by lgosselk          #+#    #+#             */
/*   Updated: 2024/09/03 15:29:20 by lgosselk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPHEADER_HPP
# define HTTPHEADER_HPP

# include <iostream>
# include "Config.hpp"
# include "httpRequest.hpp"

class Config;

class HttpHeader
{
	private:
		std::string	_protocol;
		short		_sizeHeaders;
		std::string	_statusCode; // 200, 201, 404
		std::string	_infoStatusCode; // Created, Forbidden, etc..
		std::vector<std::pair<std::string, std::string> > _headers;
	public:
		HttpHeader( void );
		virtual ~HttpHeader( void );

		HttpHeader( HttpHeader const &copy );
		HttpHeader const &operator=( HttpHeader const &copy );

		short const &getSizeHeaders( void ) const;

		std::string const &getProtocol( void ) const;
		void	setProtocol( std::string const &protocol );

		std::string const &getStatusCode( void ) const;
		void	setStatusCode( std::string const &statusCode );

		std::string const &getInfoStatusCode( void ) const;
		void	setInfoStatusCode( std::string const &infoStatusCode );

		std::pair<std::string, std::string>	const	&getPairFromHeaders(
			std::string const &key ) const;
		std::vector<std::pair<std::string, std::string> >	&getHeaders( void );
		void	modifyValuePair( std::string const &key, std::string const &value );
		void	pushPairToHeaders( std::pair<std::string, std::string> const &pair );
		std::vector<std::pair<std::string, std::string> > const	&getHeaders( void ) const;
};

#endif