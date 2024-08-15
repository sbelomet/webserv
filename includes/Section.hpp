/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Section.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgosselk <lgosselk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 10:04:57 by lgosselk          #+#    #+#             */
/*   Updated: 2024/08/14 13:37:44 by lgosselk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SECTION_HPP
# define SECTION_HPP

# include <map>
# include <iostream>

class Section
{
    private:
        std::map<std::string, std::string>    _section;
    public:
        Section();
        ~Section();
        Section( Section const &copy );

        Section const    &operator=( Section const &copy );

        std::map<std::string, std::string>  &getSection( void );
        void    setSection( std::map<std::string, std::string> const &section );
        std::map<std::string, std::string> const &getSection( void ) const;
        void    insertData( std::string const &key, std::string const &value );
};

#endif
