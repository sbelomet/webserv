/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MapConfig.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbelomet <sbelomet@42lausanne.ch>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 10:28:01 by lgosselk          #+#    #+#             */
/*   Updated: 2024/09/13 10:04:45 by sbelomet         ###   ########.fr       */
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

void	MapConfig::makeAll( Server &server, std::string const &filepath )
{
	mappingConfigs(filepath);
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

/**
 * Parse the configuration file to create the server objects, further parsing is done in Config::makeConfig
 */
void	MapConfig::mappingConfigs( std::string const &filepath )
{
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
				checkValidConfig(config);
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
}

/**
 * Check if the configuration is valid
 */
void	MapConfig::checkValidConfig( Config *config )
{
	if (config->getRoot().empty())
	{
		std::cerr << "ERROR: root value is missing" << std::endl;
		throw Webserv::NoException();
	}
	if (config->getHost().empty())
	{
		std::cerr << "ERROR: host value is missing" << std::endl;
		throw Webserv::NoException();
	}
	if (config->getIndex().empty())
	{
		std::cerr << "ERROR: index value is missing" << std::endl;
		throw Webserv::NoException();
	}
	if (config->getListen().empty())
	{
		std::cerr << "ERROR: listen value is missing" << std::endl;
		throw Webserv::NoException();
	}
	if (config->getLocations().empty())
	{
		std::cerr << "ERROR: root location is missing" << std::endl;
		throw Webserv::NoException();
	}
	if (config->getHasRootLocation() == false)
	{
		std::cerr << "ERROR: root location is missing" << std::endl;
		throw Webserv::NoException();
	}
	fillLocations(config);
}

/**
 * Fill in missing values in the location block if possible
 */
void	MapConfig::fillLocations( Config *config )
{
	std::vector<Location *> locations = config->getLocations();
	Location *rootLocation = NULL;

	// find the root location and fill in missing values
	for (std::vector<Location *>::iterator it = locations.begin(); it != locations.end(); it++)
	{
		if ((*it)->getLocation() == "/")
		{
			rootLocation = *it;
			if (rootLocation->getMaxClientBody() == 1)
				rootLocation->setMaxClientBody(config->getMaxClientBody());
			if (rootLocation->getRoot().empty())
				rootLocation->setRoot(config->getRoot());
			if (rootLocation->getIndex().empty())
				rootLocation->setIndex(config->getIndex());
			break;
		}
	}

	// fill in missing values in the other locations
	for (std::vector<Location *>::iterator it = locations.begin(); it != locations.end(); it++)
	{
		if ((*it)->getLocation() == "/")
			continue;
		if ((*it)->getRoot().empty())
		{
			if (!rootLocation->getRoot().empty())
				(*it)->setRoot(rootLocation->getRoot());
		}
		if ((*it)->getIndex().empty())
		{
			if (!rootLocation->getIndex().empty())
				(*it)->setIndex(rootLocation->getIndex());
		}
		if ((*it)->getAlias().empty())
		{
			if (!rootLocation->getAlias().empty())
				(*it)->setAlias(rootLocation->getAlias());
		}
		if ((*it)->getAutoindexSet() == false)
		{
			if (rootLocation->getAutoindexSet() == true)
				(*it)->setAutoindex(rootLocation->getAutoindex());
		}
		if ((*it)->getCgiPass().empty())
		{
			if (!rootLocation->getCgiPass().empty())
				(*it)->setCgiPass(rootLocation->getCgiPass());
		}
		if ((*it)->getReturn().path.empty())
		{
			if (!rootLocation->getReturn().path.empty())
				(*it)->setReturn(rootLocation->getReturn());
		}
		if ((*it)->getAllowedMethods().var_set == false)
		{
			if (rootLocation->getAllowedMethods().var_set == true)
				(*it)->setAllowedMethods(rootLocation->getAllowedMethods());
		}
		if ((*it)->getMaxClientBody() == 1)
		{
			if (rootLocation->getMaxClientBody() != 1)
				(*it)->setMaxClientBody(rootLocation->getMaxClientBody());
		}
	}
}

std::ostream &operator<<(std::ostream &out, MapConfig &obj)
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
}