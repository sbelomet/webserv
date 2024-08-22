/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbelomet <sbelomet@42lausanne.ch>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 10:11:54 by sbelomet          #+#    #+#             */
/*   Updated: 2024/08/22 11:05:15 by sbelomet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <algorithm>
#include "Server.hpp"
#include "Webserv.hpp"
#include "MapConfig.hpp"


int main(int ac, char **av)
{
	if (ac != 2)
        std::cout << "Need a config file" << std::endl;
	else
	{
		Server		server;
		MapConfig	configs;
		try
		{
			configs.mappingConfigs(av[1]);
		//	configs.makeAll(server);
		}
		catch (Webserv::NoException const &e)
		{
			exit(1);
		}
		catch (std::exception &e)
		{
			std::cerr << RED << e.what() << RESET << std::endl;
			exit(1);
		}
	}
	return (0);
}