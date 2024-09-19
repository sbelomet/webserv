/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MapConfig.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgosselk <lgosselk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 09:44:19 by lgosselk          #+#    #+#             */
/*   Updated: 2024/09/19 15:13:38 by lgosselk         ###   ########.fr       */
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
		//std::map<std::string, Config *> _map_config;
		
		MapConfig( MapConfig const &copy );
		MapConfig const &operator=( MapConfig const &copy );
		
		void	checkValidConfig( Config &config );
		void	fillLocations( Config &config );
		std::vector<Config>	mappingConfigs( std::string const &filepath );
	public:
		MapConfig( void );
		virtual ~MapConfig( void );

		/* methods */

		void	makeAll( Server &server, std::string const &filepath );
};

#endif
