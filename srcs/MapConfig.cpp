/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MapConfig.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbelomet <sbelomet@42lausanne.ch>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 10:28:01 by lgosselk          #+#    #+#             */
/*   Updated: 2024/08/22 13:51:23 by sbelomet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "MapConfig.hpp"

MapConfig::MapConfig( void )
{}

MapConfig::~MapConfig( void )
{
	std::map<std::string, Config *>::iterator it;
	for (it = getMapConfig().begin(); it != getMapConfig().end(); it++)
	{
		delete it->second;
	}
}

MapConfig::MapConfig( MapConfig const &copy )
{
	*this = copy;
}

MapConfig const	&MapConfig::operator=( MapConfig const &copy )
{
	if (this != &copy)
		this->setMapConfig(copy.getMapConfig());
	return (*this);
}

/* */

std::map<std::string, Config *>  &MapConfig::getMapConfig( void )
{
	return (this->_map_config);
}

void	MapConfig::setMapConfig( std::map<std::string, Config *> const &config )
{
	this->_map_config = config;
}

std::map<std::string, Config *> const	&MapConfig::getMapConfig( void ) const
{
	return (this->_map_config);
}

void	MapConfig::insertConfig(std::string const &key, Config *config)
{
	this->_map_config[key] = config;
}

Config	*MapConfig::getConfigFromMap(std::string const &key)
{
	return (this->_map_config[key]);
}

/*  */

void	MapConfig::makeAll( Server &server )
{
	std::map<std::string, Config *>::iterator it;
	for (it = getMapConfig().begin(); it != getMapConfig().end(); it++)
	{
		server.createSockets(it->second);
	}
	std::cout << GREEN << "Sockets successfully created" << RESET << std::endl;
	for (it = getMapConfig().begin(); it != getMapConfig().end(); it++)
	{
		server.createServer(it->second);
	}
	std::cout << GREEN << "Server addresses successfully created" << RESET << std::endl;
	server.bindServers();
	server.listeningServers();
}

void	MapConfig::mappingConfigs( std::string const &filepath )
{
	std::ifstream   infile(filepath.c_str());
	if (!infile.is_open())
		throw (Webserv::FileException());

	std::string	line;
	int			lineCount = 0;
	int			serverCount = 0;
	while (std::getline(infile, line))
	{
		lineCount++; // increment line count

		if (onlyWhitespaces(line)) // ignore blank lines
			continue;

		for (std::string::iterator it = line.begin(); it != line.end(); it++) // read the line character by character
		{
			std::string::iterator kw_end;
			std::string keyword;

			// ignore comments
			if (*it == '#')
				break;

			// ignore whitespaces
			if (isWhitespace(*it))
				continue;
			// we should not have anything outside of server scope
			else if (isSeparator(*it))
			{
				std::cerr << "ERROR on line " << lineCount << ": Declaration ouside server scope" << std::endl;
				throw Webserv::NoException();
			}
			// if not on seperator, then we're on a keyword
			else
			{
				// find the end of the keyword
				kw_end = findKeywordEnd(it, line.end());
				// extract the keyword
				keyword = line.substr(it - line.begin(), (kw_end - line.begin()) - (it - line.begin()));
				// move the iterator to the end of the keyword
				it = kw_end;
			}

			// if keyword is server, create a new config object and run makeConfig
			if (keyword != "server")
			{
				std::cerr << "ERROR on line " << lineCount << ": Declaration ouside server scope" << std::endl;
				throw Webserv::NoException();
			}
			else
			{
				std::cout << "server keyword found" << std::endl;
				Config	*config = new Config();
				while (it != line.end() && isWhitespace(*it))		// skip whitespaces
					it++;
				if (it == line.end())
				{
						config->makeConfig(infile, lineCount, true);	// if no parenthesis, await for it
				}
				else if (*it == '{')
				{
						config->makeConfig(infile, lineCount, false);	// if parenthesis, do not await for it
				}
				else	// no non-whitespace characters between server keyword and parenthesis
				{
					std::cerr << "ERROR on line " << lineCount << ": Missing or obstructed opening parenthesis after scope keyword" << std::endl;
					throw Webserv::NoException();
				}
				serverCount++;
				std::string key = "server";
				std::ostringstream oss;
				oss << serverCount;
				key += oss.str();
				insertConfig(key, config);
			}

			if (it == line.end())
				break;
		}
	}
	infile.close();

	std::cout << "config root out of loop: " << getConfigFromMap("server1")->getRoot() << std::endl;
	std::cout << "config host out of loop: " << getConfigFromMap("server1")->getHost() << std::endl;
	std::cout << "config index out of loop: " << getConfigFromMap("server1")->getIndex() << std::endl;
	std::cout << "config max client body size out of loop: " << getConfigFromMap("server1")->getMaxClientBody() << std::endl;
	std::vector<std::string> server_names = getConfigFromMap("server1")->getServerName();
	for (std::vector<std::string>::iterator it = server_names.begin(); it != server_names.end(); it++)
		std::cout << "server_name: " << *it << std::endl;
	std::map<short, std::string> error_pages = getConfigFromMap("server1")->getErrorPages();
	for (std::map<short, std::string>::iterator it = error_pages.begin(); it != error_pages.end(); it++)
		std::cout << "error_pages: " << it->first << " " << it->second << std::endl;
	std::vector<std::string> listen = getConfigFromMap("server1")->getListen();
	for (std::vector<std::string>::iterator it = listen.begin(); it != listen.end(); it++)
		std::cout << "listen: " << *it << std::endl;
	std::vector<Location *> locations = getConfigFromMap("server1")->getLocations();
	for (std::vector<Location *>::iterator it = locations.begin(); it != locations.end(); it++)
	{
//		std::cout << "location root: " << (*it)->getRoot() << std::endl;
//		std::cout << "location index: " << (*it)->getIndex() << std::endl;
//		std::cout << "location alias: " << (*it)->getAlias() << std::endl;
		std::cout << "location location: " << (*it)->getLocation() << std::endl;
/* 		std::cout << "location autoindex: " << (*it)->getAutoindex() << std::endl;
		std::cout << "location max client body size: " << (*it)->getMaxClientBody() << std::endl;
		std::vector<std::string> cgi_ext = (*it)->getCgiExt();
		for (std::vector<std::string>::iterator it = cgi_ext.begin(); it != cgi_ext.end(); it++)
			std::cout << "location cgi_ext: " << *it << std::endl;
		s_return ret = (*it)->getReturn();
		std::cout << "location return path: " << ret.path << std::endl;
		std::cout << "location return status: " << ret.status << std::endl;
		s_methods methods = (*it)->getAllowedMethods();
		std::cout << "location allowed methods get: " << methods.get << std::endl;
		std::cout << "location allowed methods post: " << methods.post << std::endl;
		std::cout << "location allowed methods remove: " << methods.remove << std::endl; */
	}
}