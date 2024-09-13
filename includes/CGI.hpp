/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbelomet <sbelomet@42lausanne.ch>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/04 10:43:51 by sbelomet          #+#    #+#             */
/*   Updated: 2024/09/13 13:55:45 by sbelomet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGI_HPP
# define CGI_HPP

# include "Webserv.hpp"
# include "Location.hpp"
# include "httpRequest.hpp"
# include <map>
# include <string.h>
# include <unistd.h>
# include <sys/wait.h>
# include <stdlib.h>
# include <cstdio>
# include <algorithm>

class CGI
{
	private:
		std::map<std::string, std::string> _env;
		std::map<std::string, std::string> _binaries;
		char **_malloc_env;
		char **_argv;
		int fd;
		std::string _output;

		CGI(CGI const &copy);
		CGI const &operator=(CGI const &copy);
		void fillBinaries(const std::vector<std::string> &cgiPass);
		std::string getBinary(std::string const &script);

	public:
		CGI();
		~CGI();

		std::map<std::string, std::string> const &getEnv( void ) const;
		std::string const &getOutput( void ) const;

		void fillEnv(httpRequest const &request, std::string const &script, Location const *cgiLocation);
		void executeCGI();
};

#endif