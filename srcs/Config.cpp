/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbelomet <sbelomet@42lausanne.ch>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 10:16:09 by lgosselk          #+#    #+#             */
/*   Updated: 2024/08/22 13:57:56 by sbelomet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"

Config::Config( void ) : _root(""), _host(""), _index(""), _client_max_body_size(-1)
{}

Config::~Config( void )
{
	std::vector<Location *> locations = getLocations();
	for (size_t i = 0; i < locations.size(); i++)
	{
		delete locations[i];
	}
}

Config::Config( Config const &copy )
{
	*this = copy;
}

Config const    &Config::operator=( Config const &copy )
{
	if (this != &copy)
	{
		setRoot(copy.getRoot());
		setHost(copy.getHost());
		setIndex(copy.getIndex());
		setListen(copy.getListen());
		setLocations(copy.getLocations());
		setServerName(copy.getServerName());
		setErrorPages(copy.getErrorPages());
		setMaxClientBody(copy.getMaxClientBody());
	}
	return (*this);
}

/* */

std::string const	&Config::getRoot( void ) const
{
	return (_root);
}

std::string const	&Config::getHost( void ) const
{
	return (_host);
}

std::string const	&Config::getIndex( void ) const
{
	return (_index);
}

unsigned long const	&Config::getMaxClientBody( void ) const
{
	return (_client_max_body_size);
}

std::vector<Location *> const	&Config::getLocations( void ) const
{
	return (_locations);
}

std::vector<std::string> const	&Config::getListen( void ) const
{
	return (_listen);
}

std::vector<std::string> const	&Config::getServerName( void ) const
{
	return (_server_name);
}

std::map<short, std::string> const	&Config::getErrorPages( void ) const
{
	return (_error_pages);
}

/*  */

void	Config::setRoot( std::string const &root )
{
	_root = root;
}

void	Config::setHost( std::string const &host )
{
	_host = host;
}

void	Config::setIndex( std::string const &index )
{
	_index = index;
}

void	Config::setListen( std::vector<std::string> const &listen )
{
	_listen = listen;
}

void	Config::setMaxClientBody( unsigned long const &maxClientBody )
{
	_client_max_body_size = maxClientBody;
}

void	Config::setLocations( std::vector<Location *> const &locations )
{
	_locations = locations;
}

void	Config::setServerName( std::vector<std::string> const &serverName )
{
	_server_name = serverName;
}

void	Config::setErrorPages( std::map<short, std::string> const &errorPages )
{
	_error_pages = errorPages;
}

/*  */

void	Config::pushListen( std::string const &listen )
{
	_listen.push_back(listen);
}

void	Config::pushLocation( Location *&location )
{
	_locations.push_back(location);
}

void	Config::pushServerName( std::string const &serverName )
{
	_server_name.push_back(serverName);
}

void	Config::insertErrorPage( short const &num, std::string const &file )
{
	_error_pages[num] = file;
}

/*  */

