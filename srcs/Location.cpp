/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgosselk <lgosselk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 14:09:46 by lgosselk          #+#    #+#             */
/*   Updated: 2024/09/19 15:09:56 by lgosselk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Location.hpp"

Location::Location( void ) : _get(false), _post(false), _root(std::string()), _index(std::string()),
	_alias(std::string()), _filled(false), _remove(false), _autoindex(false), _autoindex_set(false),
	_client_max_body_size(1) // A revoir
{}

Location::~Location( void )
{}

Location::Location( Location const &copy )
{
    *this = copy;
}

Location const    &Location::operator=( Location const &copy )
{
    if (this != &copy)
	{
		setGet(copy.getGet());
		setPost(copy.getPost());
		setRoot(copy.getRoot());
		setIndex(copy.getIndex());
		setAlias(copy.getAlias());
		setRemove(copy.getRemove());
		setFilled(copy.getFilled());
		setCgiPass(copy.getCgiPass());
		setReturn(copy.getReturn());
		setLocation(copy.getLocation());
		setAutoindex(copy.getAutoindex());
		setAutoindexSet(copy.getAutoindexSet());
		setMaxClientBody(copy.getMaxClientBody());
	}
    return (*this);
}

/*  */

bool const	&Location::getGet( void ) const
{
	return (_get);
}
bool const	&Location::getPost( void ) const
{
	return (_post);
}
bool const	&Location::getRemove( void ) const
{
	return (_remove);
}

bool const	&Location::getFilled( void ) const
{
	return (_filled);
}

bool const	&Location::getAutoindex( void ) const
{
	return (_autoindex);
}

bool const	&Location::getAutoindexSet( void ) const
{
	return (_autoindex_set);
}

std::string const	&Location::getRoot( void ) const
{
	return (_root);
}

std::string const	&Location::getIndex( void ) const
{
	return (_index);
}

std::string const	&Location::getReturn( void ) const
{
	return (_return);
}

std::string const	&Location::getAlias( void ) const
{
	return (_alias);
}

std::vector<std::string>	&Location::getCgiPass( void )
{
	return (_cgi_pass);
}

size_t const	&Location::getMaxClientBody( void ) const
{
	return (_client_max_body_size);
}

std::string const	&Location::getLocation( void ) const
{
	return (_location);
}

std::vector<std::string> const	&Location::getCgiPass( void ) const
{
	return (_cgi_pass);
}

/*  */

void	Location::setGet( bool const &get )
{
	_get = get;
}
void	Location::setPost( bool const &post )
{
	_post = post;
}
void	Location::setRemove( bool const &remove )
{
	_remove = remove;
}

void	Location::setFilled( bool const &filled )
{
	_filled = filled;
}

void	Location::setRoot( std::string const &root )
{
	_root = root;
}

void	Location::setAlias( std::string const &alias )
{
	_alias = alias;
}

void	Location::setIndex( std::string const &index )
{
	_index = index;
}

void	Location::setAutoindex( bool const &autoindex )
{
	_autoindex = autoindex;
}

void	Location::setReturn( std::string const &returnStr )
{
	_return = returnStr;
}

void	Location::setAutoindexSet( bool const &autoindexSet )
{
	_autoindex_set = autoindexSet;
}

// !!! Removes trailing slash
void	Location::setLocation( std::string const &location )
{
	if (location.size() > 1 && location[location.size() - 1] == '/')
		_location = location.substr(0, location.size() - 1);
	else
		_location = location;
}

void	Location::setCgiPass( std::vector<std::string> const &cgiPass )
{
	_cgi_pass = cgiPass;
}

void	Location::setMaxClientBody( size_t const &maxClientBody )
{
	_client_max_body_size = maxClientBody;
}

/*  */

void	Location::pushCgiPass( std::string const &cgiPass )
{
	_cgi_pass.push_back(cgiPass);
}

void	Location::clearLocation( void )
{
	setGet(false);
	setPost(false);
	setFilled(false);
	setRemove(false);
	setAutoindex(false);
	setMaxClientBody(1);
	getCgiPass().clear();
	setAutoindexSet(false);
	setRoot(std::string());
	setIndex(std::string());
	setAlias(std::string());
	setReturn(std::string());
	setLocation(std::string());
}

bool	Location::isAllowedMethod( std::string const &method )
{
	if (method == "GET" && getGet())
		return (true);
	else if (method == "POST" && getPost())
		return (true);
	else if (method == "DELETE" && getRemove())
		return (true);
	return (false);
}

std::ostream &operator<<(std::ostream &out, Location &obj)
{
	out << "Location: " << obj.getLocation() << std::endl;
	out << "Root: " << obj.getRoot() << std::endl;
	out << "Index: " << obj.getIndex() << std::endl;
	out << "Allowed methods: " << (obj.getGet() ? "GET " : "") << (obj.getPost() ? "POST " : "") << (obj.getRemove() ? "DELETE " : "") << std::endl;
	out << "Alias: " << obj.getAlias() << std::endl;
	out << "Return: " << obj.getReturn() << std::endl;
	out << "Autoindex: " << obj.getAutoindex() << std::endl;
	out << "Max client body size: " << obj.getMaxClientBody() << std::endl;
	out << "Cgi pass: ";
	std::vector<std::string> cgiPass = obj.getCgiPass();
	for (std::vector<std::string>::iterator it = cgiPass.begin(); it != cgiPass.end(); it++)
		out << *it << " ";
	out << std::endl;
	return (out);
}