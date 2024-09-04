/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbelomet <sbelomet@42lausanne.ch>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/04 10:43:51 by sbelomet          #+#    #+#             */
/*   Updated: 2024/09/04 13:41:48 by sbelomet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGI_HPP
# define CGI_HPP

# include "Webserv.hpp"
# include <map>
# include <string.h>
# include <unistd.h>
# include <sys/wait.h>

class CGI
{
	private:
		std::map<std::string, std::string> _env;
		char **_malloc_env;
		char **_argv;

		CGI(CGI const &copy);
		CGI const &operator=(CGI const &copy);

	public:
		CGI();
		~CGI();

		void fillEnv();
		void executeCGI();
};

#endif