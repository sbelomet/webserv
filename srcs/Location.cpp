/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbelomet <sbelomet@42lausanne.ch>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 14:09:46 by lgosselk          #+#    #+#             */
/*   Updated: 2024/09/10 10:29:48 by sbelomet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Location.hpp"

Location::Location( void ) : _root(""), _index(""), _alias(""), _autoindex(false), _autoindex_set(false), _client_max_body_size(1)
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
		setRoot(copy.getRoot());
		setIndex(copy.getIndex());
		setAlias(copy.getAlias());
		setCgiPass(copy.getCgiPass());
		setReturn(copy.getReturn());
		setLocation(copy.getLocation());
		setAutoindex(copy.getAutoindex());
		setMaxClientBody(copy.getMaxClientBody());
		setAllowedMethods(copy.getAllowedMethods());
	}
    return (*this);
}

/*  */

bool const	&Location::getAutoindex( void ) const
{
	return (_autoindex);
}

bool const	&Location::getAutoindexSet( void ) const
{
	return (_autoindex_set);
}

s_return const	&Location::getReturn( void ) const
{
	return (_return);
}

std::string const	&Location::getRoot( void ) const
{
	return (_root);
}

std::string const	&Location::getIndex( void ) const
{
	return (_index);
}

std::string const	&Location::getAlias( void ) const
{
	return (_alias);
}

size_t const	&Location::getMaxClientBody( void ) const
{
	return (_client_max_body_size);
}

std::string const	&Location::getLocation( void ) const
{
	return (_location);
}

s_methods const	&Location::getAllowedMethods( void ) const
{
	return (_allowed_methods);
}

std::vector<std::string> const	&Location::getCgiPass( void ) const
{
	return (_cgi_pass);
}

/*  */

void	Location::setRoot( std::string const &root )
{
	_root = root;
}

void	Location::setIndex( std::string const &index )
{
	_index = index;
}

void	Location::setAutoindex( bool const &autoindex )
{
	_autoindex = autoindex;
}

void	Location::setAutoindexSet( bool const &autoindexSet )
{
	_autoindex_set = autoindexSet;
}

void	Location::setAlias( std::string const &alias )
{
	_alias = alias;
}

void	Location::setReturn( s_return const &return_values )
{
	_return.path = return_values.path;
	_return.status = return_values.status;
}

// !!! Removes trailing slash
void	Location::setLocation( std::string const &location )
{
	if (location.size() > 1 && location[location.size() - 1] == '/')
		_location = location.substr(0, location.size() - 1);
	else
		_location = location;
}

void	Location::setAllowedMethodsSet( bool const &allowedMethodsSet )
{
	_allowed_methods.var_set = allowedMethodsSet;
}

void	Location::setAllowedMethods( s_methods const &allowedMethods )
{
	_allowed_methods.get = allowedMethods.get;
	_allowed_methods.post = allowedMethods.post;
	_allowed_methods.remove = allowedMethods.remove;
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

void	Location::switchGet( void )
{
	_allowed_methods.get = !_allowed_methods.get;
}

void	Location::switchPost( void )
{
	_allowed_methods.post = !_allowed_methods.post;
}

void	Location::switchRemove( void )
{
	_allowed_methods.remove = !_allowed_methods.remove;
}

void	Location::pushCgiPass( std::string const &cgiPass )
{
	_cgi_pass.push_back(cgiPass);
}

void	Location::setPathFromReturn( std::string const &path )
{
	_return.path = path;
}

void	Location::setStatusFromReturn( std::string const &status )
{
	_return.status = status;
}

bool	Location::isAllowedMethod( std::string const &method )
{
	s_methods	methods = getAllowedMethods();
	if (method == "GET" && methods.get)
		return (true);
	else if (method == "POST" && methods.post)
		return (true);
	else if (method == "DELETE" && methods.remove)
		return (true);
	return (false);
}