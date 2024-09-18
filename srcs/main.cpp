/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbelomet <sbelomet@42lausanne.ch>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 10:11:54 by sbelomet          #+#    #+#             */
/*   Updated: 2024/09/18 14:30:22 by sbelomet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <signal.h>
#include <algorithm>
#include "Server.hpp"
#include "Webserv.hpp"
#include "Manager.hpp"
#include "MapConfig.hpp"

void handle_signal(int)
{
	return ;
}

int main(int ac, char **av)
{
	struct sigaction sa;
    memset( &sa, 0, sizeof(sa) );
    sa.sa_handler = handle_signal;
    sigfillset(&sa.sa_mask);
    sigaction(SIGINT,&sa,NULL);

	if (ac != 2)
		std::cout << "Need a config file" << std::endl;
	else
	{
		Manager		manager;
		try
		{
			manager.makeAll(av[1]);
		}
		catch (Webserv::NoException const &e)
		{
			return 1;
		}
		catch (std::exception &e)
		{
			std::cerr << RED << e.what() << RESET << std::endl;
			return 1;
		}
	}
	return (0);
}
