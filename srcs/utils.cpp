/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbelomet <sbelomet@42lausanne.ch>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 10:32:48 by sbelomet          #+#    #+#             */
/*   Updated: 2024/09/11 13:51:34 by sbelomet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"

/**
 * Check if a character is a whitespace (space, tab, newline, carriage return, null)
 */
bool isWhitespace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\0');
}

/**
 * Check if a character is a non-whitespace separator ('{', '}', ';', '#')
 */
bool isSeparatorNW(char c)
{
	return (c == '{' || c == '}' || c == ';' || c == '#');
}

/**
 * Check if a character is a separator (whitespace or non-whitespace)
 */
bool isSeparator(char c)
{
	return (isWhitespace(c) || isSeparatorNW(c));
}

/**
 * Check if a string is only made of whitespaces
 */
bool onlyWhitespaces(const std::string &str)
{
	for (size_t i = 0; i < str.size(); i++)
	{
		if (!isWhitespace(str[i]))
			return false;
	}
	return true;
}

/**
 * Return an iterator to the end of a keyword in a string
 */
std::string::iterator findKeywordEnd(std::string::iterator it, std::string::iterator end)
{
	while (it != end && !isSeparator(*it))
		it++;
	return it;
}

/**
 * Return an iterator to the end of a keyword in a string
 */
std::string::iterator findKeywordEnd(std::string::iterator it, std::string::const_iterator end)
{
	while (it != end && !isSeparator(*it))
		it++;
	return it;
}

/**
 * Split a string into a vector of strings using a delimiter
 */
std::vector<std::string>	vecSplit( std::string const &str, char delimiter )
{
	std::string					word;
	std::vector<std::string>	words;
	std::stringstream			ss(str);

	while (!ss.eof())
	{
		std::getline(ss, word, delimiter);
		if (!word.empty())
			words.push_back(word);
	}
	return (words);
}

static std::string const	checkAbsolute( std::string const &path )
{
	if (path[0] == '.' && path[1] == '/')
		return (path);
	std::string const newPath = "./" + path;
	return (newPath);
}

bool	isDirectory( std::string path )
{
	struct stat	info;

	path = checkAbsolute(path);
	if (stat(path.c_str(), &info) != 0 || !(info.st_mode & S_IFDIR))
		return (false);
	return (true);
}

bool	isRegularFile( std::string path )
{
	struct stat	info;

	path = checkAbsolute(path);
	if (stat(path.c_str(), &info) != 0 || !(info.st_mode & S_IFREG))
		return (false);
	return (true);
}

/**
 * Convert a string to lowercase (check if it's a letter before converting)
 */
void	strtolower(std::string &str)
{
	for (size_t i = 0; i < str.size(); i++)
	{
		if (str[i] >= 'A' && str[i] <= 'Z')
			str[i] = std::tolower(str[i]);
	}
}