/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   httpHeader.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbelomet <sbelomet@42lausanne.ch>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 10:33:55 by lgosselk          #+#    #+#             */
/*   Updated: 2024/09/10 10:45:46 by sbelomet         ###   ########.fr       */
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
		short				_sizeHeaders;
		std::string			_protocol;
		std::string			_firstLine;
		std::string			_statusCode; // 200, 201, 404
		std::string			_infoStatusCode; // Created, Forbidden, etc..
		std::string			_acceptTypefiles;
		std::vector<std::pair<std::string, std::string> > _headers; // Date to do
	public:
		HttpHeader( void );
		virtual ~HttpHeader( void );

		HttpHeader( HttpHeader const &copy );
		HttpHeader const &operator=( HttpHeader const &copy );

		/* getters - setters */

		short const &getSizeHeaders( void ) const;

		std::string const &getProtocol( void ) const;
		void	setProtocol( std::string const &protocol );

		std::string const &getStatusCode( void ) const;
		void	setStatusCode( std::string const &statusCode );

		std::string const &getInfoStatusCode( void ) const;
		void	setInfoStatusCode( std::string const &infoStatusCode );

		std::string const &getAcceptTypefiles( void ) const;
		void	setAcceptTypefiles( std::string const &acceptTypefiles );

		std::pair<std::string, std::string>	const	&getPairFromHeaders(
			std::string const &key ) const;
		std::vector<std::pair<std::string, std::string> >	&getHeaders( void );
		void	modifyValuePair( std::string const &key, std::string const &value );
		void	pushPairToHeaders( std::pair<std::string, std::string> const &pair );
		std::vector<std::pair<std::string, std::string> > const	&getHeaders( void ) const;

		/* methods  */

		void	updateStatus( short const &statusCode );
};

#endif