/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbelomet <sbelomet@42lausanne.ch>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 10:32:48 by sbelomet          #+#    #+#             */
/*   Updated: 2024/08/26 13:54:07 by sbelomet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"


bool isWhitespace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\0');
}

bool isSeparatorNW(char c)
{
	return (c == '{' || c == '}' || c == ';' || c == '#');
}

bool isSeparator(char c)
{
	return (isWhitespace(c) || isSeparatorNW(c));
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

std::string::iterator findKeywordEnd(std::string::iterator it, std::string::const_iterator end)
{
	while (it != end && !isSeparator(*it))
		it++;
	return it;
}
