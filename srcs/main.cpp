/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgosselk <lgosselk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 10:11:54 by sbelomet          #+#    #+#             */
/*   Updated: 2024/08/28 15:37:35 by lgosselk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <algorithm>
#include "Server.hpp"
#include "Webserv.hpp"
#include "Manager.hpp"
#include "MapConfig.hpp"

int main(int ac, char **av)
{
	if (ac != 2)
		std::cout << "Need a config file" << std::endl;
	else
	{
		Server		server;
		Manager		manager;
		try
		{
			manager.makeAll(server, av[1]);
			//std::cout << manager.getMapConfig() << std::endl;
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
