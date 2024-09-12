/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MapConfig.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgosselk <lgosselk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 09:44:19 by lgosselk          #+#    #+#             */
/*   Updated: 2024/09/10 15:57:24 by lgosselk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MAPCONFIG_HPP
# define MAPCONFIG_HPP

# include "Server.hpp"
# include "Config.hpp"
# include "Webserv.hpp"
# include "sys/socket.h"

class Config;
class Server;

class MapConfig
{
	private:
		std::map<std::string, Config *> _map_config;
		
		MapConfig( MapConfig const &copy );
		MapConfig const &operator=( MapConfig const &copy );
		
		void	checkValidConfig( Config *config );
		void	fillLocations( Config *config );
		void	mappingConfigs( std::string const &filepath );
	public:
		MapConfig( void );
		~MapConfig( void );

		/* getters */

		std::map<std::string, Config *> &getMapConfig( void );
		std::map<std::string, Config *> const &getMapConfig( void ) const;

		/* setters */

		void setMapConfig( std::map<std::string, Config *> const &mapConfig );

		/* methods */

		Config	*getConfigFromMap( std::string const &key );
		void	insertConfig( std::string const &key, Config *config );
		void	makeAll( Server &server, std::string const &filepath );
};

std::ostream &operator<<(std::ostream &out, MapConfig &obj);

#endif
