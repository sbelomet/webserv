/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgosselk <lgosselk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 10:16:14 by lgosselk          #+#    #+#             */
/*   Updated: 2024/08/14 13:29:20 by lgosselk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_HPP
# define CONFIG_HPP

# include <sstream>
# include <fstream>
# include "Section.hpp"

class Section;

class Config
{
    private:
        std::map<std::string, Section>  _config;
    public:
        Config();
        ~Config();
        Config( Config const &copy );

        Config const    &operator=( Config const &copy );

        std::map<std::string, Section>  &getConfig( void );
        void    setConfig( std::map<std::string, Section> const &config );
        std::map<std::string, Section> const    &getConfig( void ) const;

        Section &getSectionFromMap( std::string const &key );
        void    insertSection( std::string const &key, Section const &section );
        
        void    makeConfig( std::string const &filePath );

        class	FileException: public std::exception
		{
			public:
				virtual const char	*what() const throw();
		};
};

#endif
