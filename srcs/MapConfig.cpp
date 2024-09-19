/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MapConfig.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgosselk <lgosselk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 10:28:01 by lgosselk          #+#    #+#             */
/*   Updated: 2024/09/19 11:16:39 by lgosselk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "MapConfig.hpp"

MapConfig::MapConfig( void )
{}

MapConfig::~MapConfig( void )
{
	//std::map<std::string, Config *>::iterator it;
	//std::cout << "delete configs" << std::endl;
	//for (it = getMapConfig().begin(); it != getMapConfig().end(); it++)
	//{
	//	delete it->second;
	//}
}

MapConfig::MapConfig( MapConfig const &copy )
{
	*this = copy;
}

MapConfig const	&MapConfig::operator=( MapConfig const &copy )
{
	if (this != &copy)
	{}
	return (*this);
}

/* */

/*std::map<std::string, Config *>  &MapConfig::getMapConfig( void )
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
}*/

/*  */

void	MapConfig::makeAll( Server &server, std::string const &filepath )
{
	std::vector<Config> configs = mappingConfigs(filepath);
	if (configs.empty())
		throw (Webserv::NoException());
	std::cout << "configs ready" << std::endl;
	for (size_t i = 0; i < configs.size(); i++)
	{
		server.createSockets(configs[i]);
		server.createServer(configs[i]);
	}
	std::cout << GREEN << "Sockets and Servers successfully created" << RESET << std::endl;
	server.bindServers();
	server.listeningServers();
}

/**
 * Parse the configuration file to create the server objects, further parsing is done in Config::makeConfig
 */
std::vector<Config>	MapConfig::mappingConfigs( std::string const &filepath )
{
	std::vector<Config>	configs;
	std::ifstream   infile(filepath.c_str());
	if (!infile.is_open())
		throw (Webserv::FileException());
	if (infile.peek() == std::ifstream::traits_type::eof())
	{
		std::cerr << "ERROR: Empty configuration file" << std::endl;
		throw Webserv::NoException();
	}

	std::string	line;
	int			lineCount = 0;
	//int			serverCount = 0;
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
				Config	config;
				while (it != line.end() && isWhitespace(*it))		// skip whitespaces
					it++;
				if (it == line.end())
				{
					config.makeConfig(infile, lineCount, true);	// if no parenthesis, await for it
				}
				else if (*it == '{')
				{
					config.makeConfig(infile, lineCount, false);	// if parenthesis, do not await for it
				}
				else	// no non-whitespace characters between server keyword and parenthesis
				{
					std::cerr << "ERROR on line " << lineCount << ": Missing or obstructed opening parenthesis after scope keyword" << std::endl;
					throw Webserv::NoException();
				}
				checkValidConfig(config);
				//serverCount++;
				//std::string key = "server";
				//std::ostringstream oss;
				//oss << serverCount;
				//key += oss.str();
				//insertConfig(key, config);
				configs.push_back(config);
				std::cout << config << std::endl;
				return (configs);
			}
			if (it == line.end())
				break;
		}
	}
	infile.close();
	return (configs);
}

/**
 * Check if the configuration is valid
 */
void	MapConfig::checkValidConfig( Config &config )
{
	if (config.getRoot().empty())
	{
		std::cerr << "ERROR: root value is missing" << std::endl;
		throw Webserv::NoException();
	}
	if (config.getHost().empty())
	{
		std::cerr << "ERROR: host value is missing" << std::endl;
		throw Webserv::NoException();
	}
	if (config.getIndex().empty())
	{
		std::cerr << "ERROR: index value is missing" << std::endl;
		throw Webserv::NoException();
	}
	if (config.getListen().empty())
	{
		std::cerr << "ERROR: listen value is missing" << std::endl;
		throw Webserv::NoException();
	}
	if (config.getLocations().empty())
	{
		std::cerr << "ERROR: root location is missing" << std::endl;
		throw Webserv::NoException();
	}
	if (config.getHasRootLocation() == false)
	{
		std::cerr << "ERROR: root location is missing" << std::endl;
		throw Webserv::NoException();
	}
	fillLocations(config);
}

/**
 * Fill in missing values in the location block if possible
 */
