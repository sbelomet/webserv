/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgosselk <lgosselk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 10:16:14 by lgosselk          #+#    #+#             */
/*   Updated: 2024/09/09 13:41:46 by lgosselk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_HPP
# define CONFIG_HPP

# include <map>
# include <algorithm>
# include <cstdlib>
# include <cerrno>
# include <climits>
# include <sstream>
# include <fstream>
# include "Webserv.hpp"
# include "Location.hpp"

class Location;

class Config
{
	private:
		std::string						_root;
		std::string						_host;
		std::string						_index;
		std::vector<std::string>		_listen;
		std::vector<Location *>			_locations;
		std::vector<std::string>		_server_name;
		std::map<short, std::string>	_error_pages;
		bool							_has_root_location;
		size_t							_client_max_body_size;

		Config( Config const &copy );
		Config const &operator=( Config const &copy );

		void lexingAfterVar(	std::string const &line, int const &lineCount, std::string const &keyword,
			std::string::iterator &it, Location *location, bool &afterSepNW, bool &afterVar );

		void parseKeywordRoot( std::vector<std::string> const &values, int const &lineCount, Location *location );
		void parseKeywordHost( std::vector<std::string> const &values, int const &lineCount, Location *location );
		void parseKeywordIndex( std::vector<std::string> const &values, int const &lineCount, Location *location );
		void parseKeywordAlias( std::vector<std::string> const &values, int const &lineCount, Location *location );
		void parseKeywordListen( std::vector<std::string> const &values, int const &lineCount, Location *location );
		void parseKeywordReturn( std::vector<std::string> const &values, int const &lineCount, Location *location );
		void parseKeywordCgiPass( std::vector<std::string> const &values, int const &lineCount, Location *location );
		void parseKeywordErrorPage( std::vector<std::string> const &values, int const &lineCount, Location *location );
		void parseKeywordAutoindex( std::vector<std::string> const &values, int const &lineCount, Location *location );
		void parseKeywordServerName( std::vector<std::string> const &values, int const &lineCount, Location *location );
		void parseKeywordAllowedMethods( std::vector<std::string> const &values, int const &lineCount, Location *location );
		void parseKeywordClientMaxBodySize( std::vector<std::string> const &values, int const &lineCount, Location *location );
		void parseKeyword( std::string const &keyword, std::vector<std::string> const &values, int const &lineCount,
			Location *location );
	public:
		Config();
		~Config();

		/* getters */

		std::string const	&getRoot( void ) const;
		std::string const	&getHost( void ) const;
		std::string const	&getIndex( void ) const;
		size_t const	&getMaxClientBody( void ) const;
		bool const		&getHasRootLocation( void ) const;
		std::vector<std::string> const	&getListen( void ) const;
		std::vector<Location *> const	&getLocations( void ) const;
		std::vector<std::string> const	&getServerName( void ) const;
		std::map<short, std::string> const	&getErrorPages( void ) const;

		/* setters */

		void	setRoot( std::string const & );
		void	setHost( std::string const & );
		void	setIndex( std::string const & );
		void	setMaxClientBody( size_t const & );
		void	setLocations( std::vector<Location *> const & );
		void	setListen( std::vector<std::string> const & );
		void	setServerName( std::vector<std::string> const & );
		void	setErrorPages( std::map<short, std::string> const & );

		/* access methods for vectors/map */

		void		pushLocation( Location *&location );
		void		pushListen( std::string const &listen );
		Location	*getSingleLocation( std::string const &path );
		void		pushServerName( std::string const &serverName );
		void		insertErrorPage( short const &num, std::string const &file );

		/* methods */

		void makeConfig( std::ifstream &infile, int &lineCount, bool awaitParenth );
};

#endif
