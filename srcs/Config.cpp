/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbelomet <sbelomet@42lausanne.ch>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 10:16:09 by lgosselk          #+#    #+#             */
/*   Updated: 2024/08/20 11:55:43 by sbelomet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"
Config::Config( void )
{}

Config::~Config( void )
{
	// delete if malloced
}

Config::Config( Config const &copy )
{
	*this = copy;
}

Config const    &Config::operator=( Config const &copy )
{
	if (this != &copy)
		this->setConfig(copy.getConfig());
	return (*this);
}

/* */

char const	*Config::FileException::what() const throw()
{
	return ("Error: could not open file.");
}

/* */

std::map<std::string, Section>  &Config::getConfig( void )
{
	return (this->_config);
}

void    Config::setConfig( std::map<std::string, Section> const &config )
{
	this->_config = config;
}

std::map<std::string, Section> const    &Config::getConfig( void ) const
{
	return (this->_config);
}

void    Config::insertSection( std::string const &key, Section const &section )
{
	this->_config[key] = section;
}

Section &Config::getSectionFromMap( std::string const &key )
{
	return (this->_config[key]);
}

/* */
/*
static std::string const    trim( std::string line )
{
	size_t  end = 0;
	size_t  start = 0;
	std::string::iterator   it;
	for (it = line.begin(); it != line.end(); it++)
	{
		if (isspace(*it))
			start++;
		else
			break ;
	}
	for (it = line.end(); it != line.begin(); it--)
	{
		if (isspace(*it))
			end++;
		else
			break ;
	}
	return (line.substr(start, (end - (line.size() - 1))));
}
static bool   check_header( std::string const value )
{
	for (size_t i = 0; i < value.size(); i++)
	{
		if (value[i] == '{')
			return (true);
	}
	return (false);
}

static bool   check_closing( std::string const key )
{
	for (size_t i = 0; i < key.size(); i++)
	{
		if (key[i] == '}')
			return (true);
	}
	return (false);
} */

static bool isWhitespace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n' || c == '\r');
}

static bool isSeparatorNW(char c)
{
	return (c == '{' || c == '}' || c == ';' || c == '\0' || c == '#');
}

static bool isSeparator(char c)
{
	return (isWhitespace(c) || isSeparatorNW(c));
}

static bool onlyWhitespaces(const std::string &str)
{
	for (size_t i = 0; i < str.size(); i++)
	{
		if (!isWhitespace(str[i]))
			return false;
	}
	return true;
}

static std::string::iterator findKeywordEnd(std::string::iterator it, std::string::iterator end)
{
	while (it != end && !isSeparator(*it))
		it++;
	return it;
}

void    Config::makeConfig( std::string const &filePath )
{
	std::string     curr_section;
	std::ifstream   infile(filePath.c_str());
	if (!infile.is_open())
		throw (FileException());

	bool inServer = false;
	bool inLocation = false;
	bool awaitingParenth = false;
	bool afterSepNW = false;
	std::string line;
	size_t lineCount = 0;
	while (getline(infile, line))
	{
		lineCount++; // increment line count

		if (onlyWhitespaces(line)) // ignore blank lines
			continue;

		for (std::string::iterator it = line.begin(); it != line.end(); it++)
		{
			std::string::iterator kw_end;
			std::string keyword;

			// ignore comments
			if (*it == '#')
				break;

			// ignore whitespaces
			if (isWhitespace(*it))
				continue;

			if (*it == ';')
			{
				afterSepNW = true;
				std::cout << "SEMICOLON" << std::endl;
			}

			// if awaiting parenthesis, check for the parenthesis
			if (awaitingParenth)
			{
				if (*it == '{')
				{
					awaitingParenth = false;
					afterSepNW = true;
				}
				else if (isSeparator(*it))
				{
					continue;
				}
				else
				{
					std::cerr << "ERROR on line " << lineCount << ": Missing parenthesis after scope keyword" << std::endl;
					return ;
				}
			}
			// if not on seperator, then we're on a keyword
			if (!isSeparator(*it))
			{
				kw_end = findKeywordEnd(it, line.end());
				keyword = line.substr(it - line.begin(), (kw_end - line.begin()) - (it - line.begin()));

				std::string color = MAGENTA;
				std::string indent = "";
				if (inServer)
				{
					color = CYAN;
					indent = "  ";
				}
				if (inLocation)
				{
					indent = "    ";
				}
				if (afterSepNW && inServer)
				{
					color = RED;
					afterSepNW = false;
				}
				std::cout << color << indent << keyword << RESET << std::endl;
				it = kw_end;
				if (isSeparatorNW(*it))
				{
					afterSepNW = true;
				}
			}

			// if keyword is server, then check for parenthesis and await one if not found (also set inServer to true)
			if (keyword == "server")
			{
				inServer = true;
				while (it != line.end() && *it && isSeparator(*it) && *it != '{')
				{
					it++;
				}
				if (it == line.end())
				{
					awaitingParenth = true;
					break;
				}
				if (*it != '{')
				{
					std::cerr << "ERROR on line " << lineCount << ": Missing parenthesis after server keyword" << std::endl;
					return ;
				}
			}

			// if we're not in server scope, then no declaration should be made
			if (!inServer)
			{
				std::cerr << "ERROR on line " << lineCount << ": Declaration outside of server scope" << std::endl;
				return ;
			}

			// if keyword is location, check for location value and parenthesis (also set inLocation to true)
			if (keyword == "location")
			{
				inLocation = true;
				if (*it != ' ' || isSeparator(*(it + 1)))
				{
					std::cerr << "ERROR on line " << lineCount << ": Invalid location value" << std::endl;
					return ;
				}
				it++;
				std::string::iterator loc_end = findKeywordEnd(it, line.end());
				std::string location = line.substr(it - line.begin(), (loc_end - line.begin()) - (it - line.begin()));
				std::cout << GREEN << "  " << location << RESET << std::endl;
				it = loc_end;
				while (it != line.end() && *it && isSeparator(*it) && *it != '{')
				{
					it++;
				}
				if (it == line.end())
				{
					awaitingParenth = true;
					break;
				}
				if (*it != '{')
				{
					std::cerr << "ERROR on line " << lineCount << ": Missing parenthesis after loction keyword" << std::endl;
					return ;
				}
				if (*it == '{')
				{
					afterSepNW = true;
				}
			}

			// if we encounter a closing parenthesis...
			if (*it == '}')
			{
				if (!inLocation) // ...while in server scope, inServer should be set to false
				{
					inServer = false;
				}
				if (inLocation) // ...while in location scope, inLocation should be set to false
				{
					inLocation = false;
				}
			}

			if (it == line.end())
				break;
		}
	}

	// if we're still in a scope at the end of the file, then we're missing a closing parenthesis
	if (inServer || inLocation)
	{
		std::cerr << "ERROR on line " << lineCount << ": Missing closing parenthesis" << std::endl;
		return ;
	}
}
