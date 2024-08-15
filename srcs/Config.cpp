/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgosselk <lgosselk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 10:16:09 by lgosselk          #+#    #+#             */
/*   Updated: 2024/08/15 11:04:13 by lgosselk         ###   ########.fr       */
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
}

void    Config::makeConfig( std::string const &filePath )
{
    std::string     curr_section;
    std::ifstream   infile(filePath);
    if (!infile.is_open())
        throw (FileException());
    std::string line;
    while (std::getline(infile, line))
    {
        if (line.empty() || line[0] == '\n')
            continue ;
        line = trim(line);
        std::stringstream   ss(line);
        std::string key;
        std::string value;
        std::getline(ss, key, ' ');
        std::getline(ss, value, '\n');
        if (key.empty() || value.empty() || check_closing(key))
            continue ; // throw exception
        if (check_header(value) && key != curr_section)
        {
            Section section;
            curr_section = key;
            this->insertSection(curr_section, section);
        }
        else
            getSectionFromMap(curr_section).insertData(key, value);
    }
}
