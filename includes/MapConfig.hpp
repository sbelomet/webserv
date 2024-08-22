/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MapConfig.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbelomet <sbelomet@42lausanne.ch>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 09:44:19 by lgosselk          #+#    #+#             */
/*   Updated: 2024/08/22 12:59:38 by sbelomet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MAPCONFIG_HPP
# define MAPCONFIG_HPP

# include <vector>
# include <sstream>
# include <fstream>
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
	public:
		MapConfig( void );
		~MapConfig( void );
		MapConfig( MapConfig const &copy );

		MapConfig const &operator=( MapConfig const &copy );

		/* getters */

		std::map<std::string, Config *> &getMapConfig(void);
		std::map<std::string, Config *> const &getMapConfig(void) const;

		/* setters */

		void setMapConfig(std::map<std::string, Config *> const &mapConfig);

		/* methods */

		void	makeAll( Server &server );
		Config	*getConfigFromMap(std::string const &key);
		void	mappingConfigs(std::string const &filepath);
		void	insertConfig(std::string const &key, Config *config);
};

#endif
