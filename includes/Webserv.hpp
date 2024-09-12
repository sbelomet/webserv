/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbelomet <sbelomet@42lausanne.ch>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 10:12:17 by sbelomet          #+#    #+#             */
/*   Updated: 2024/09/11 13:48:32 by sbelomet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_H
# define WEBSERV_H

# include <vector>
# include <sstream> 
# include <iostream>
# include <sys/stat.h>

# define MAX_EVENTS 4096
# define BUFFER_SIZE 50000
# define RED "\033[1;31m"
# define GREEN "\033[1;32m"
# define YELLOW "\033[1;33m"
# define BLUE "\033[1;34m"
# define MAGENTA "\033[1;35m"
# define CYAN "\033[1;36m"
# define RESET "\033[0m"

/* UTILS */

bool	isSeparator(char c);
bool	isWhitespace(char c);
bool	isSeparatorNW(char c);
void	strtolower(std::string &str);
bool	isDirectory( std::string path );
bool	isRegularFile( std::string path );
bool	onlyWhitespaces(const std::string &str);
std::vector<std::string>	vecSplit( std::string const &str, char delimiter );
std::string::iterator findKeywordEnd(std::string::iterator it, std::string::iterator end);
std::string::iterator findKeywordEnd(std::string::iterator it, std::string::const_iterator end);

class Webserv
{
	private:
		Webserv();
		Webserv(Webserv const &copy);
		Webserv const &operator=(Webserv const &copy);
	public:
		~Webserv();

		class FileException : public std::exception
		{
			public:
				virtual const char *what() const throw();
		};

		class SocketException : public std::exception
		{
			public:
				virtual const char *what() const throw();
		};

		class SocketOptException : public std::exception
		{
			public:
				virtual const char *what() const throw();
		};

		class InetPtonException : public std::exception
		{
			public:
				virtual const char *what() const throw();
		};

		class NoException : public std::exception
		{
			public:
				virtual const char *what() const throw();
		};

		class EpollException : public std::exception
		{
			public:
				virtual const char *what() const throw();
		};
};

#endif