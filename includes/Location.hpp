/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbelomet <sbelomet@42lausanne.ch>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 14:08:16 by lgosselk          #+#    #+#             */
/*   Updated: 2024/09/10 10:28:16 by sbelomet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATION_HPP
# define LOCATION_HPP

# include <vector>
# include <iostream>

struct s_return
{
	std::string path;
	std::string status;
	s_return()
	{
		path = "";
		status = "";
	}
};

struct s_methods
{
	bool	var_set;
	bool	get;
	bool	post;
	bool	remove;
	s_methods()
	{
		var_set = false;
		get = false;
		post = false;
		remove = false;
	}
};

class Location
{
	private:
		std::string 				_root;
		std::string 				_index;
		std::string 				_alias;
		s_return					_return;
		std::vector<std::string>	_cgi_pass;
		std::string 				_location;
		bool						_autoindex;
		bool						_autoindex_set;
		s_methods					_allowed_methods;
		size_t						_client_max_body_size;

		Location( Location const &copy );
		Location const &operator=( Location const &copy );
	public:
		Location( void );
		~Location( void );

		/* getters */
		bool const	&getAutoindex( void ) const;
		s_return const	&getReturn( void ) const;
		bool const	&getAutoindexSet( void ) const;
		std::string const	&getRoot( void ) const;
		std::string const	&getIndex( void ) const;
		std::string const	&getAlias( void ) const;
		std::string const	&getLocation( void ) const;
		size_t const	&getMaxClientBody( void ) const;
		s_methods const	&getAllowedMethods( void ) const;
		std::vector<std::string> const	&getCgiPass( void ) const;

		/* setters */
		void	setAutoindex( bool const & );
		void	setReturn( s_return const & );
		void	setRoot( std::string const & );
		void	setAutoindexSet( bool const & );
		void	setIndex( std::string const & );
		void	setAlias( std::string const & );
		void	setMaxClientBody( size_t const & );
		void	setLocation( std::string const & );
		void	setAllowedMethods( s_methods const & );
		void	setCgiPass( std::vector<std::string> const & );
		void	setAllowedMethodsSet( bool const &allowedMethodsSet );

		/* access methods for vector/struct */

		void	switchGet( void );
		void	switchPost( void );
		void	switchRemove( void );
		void	pushCgiPass( std::string const &cgiExt );
		void	setPathFromReturn( std::string const &path );
		bool	isAllowedMethod( std::string const &method );
		void	setStatusFromReturn( std::string const &status );
};

#endif