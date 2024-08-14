/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbelomet <sbelomet@42lausanne.ch>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 10:11:54 by sbelomet          #+#    #+#             */
/*   Updated: 2024/08/14 13:36:29 by sbelomet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"
#include <fstream>
#include <sstream>
#include <algorithm>

bool isWhitespace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n' || c == '\r');
}

bool isSeparator(char c)
{
	return (isWhitespace(c) || c == '{' || c == '}' || c == ';' || c == '\0' || c == '#');
}

bool onlyWhitespaces(const std::string &str)
{
	for (size_t i = 0; i < str.size(); i++)
	{
		if (!isWhitespace(str[i]))
			return false;
	}
	return true;
}

std::string::iterator findKeywordEnd(std::string::iterator it, std::string::iterator end)
{
	while (it != end && !isSeparator(*it))
		it++;
	return it;
}

int main(int ac, char **av)
{
	if (ac != 2)
		return 1;
	
	std::ifstream file(av[1]);
	if (!file.is_open())
	{
		std::cerr << "dumbass not a good file" << std::endl;
		return 1;
	}

	bool afterSep = false;
	std::string line;
	while (getline(file, line))
	{
		if (onlyWhitespaces(line))
			continue;

		for (std::string::iterator it = line.begin(); it != line.end(); it++)
		{
			if (*it == '#') // ignore comments
				break;

			if (!isSeparator(*it)) // if not on seperator, then we're on a keyword
			{
				std::string::iterator kw_end = findKeywordEnd(it, line.end());
				std::cout << "start: " << it - line.begin() << ", len: " << (kw_end - line.begin()) - (it - line.begin()) << std::endl;
				std::string keyword = line.substr(it - line.begin(), (kw_end - line.begin()) - (it - line.begin()));
				std::cout << RED << keyword << RESET << std::endl;
				it = kw_end;
			}
		}
		

//		std::cout << line << std::endl;
	}
}