/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mime.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgosselk <lgosselk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 14:41:57 by sbelomet          #+#    #+#             */
/*   Updated: 2024/09/24 11:15:26 by lgosselk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MIME_HPP
# define MIME_HPP
# include <map>
# include <string>
# include "Webserv.hpp"

class Mime
{
	private:
		std::map<std::string, std::string> _mimeTypes;

	public:
		Mime();
		~Mime();
		Mime(Mime const &copy);
		Mime const &operator=(Mime const &copy);

		void			initMimeTypes( void );
		std::string	getMimeType( std::string const &extension,
			std::string const &acceptList );
};

#endif