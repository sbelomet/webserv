/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbelomet <sbelomet@42lausanne.ch>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 10:16:09 by lgosselk          #+#    #+#             */
/*   Updated: 2024/09/25 14:33:24 by sbelomet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"

Config::Config( void ) : _root(std::string()), _host(std::string()), _index(std::string()),
	_listen(std::string()), _locationIndex(-1), _has_root_location(false), _client_max_body_size(1)
{}

Config::~Config( void )
{}

Config::Config( Config const &copy )
{
	*this = copy;
}

Config const	&Config::operator=( Config const &copy )
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

std::string const	&Config::getRoot( void ) const { return (_root); }
std::string const	&Config::getHost( void ) const { return (_host); }
std::string const	&Config::getIndex( void ) const { return (_index); }
std::string const	&Config::getListen( void ) const { return (_listen); }
bool const	&Config::getHasRootLocation( void ) const { return (_has_root_location); }
std::vector<Location> const	&Config::getLocations( void ) const { return (_locations); }
std::vector<Location>	&Config::getLocations( void ) { return (_locations); }
size_t const	&Config::getMaxClientBody( void ) const { return (_client_max_body_size); }
std::vector<std::string> const	&Config::getServerName( void ) const { return (_server_name); }
std::map<short, std::string> const	&Config::getErrorPages( void ) const { return (_error_pages); }

/*  */

void	Config::setRoot( std::string const &root ) { _root = root; }
void	Config::setHost( std::string const &host ) { _host = host; }
void	Config::setIndex( std::string const &index ) { _index = index; }
void	Config::setListen( std::string const &listen ) { _listen = listen; }
void	Config::setLocations( std::vector<Location> const &locations ) { _locations = locations; }
void	Config::setServerName( std::vector<std::string> const &serverName ) { _server_name = serverName; }
void	Config::setErrorPages( std::map<short, std::string> const &errorPages ) { _error_pages = errorPages; }
void	Config::setMaxClientBody( size_t const &maxClientBody ) { _client_max_body_size = maxClientBody; }

/*  */

void	Config::pushLocation( Location location ) { _locations.push_back(location); }
void	Config::pushServerName( std::string const &serverName ) { _server_name.push_back(serverName); }
void	Config::insertErrorPage( short const &num, std::string const &file ) { _error_pages[num] = file; }

/*  */

static	std::string const	getExtension( std::string const &word )
{
	size_t	pos = word.find_last_of('.');
	if (pos != std::string::npos)
		return (word.substr(pos));
	else
		return (std::string());
}

Location	Config::getSingleLocation( std::string const &path )
{
	std::string					extension;
	std::string					remainder("/");
	Location					defaultLocation;
	std::vector<std::string>	words = vecSplit(path, '/');

	if (words.size() > 0)
		extension = getExtension(words[(words.size() - 1)]);
	if (words.empty())
		words.push_back("");
	for (size_t i = 0; i < words.size(); i++)
	{
		if (i == 0)
			remainder += words[i];
		else
			remainder += "/" + words[i];
		for (std::vector<Location>::iterator it = _locations.begin(); it != _locations.end(); it++)
		{
			if (!extension.empty())
			{
				std::string const	locationExt = getExtension((*it).getLocation());
				if (extension == locationExt)
					return (*it);
			}
			if ((*it).getLocation() == "/")
				defaultLocation = *it;
			if ((*it).getLocation() == remainder)
				return (*it);
		}
	}
	return (defaultLocation);
}

void    Config::makeConfig( std::ifstream &infile, int &lineCount, bool awaitParenth )
{
	bool afterSepNW = true;
	bool afterVar = false;
	bool inLocation = false;
	std::string line;
	while (getline(infile, line)) // read line by line
	{
		lineCount++; // increment line count

		if (onlyWhitespaces(line)) // ignore blank lines
			continue;

		for (std::string::iterator it = line.begin(); it != line.end(); it++) // read each line character by character
		{
			std::string keyword;

			// ignore comments
			if (*it == '#')
				break;

			// ignore whitespaces
			if (isWhitespace(*it))
				continue;

			// if semicolon not directly after variable and their values, throw error
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
			else if (*it == '{')
			{
				std::cerr << "ERROR on line " << lineCount << ": Unexpected opening parenthesis" << std::endl;
				throw Webserv::NoException();
			}

			if (afterVar)
			{
				std::cerr << "ERROR on line " << lineCount << ": Variable declaration must be on one line and end with a semicolon" << std::endl;
				throw Webserv::NoException();
			}

			// if not on seperator, then we're on a keyword
			if (!isSeparator(*it))
			{
				std::string::iterator kw_end = findKeywordEnd(it, line.end());
				keyword = line.substr(it - line.begin(), (kw_end - line.begin()) - (it - line.begin()));
				if (afterSepNW)
				{
					afterSepNW = false;
					afterVar = true;
				}
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
				Location location;
				_locationIndex++;
				inLocation = true;
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
				std::string locationPath = line.substr(it - line.begin(), (loc_end - line.begin()) - (it - line.begin()));
				it = loc_end;

				// check if location value starts with a slash or a star
				if (locationPath[0] != '/' && locationPath[0] != '*')
				{
					std::cerr << "ERROR on line " << lineCount << ": Location value must start with a slash or a star" << std::endl;
					throw Webserv::NoException();
				}

				// set location value and push location to locations vector
				location.setLocation(locationPath);
				if (locationPath == "/")
					_has_root_location = true;
				_locations.push_back(location);

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
				lexingAfterVar(line, lineCount, keyword, it, inLocation, afterSepNW, afterVar);
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
					inLocation = false;
				}
			}

			// the for loop doesn't break if we're exactly at the end of the line, so we break manually
			if (it == line.end() || *it == '#')
				break;
		}
	}

	// if we're still in a scope at the end of the file, then we're missing a closing parenthesis
	std::cerr << "ERROR on line " << lineCount << ": Missing closing parenthesis" << std::endl;
	throw Webserv::NoException();
}