void    Config::makeConfig( std::ifstream &infile, int &lineCount, bool awaitParenth )
{
	Location *inLocation = NULL;
	bool afterSepNW = true;
	bool afterVar = false;
	std::string line;
	while (getline(infile, line)) // read line by line
	{
		lineCount++; // increment line count

		if (onlyWhitespaces(line)) // ignore blank lines
			continue;

		for (std::string::iterator it = line.begin(); it != line.end(); it++) // read each line character by character
		{
			std::string::iterator kw_end;
			std::string keyword;
			std::string indent = "  ";

			// ignore comments
			if (*it == '#')
				break;

			// ignore whitespaces
			if (isWhitespace(*it))
				continue;

			if (*it == ';')
			{
				std::cerr << "ERROR on line " << lineCount << ": Unexpected semicolon" << std::endl;
				throw Webserv::NoException();
			}

			// if awaiting parenthesis, check for the parenthesis
			if (awaitParenth)
			{
				if (*it == '{')
				{
					awaitParenth = false;
					afterSepNW = true;
					afterVar = false;
				}
				else
				{
					std::cerr << "ERROR on line " << lineCount << ": Missing or obstructed opening parenthesis after scope keyword" << std::endl;
					throw Webserv::NoException();
				}
			}

			// if not on seperator, then we're on a keyword
			if (!isSeparator(*it))
			{
				kw_end = findKeywordEnd(it, line.end());
				keyword = line.substr(it - line.begin(), (kw_end - line.begin()) - (it - line.begin()));

				std::string color = CYAN;
				if (inLocation)
				{
					indent = "    ";
					if (afterSepNW)
					{
						color = MAGENTA;
						afterSepNW = false;
						afterVar = true;
					}
				}
				if (afterSepNW)
				{
					color = RED;
					afterSepNW = false;
					afterVar = true;
				}
				std::cout << color << indent << keyword << RESET << std::endl;

				it = kw_end;
			}

			// can't declare server scope in server scope
			if (keyword == "server")
			{
				std::cerr << "ERROR on line " << lineCount << ": Can't declare server scope in server scope" << std::endl;
				throw Webserv::NoException();
			}

			// if keyword is location, check for location value and parenthesis (also set inLocation to true)
			if (keyword == "location")
			{
				inLocation = new Location();
				while (it != line.end() && isWhitespace(*it))	// skip whitespaces
				{
					it++;
				}
				if (it == line.end() || isSeparator(*it))	// if no value directly after, throw error
				{
					std::cerr << "ERROR on line " << lineCount << ": Missing or obstructed location value" << std::endl;
					throw Webserv::NoException();
				}
				std::string::iterator loc_end = findKeywordEnd(it, line.end());
				std::string location = line.substr(it - line.begin(), (loc_end - line.begin()) - (it - line.begin()));
				std::cout << GREEN << indent << location << RESET << std::endl;
				it = loc_end;

				inLocation->setLocation(location);
				_locations.push_back(inLocation);

				// check for opening parenthesis, if non-whitespace characters are between location value and parenthesis, throw error
				while (it != line.end() && *it && isWhitespace(*it) && *it != '{')
				{
					it++;
				}
				if (it == line.end())
				{
					awaitParenth = true;
					break;
				}
				if (*it != '{')
				{
					std::cerr << "ERROR on line " << lineCount << ": Missing or obstructed opening parenthesis after loction keyword" << std::endl;
					throw Webserv::NoException();
				}
				if (*it == '{')
				{
					afterSepNW = true;
					afterVar = false;
				}
			}

			// if we're after a variable, get all values until semicolon
			if (afterVar)
			{
				int nbvals = 0;
				std::vector<std::string> values;
				while (it != line.end() && *it && *it != ';')
				{
					while (it != line.end() && *it && isWhitespace(*it))
					{
						it++;
					}
					if (it == line.end())
						break;
					if (*it == ';')
					{
						if (nbvals == 0) // if no value is assigned, throw error
						{
							std::cerr << "ERROR on line " << lineCount << ": Assigning no value is forbidden" << std::endl;
							throw Webserv::NoException();
						}
						else
						{
							afterSepNW = true;
							afterVar = false;
							parseKeyword(keyword, values, lineCount);
							break;
						}
					}
					std::string::iterator val_end = findKeywordEnd(it, line.end());
					std::string value = line.substr(it - line.begin(), (val_end - line.begin()) - (it - line.begin()));
					std::cout << YELLOW << indent << value << RESET << std::endl;
					it = val_end;
					nbvals++;
					values.push_back(value);
					if (*it == ';')
					{
						afterSepNW = true;
						afterVar = false;
						parseKeyword(keyword, values, lineCount);
					}
				}
				if (it == line.end())
				{
					std::cerr << "ERROR on line " << lineCount << ": Definition of variable not on one line" << std::endl;
					throw Webserv::NoException();
				}
				if (!nbvals && *it == ';')
				{
					std::cerr << "ERROR on line " << lineCount << ": Assigning no value is forbidden" << std::endl;
					throw Webserv::NoException();
				}
			}

			// if we encounter a closing parenthesis...
			if (*it == '}')
			{
				if (!inLocation) // ...while in server scope, we return
				{
					return ;
				}
				if (inLocation) // ...while in location scope, inLocation should be set to false
				{
					inLocation = NULL;
				}
			}

			// the for loop doesn't break if we're exactly at the end of the line, so we break manually
			if (it == line.end())
				break;
		}
	}

	// if we're still in a scope at the end of the file, then we're missing a closing parenthesis
	std::cerr << "ERROR on line " << lineCount << ": Missing closing parenthesis" << std::endl;
	throw Webserv::NoException();
}

