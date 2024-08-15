/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Section.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgosselk <lgosselk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 10:16:05 by lgosselk          #+#    #+#             */
/*   Updated: 2024/08/14 13:38:00 by lgosselk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Section.hpp"

Section::Section( void )
{}

Section::~Section( void )
{
    // delete if malloced
}

Section::Section( Section const &copy )
{
    *this = copy;
}

Section const    &Section::operator=( Section const &copy )
{
    if (this != &copy)
        this->setSection(copy.getSection());
    return (*this);
}

std::map<std::string, std::string>  &Section::getSection( void )
{
    return (this->_section);
}

void    Section::setSection( std::map<std::string, std::string> const &section )
{
    this->_section = section;
}

std::map<std::string, std::string> const &Section::getSection( void ) const
{
    return (this->_section);
}

void    Section::insertData( std::string const &key, std::string const &value )
{
    this->_section[key] = value;
}