void	MapConfig::fillLocations( Config &config )
{
	size_t rootLocationIndex;

	// find the root location and fill in missing values
	for (size_t i = 0; i < config.getLocations().size(); i++)
	{
		if (config.getLocations()[i].getLocation() == "/")
		{
			rootLocationIndex = i;
			if (config.getLocations()[i].getMaxClientBody() == 1)
				config.getLocations()[i].setMaxClientBody(config.getMaxClientBody());
			if (config.getLocations()[i].getRoot().empty())
				config.getLocations()[i].setRoot(config.getRoot());
			if (config.getLocations()[i].getIndex().empty())
				config.getLocations()[i].setIndex(config.getIndex());
			break;
		}
	}

	// fill in missing values in the other locations
	for (size_t i = 0; i < config.getLocations().size(); i++)
	{
		if (config.getLocations()[i].getLocation() == "/")
			continue;
		if (config.getLocations()[i].getRoot().empty())
		{
			if (!config.getLocations()[rootLocationIndex].getRoot().empty())
				config.getLocations()[i].setRoot(config.getLocations()[rootLocationIndex].getRoot());
		}
		if (config.getLocations()[i].getAlias().empty())
		{
			if (!config.getLocations()[rootLocationIndex].getAlias().empty())
				config.getLocations()[i].setAlias(config.getLocations()[rootLocationIndex].getAlias());
		}
		if (config.getLocations()[i].getAutoindexSet() == false)
		{
			if (config.getLocations()[rootLocationIndex].getAutoindexSet() == true)
				config.getLocations()[i].setAutoindex(config.getLocations()[rootLocationIndex].getAutoindex());
		}
		if (config.getLocations()[i].getCgiPass().empty())
		{
			if (!config.getLocations()[rootLocationIndex].getCgiPass().empty())
				config.getLocations()[i].setCgiPass(config.getLocations()[rootLocationIndex].getCgiPass());
		}
		if (config.getLocations()[i].getReturn().empty())
		{
			if (!config.getLocations()[rootLocationIndex].getReturn().empty())
				config.getLocations()[i].setReturn(config.getLocations()[rootLocationIndex].getReturn());
		}
		if (!config.getLocations()[i].getGet() && !config.getLocations()[i].getPost() && !config.getLocations()[i].getRemove())
		{
			config.getLocations()[i].setGet(config.getLocations()[rootLocationIndex].getGet());
			config.getLocations()[i].setPost(config.getLocations()[rootLocationIndex].getPost());
			config.getLocations()[i].setRemove(config.getLocations()[rootLocationIndex].getRemove());
		}
		if (config.getLocations()[i].getMaxClientBody() == 1)
		{
			if (config.getLocations()[rootLocationIndex].getMaxClientBody() != 1)
				config.getLocations()[i].setMaxClientBody(config.getLocations()[rootLocationIndex].getMaxClientBody());
		}
	}
}

/*std::ostream &operator<<(std::ostream &out, MapConfig &obj)
{
	for (std::map<std::string, Config *>::iterator it = obj.getMapConfig().begin(); it != obj.getMapConfig().end(); it++)
	{
		out << MAGENTA << it->first << "\n{" << RESET << std::endl;
		out << RED << "  root: " << RESET << obj.getConfigFromMap(it->first)->getRoot() << std::endl;
		out << RED << "  host: " << RESET << obj.getConfigFromMap(it->first)->getHost() << std::endl;
		out << RED << "  index: " << RESET << obj.getConfigFromMap(it->first)->getIndex() << std::endl;
		out << RED << "  max client body size: " << RESET << obj.getConfigFromMap(it->first)->getMaxClientBody() << std::endl;
		std::vector<std::string> server_names = obj.getConfigFromMap(it->first)->getServerName();
		for (std::vector<std::string>::iterator it = server_names.begin(); it != server_names.end(); it++)
			out << RED << "  server_name: " << RESET << *it << std::endl;
		std::map<short, std::string> error_pages = obj.getConfigFromMap(it->first)->getErrorPages();
		for (std::map<short, std::string>::iterator it = error_pages.begin(); it != error_pages.end(); it++)
			out << RED << "  error_pages: " << RESET << it->first << " " << it->second << std::endl;
		out << RED << "  listen: " << RESET << obj.getConfigFromMap(it->first)->getListen() << std::endl;
		std::vector<Location *> locations = obj.getConfigFromMap(it->first)->getLocations();
		for (std::vector<Location *>::iterator it = locations.begin(); it != locations.end(); it++)
		{
			out << YELLOW << "  location: " << RESET << (*it)->getLocation() << std::endl;
			out << YELLOW << "  {" << RESET << std::endl;
			out << CYAN << "    root: " << RESET << (*it)->getRoot() << std::endl;
			out << CYAN << "    index: " << RESET << (*it)->getIndex() << std::endl;
			out << CYAN << "    alias: " << RESET << (*it)->getAlias() << std::endl;
			out << CYAN << "    autoindex: " << RESET << (*it)->getAutoindex() << std::endl;
			out << CYAN << "    max client body size: " << RESET << (*it)->getMaxClientBody() << std::endl;
			std::vector<std::string> cgi_ext = (*it)->getCgiPass();
			for (std::vector<std::string>::iterator it = cgi_ext.begin(); it != cgi_ext.end(); it++)
				out << CYAN << "    cgi_pass: " << RESET << *it << std::endl;
			s_return ret = (*it)->getReturn();
			out << CYAN << "    return path: " << RESET << ret.path << std::endl;
			out << CYAN << "    return status: " << RESET << ret.status << std::endl;
			s_methods methods = (*it)->getAllowedMethods();
			out << CYAN << "    allowed methods get: " << RESET << methods.get << std::endl;
			out << CYAN << "    allowed methods post: " << RESET << methods.post << std::endl;
			out << CYAN << "    allowed methods remove: " << RESET << methods.remove << std::endl;
			out << YELLOW << "  }" << RESET << std::endl;
		}
		out << MAGENTA << "}" << RESET << std::endl;
	}
	return (out);
}*/