void	Config::lexingAfterVar( std::string const &line, int const &lineCount, std::string const &keyword,
								std::string::iterator &it, bool &inLocation, bool &afterSepNW, bool &afterVar )
{
	int nbvals = 0;
	std::vector<std::string> values;
	while (it != line.end() && *it && *it != ';')
	{
		if ((*it) == '#')
			break;
		
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
				parseKeyword(keyword, values, lineCount, inLocation);
				break;
			}
		}
		if (isSeparator(*it))
		{
			std::cerr << "ERROR on line " << lineCount << ": Unexpected separator" << std::endl;
			throw Webserv::NoException();
		}
		std::string::iterator val_end = findKeywordEnd(it, line.end());
		std::string value = line.substr(it - line.begin(), (val_end - line.begin()) - (it - line.begin()));
		it = val_end;
		nbvals++;
		values.push_back(value);
		if (*it == ';')
		{
			afterSepNW = true;
			afterVar = false;
			parseKeyword(keyword, values, lineCount, inLocation);
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

/* Parsing methods */

void	Config::parseKeyword( std::string const &keyword, std::vector<std::string> const &values,
	int const &lineCount, bool &inLocation )
{
	if (keyword == "root")
	{
		parseKeywordRoot(values, lineCount, inLocation);
	}
	else if (keyword == "host")
	{
		parseKeywordHost(values, lineCount, inLocation);
	}
	else if (keyword == "index")
	{
		parseKeywordIndex(values, lineCount, inLocation);
	}
	else if (keyword == "listen")
	{
		parseKeywordListen(values, lineCount, inLocation);
	}
	else if (keyword == "server_name")
	{
		parseKeywordServerName(values, lineCount, inLocation);
	}
	else if (keyword == "error_page")
	{
		parseKeywordErrorPage(values, lineCount, inLocation);
	}
	else if (keyword == "client_max_body_size")
	{
		parseKeywordClientMaxBodySize(values, lineCount, inLocation);
	}
	else if (keyword == "alias")
	{
		parseKeywordAlias(values, lineCount, inLocation);
	}
	else if (keyword == "return")
	{
		parseKeywordReturn(values, lineCount, inLocation);
	}
	else if (keyword == "autoindex")
	{
		parseKeywordAutoindex(values, lineCount, inLocation);
	}
	else if (keyword == "allow_methods")
	{
		parseKeywordAllowedMethods(values, lineCount, inLocation);
	}
	else if (keyword == "cgi_pass")
	{
		parseKeywordCgiPass(values, lineCount, inLocation);
	}
	else
	{
		std::cerr << "ERROR on line " << lineCount << ": Unknown variable name" << std::endl;
		throw Webserv::NoException();
	}
}

void	Config::parseKeywordRoot( std::vector<std::string> const &values, int const &lineCount, bool &inLocation )
{
	if ((_root != "" && !inLocation) || (inLocation && _locations[_locationIndex].getRoot() != ""))
	{
		std::cerr << "ERROR on line " << lineCount << ": Root variable already defined in scope" << std::endl;
		throw Webserv::NoException();
	}
	if (values.size() != 1)
	{
		std::cerr << "ERROR on line " << lineCount << ": Too many values for root variable" << std::endl;
		throw Webserv::NoException();
	}
	if (inLocation)
		_locations[_locationIndex].setRoot(values[0]);
	else
		setRoot(values[0]);
}

void	Config::parseKeywordHost( std::vector<std::string> const &values, int const &lineCount, bool &inLocation )
{
	if (inLocation)
	{
		std::cerr << "ERROR on line " << lineCount << ": Host variable not allowed in location scope" << std::endl;
		throw Webserv::NoException();
	}
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

void	Config::parseKeywordIndex( std::vector<std::string> const &values, int const &lineCount, bool &inLocation )
{
	if ((_index != "" && !inLocation) || (inLocation && _locations[_locationIndex].getIndex() != ""))
	{
		std::cerr << "ERROR on line " << lineCount << ": Index variable already defined in scope" << std::endl;
		throw Webserv::NoException();
	}
	if (values.size() != 1)
	{
		std::cerr << "ERROR on line " << lineCount << ": Too many values for index variable" << std::endl;
		throw Webserv::NoException();
	}
	if (values[0] != "index.html" && !inLocation)
	{
		std::cerr << "ERROR on line " << lineCount << ": Index should be index.html in server scope" << std::endl;
		throw Webserv::NoException();
	}
	if (inLocation)
		_locations[_locationIndex].setIndex(values[0]);
	else
		setIndex(values[0]);
}

void	Config::parseKeywordListen( std::vector<std::string> const &values, int const &lineCount, bool &inLocation )
{
	if (inLocation)
	{
		std::cerr << "ERROR on line " << lineCount << ": Listen variable not allowed in location scope" << std::endl;
		throw Webserv::NoException();
	}
	if (_listen != "")
	{
		std::cerr << "ERROR on line " << lineCount << ": Listen variable already defined" << std::endl;
		throw Webserv::NoException();
	}
	if (values.size() != 1)
	{
		std::cerr << "ERROR on line " << lineCount << ": Too many values for listen variable" << std::endl;
		throw Webserv::NoException();
	}
	if (values[0].find_first_not_of("0123456789") != std::string::npos || values[0].size() < 4)
	{
		std::cerr << "ERROR on line " << lineCount << ": Invalid port number" << std::endl;
		throw Webserv::NoException();
	}
	setListen(values[0]);
}

void	Config::parseKeywordServerName( std::vector<std::string> const &values, int const &lineCount,
	bool &inLocation )
{
	if (inLocation)
	{
		std::cerr << "ERROR on line " << lineCount << ": Server name variable not allowed in location scope" << std::endl;
		throw Webserv::NoException();
	}
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

void	Config::parseKeywordErrorPage( std::vector<std::string> const &values, int const &lineCount,
	bool &inLocation )
{
	if (inLocation)
	{
		std::cerr << "ERROR on line " << lineCount << ": Error page variable not allowed in location scope" << std::endl;
		throw Webserv::NoException();
	}
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
	if (values[1][0] != '/')
	{
		std::cerr << "ERROR on line " << lineCount << ": Error page must start with a slash" << std::endl;
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

void	Config::parseKeywordClientMaxBodySize( std::vector<std::string> const &values, int const &lineCount,
	bool &inLocation )
{
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
	size_t maxClientBody = std::strtoul(values[0].c_str(), &end, 10);
	if (errno == ERANGE || *end != '\0' || maxClientBody == ULONG_MAX)
	{
		std::cerr << "ERROR on line " << lineCount << ": Invalid client max body size" << std::endl;
		throw Webserv::NoException();
	}
	if (inLocation)
		_locations[_locationIndex].setMaxClientBody(maxClientBody);
	else
		setMaxClientBody(maxClientBody);
}

void	Config::parseKeywordAlias( std::vector<std::string> const &values, int const &lineCount,
	bool &inLocation )
{
	if (!inLocation)
	{
		std::cerr << "ERROR on line " << lineCount << ": Alias variable not allowed outside location scope" << std::endl;
		throw Webserv::NoException();
	}
	if (_locations[_locationIndex].getAlias() != "")
	{
		std::cerr << "ERROR on line " << lineCount << ": Alias variable already defined" << std::endl;
		throw Webserv::NoException();
	}
	if (values.size() != 1)
	{
		std::cerr << "ERROR on line " << lineCount << ": Too many values for alias variable" << std::endl;
		throw Webserv::NoException();
	}
	_locations[_locationIndex].setAlias(values[0]);
}

void	Config::parseKeywordReturn( std::vector<std::string> const &values, int const &lineCount, bool &inLocation )
{
	if (!inLocation)
	{
		std::cerr << "ERROR on line " << lineCount << ": Return variable not allowed outside location scope" << std::endl;
		throw Webserv::NoException();
	}
	if (_locations[_locationIndex].getReturn() != "")
	{
		std::cerr << "ERROR on line " << lineCount << ": Return variable already defined" << std::endl;
		throw Webserv::NoException();
	}
	if (values.size() != 1)
	{
		std::cerr << "ERROR on line " << lineCount << ": Too many values for return variable" << std::endl;
		throw Webserv::NoException();
	}
	_locations[_locationIndex].setReturn(values[0]);
}

void	Config::parseKeywordAutoindex( std::vector<std::string> const &values, int const &lineCount, bool &inLocation )
{
	if (!inLocation)
	{
		std::cerr << "ERROR on line " << lineCount << ": Autoindex variable not allowed outside location scope" << std::endl;
		throw Webserv::NoException();
	}
	if (_locations[_locationIndex].getAutoindexSet())
	{
		std::cerr << "ERROR on line " << lineCount << ": Autoindex variable already defined" << std::endl;
		throw Webserv::NoException();
	}
	if (values.size() != 1)
	{
		std::cerr << "ERROR on line " << lineCount << ": Too many values for autoindex variable" << std::endl;
		throw Webserv::NoException();
	}
	if (values[0] != "on" && values[0] != "off")
	{
		std::cerr << "ERROR on line " << lineCount << ": Invalid value for autoindex variable" << std::endl;
		throw Webserv::NoException();
	}
	_locations[_locationIndex].setAutoindexSet(true);
	if (values[0] == "on")
		_locations[_locationIndex].setAutoindex(true);
}

void	Config::parseKeywordAllowedMethods( std::vector<std::string> const &values, int const &lineCount,
	bool &inLocation )
{
	if (!inLocation)
	{
		std::cerr << "ERROR on line " << lineCount << ": Allowed methods variable not allowed outside location scope" << std::endl;
		throw Webserv::NoException();
	}
	if (_locations[_locationIndex].getGet() || _locations[_locationIndex].getPost() || _locations[_locationIndex].getRemove())
	{
		std::cerr << "ERROR on line " << lineCount << ": Allowed methods variable already defined" << std::endl;
		throw Webserv::NoException();
	}
	for (size_t i = 0; i < values.size(); i++)
	{
		if (values[i] != "GET" && values[i] != "POST" && values[i] != "DELETE")
		{
			std::cerr << "ERROR on line " << lineCount << ": Invalid value for allowed methods variable" << std::endl;
			throw Webserv::NoException();
		}
		if (std::count(values.begin(), values.end(), values[i]) > 1)
		{
			std::cerr << "ERROR on line " << lineCount << ": Duplicate allowed methods forbidden" << std::endl;
			throw Webserv::NoException();
		}
		if (values[i] == "GET")
			_locations[_locationIndex].setGet(true);
		if (values[i] == "POST")
			_locations[_locationIndex].setPost(true);
		if (values[i] == "DELETE")
			_locations[_locationIndex].setRemove(true);
	}
}

void	Config::parseKeywordCgiPass( std::vector<std::string> const &values, int const &lineCount, bool &inLocation )
{
	if (!inLocation)
	{
		std::cerr << "ERROR on line " << lineCount << ": CGI pass variable not allowed outside location scope" << std::endl;
		throw Webserv::NoException();
	}
	if (!_locations[_locationIndex].getCgiPass().empty())
	{
		std::cerr << "ERROR on line " << lineCount << ": CGI pass variable already defined" << std::endl;
		throw Webserv::NoException();
	}
	for (size_t i = 0; i < values.size(); i++)
	{
		if (std::count(_locations[_locationIndex].getCgiPass().begin(), _locations[_locationIndex].getCgiPass().end(), values[i]))
		{
			std::cerr << "ERROR on line " << lineCount << ": Duplicate CGI pass values forbidden" << std::endl;
			throw Webserv::NoException();
		}
		if(values[i][0] != '/')
		{
			std::cerr << "ERROR on line " << lineCount << ": CGI pass value must start with a slash" << std::endl;
			throw Webserv::NoException();
		}
		_locations[_locationIndex].pushCgiPass(values[i]);
	}
}

std::ostream &operator<<(std::ostream &out, Config &obj)
{
	out << "Root: " << obj.getRoot() << std::endl;
	out << "Host: " << obj.getHost() << std::endl;
	out << "Index: " << obj.getIndex() << std::endl;
	out << "Listen: " << obj.getListen() << std::endl;
	out << "Server name: ";
	for (size_t i = 0; i < obj.getServerName().size(); i++)
	{
		out << obj.getServerName()[i];
		if (i < obj.getServerName().size() - 1)
			out << ", ";
	}
	out << std::endl;
	out << "Error pages: ";
	for (std::map<short, std::string>::const_iterator it = obj.getErrorPages().begin(); it != obj.getErrorPages().end(); it++)
	{
		out << it->first << " -> " << it->second;
		if (it != --obj.getErrorPages().end())
			out << ", ";
	}
	out << std::endl;
	out << "Max client body size: " << obj.getMaxClientBody() << std::endl;
	out << "Locations: " << std::endl;
	std::vector<Location> locations = obj.getLocations();
	for (size_t i = 0; i < locations.size(); i++)
	{
		out << locations[i];
	}
	return (out);
}