void	Config::parseKeyword( std::string const &keyword, std::vector<std::string> const &values, int const &lineCount )
{
	if (keyword == "root")
	{
		if (_root != "")
		{
			std::cerr << "ERROR on line " << lineCount << ": Root variable already defined" << std::endl;
			throw Webserv::NoException();
		}
		if (values.size() != 1)
		{
			std::cerr << "ERROR on line " << lineCount << ": Too many values for root variable" << std::endl;
			throw Webserv::NoException();
		}
		setRoot(values[0]);
	}
	else if (keyword == "host")
	{
		if (_host != "")
		{
			std::cerr << "ERROR on line " << lineCount << ": Host variable already defined" << std::endl;
			throw Webserv::NoException();
		}
		if (values.size() != 1)
		{
			std::cerr << "ERROR on line " << lineCount << ": Too many values for host variable" << std::endl;
			throw Webserv::NoException();
		}
		setHost(values[0]);
	}
	else if (keyword == "index")
	{
		if (_index != "")
		{
			std::cerr << "ERROR on line " << lineCount << ": Index variable already defined" << std::endl;
			throw Webserv::NoException();
		}
		if (values.size() != 1)
		{
			std::cerr << "ERROR on line " << lineCount << ": Too many values for index variable" << std::endl;
			throw Webserv::NoException();
		}
		if (values[0] != "index.html")
		{
			std::cerr << "ERROR on line " << lineCount << ": Index should be index.html" << std::endl;
			throw Webserv::NoException();
		}
		setIndex(values[0]);
	}
	else if (keyword == "listen")
	{
		if (values.size() != 1)
		{
			std::cerr << "ERROR on line " << lineCount << ": Too many values for listen variable, use multiple lines when defining multiple ports" << std::endl;
			throw Webserv::NoException();
		}
		if (values[0].find_first_not_of("0123456789") != std::string::npos || values[0].size() < 4)
		{
			std::cerr << "ERROR on line " << lineCount << ": Invalid port number" << std::endl;
			throw Webserv::NoException();
		}
		if (std::count(_listen.begin(), _listen.end(), values[0]))
		{
			std::cerr << "ERROR on line " << lineCount << ": Duplicate ports forbidden" << std::endl;
			throw Webserv::NoException();
		}
		pushListen(values[0]);
	}
	else if (keyword == "location")
	{
		std::cout << "location keyword found" << std::endl;
	}
	else if (keyword == "server_name")
	{
		if (!_server_name.empty())
		{
			std::cerr << "ERROR on line " << lineCount << ": Server name variable already defined, use one line when defining multiple server names" << std::endl;
			throw Webserv::NoException();
		}
		for (size_t i = 0; i < values.size(); i++)
		{
			if (std::count(_server_name.begin(), _server_name.end(), values[i]))
			{
				std::cerr << "ERROR on line " << lineCount << ": Duplicate server names forbidden" << std::endl;
				throw Webserv::NoException();
			}
			pushServerName(values[i]);
		}
	}
	else if (keyword == "error_page")
	{
		if (values.size() != 2)
		{
			std::cerr << "ERROR on line " << lineCount << ": Values must be one error number and one error page, use multiple lines when defining multiple error pages" << std::endl;
			throw Webserv::NoException();
		}
		if (values[0].find_first_not_of("0123456789") != std::string::npos || values[0].size() != 3)
		{
			std::cerr << "ERROR on line " << lineCount << ": Invalid error number" << std::endl;
			throw Webserv::NoException();
		}
		short errorNum = std::atoi(values[0].c_str());
		if (_error_pages.find(errorNum) != _error_pages.end())
		{
			std::cerr << "ERROR on line " << lineCount << ": Duplicate error numbers forbidden" << std::endl;
			throw Webserv::NoException();
		}
		insertErrorPage(errorNum, values[1]);
	}
	else if (keyword == "client_max_body_size")
	{
		if (_client_max_body_size != static_cast<unsigned long>(-1))
		{
			std::cerr << "ERROR on line " << lineCount << ": Client max body size variable already defined" << std::endl;
			throw Webserv::NoException();
		}
		if (values.size() != 1)
		{
			std::cerr << "ERROR on line " << lineCount << ": Too many values for client max body size variable" << std::endl;
			throw Webserv::NoException();
		}
		if (values[0].find_first_not_of("0123456789") != std::string::npos)
		{
			std::cerr << "ERROR on line " << lineCount << ": Invalid client max body size" << std::endl;
			throw Webserv::NoException();
		}
		char *end;
		errno = 0;
		unsigned long maxClientBody = std::strtoul(values[0].c_str(), &end, 10);
		if (errno == ERANGE || *end != '\0' || maxClientBody == ULONG_MAX)
		{
			std::cerr << "ERROR on line " << lineCount << ": Invalid client max body size" << std::endl;
			throw Webserv::NoException();
		}
		setMaxClientBody(maxClientBody);
	}
	else
	{
		std::cerr << "ERROR on line " << lineCount << ": Unknown variable name" << std::endl;
		throw Webserv::NoException();
	}
}