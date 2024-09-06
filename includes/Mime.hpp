/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mime.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbelomet <sbelomet@42lausanne.ch>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 14:41:57 by sbelomet          #+#    #+#             */
/*   Updated: 2024/09/06 15:36:14 by sbelomet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MIME_HPP
# define MIME_HPP
# include <map>
# include <string>

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
		std::string	getMimeType( std::string const &extension );
};

#endif