/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgosselk <lgosselk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 10:32:48 by sbelomet          #+#    #+#             */
/*   Updated: 2024/09/24 09:36:49 by lgosselk         ###   ########.fr       */
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

bool	isDirectory( std::string const &path )
{
	struct stat	info;

	if (stat(path.c_str(), &info) == 0 && (info.st_mode & S_IFDIR))
		return (true);
	return (false);
}

bool	isRegularFile( std::string const &path )
{
	struct stat	info;

	if (stat(path.c_str(), &info) == 0 && (info.st_mode & S_IFREG))
		return (true);
	return (false);
}

size_t	fileSize( int const fd)
{
	struct stat	info;

	fstat(fd, &info);
	return (info.st_size);	
}

size_t	fileSize( std::string const &file )
{
	struct stat	info;

	stat(file.c_str(), &info);
	return (info.st_size);
}

std::string	formatRoot( std::string root )
{
	if (root == "./")
		return (".");
	if (root[0] == '.' || root[0] == '/')
	{
		if (root[0] == '.' && root[1] == '/')
			root.erase(0, 2);
		else if (root[0] == '/')
			root.erase(0, 1);
	}
	if (root[(root.size() - 1)] == '/')
		root.erase((root.size() - 1), 1);
	root = "./" + root;
	return (root);
}

std::string	const concatenateRoot( std::string const &root,
	std::string const &path )
{
	std::string	newRoot = formatRoot(root);
	if (newRoot == ("." + path))
		return (newRoot);
	if (newRoot == ".")
		return (newRoot + path);
	if (path == "/")
		return (newRoot);
	if (path != "/")
		return (newRoot + path);
	return (newRoot);
}

/**
 * Extracts the extension of a file from a path, returns "default" if no extension is found
*/
std::string	extractPathExtension( std::string const &path )
{
	size_t	dot = path.find_last_of('.');
	if (dot != std::string::npos && dot + 1 < path.size())
		return path.substr(dot + 1);
	else
		return "default";
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