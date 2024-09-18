/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbelomet <sbelomet@42lausanne.ch>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 14:08:16 by lgosselk          #+#    #+#             */
/*   Updated: 2024/09/18 13:25:40 by sbelomet         ###   ########.fr       */
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
		bool						_get;
		bool						_post;
		std::string 				_root;
		std::string 				_index;
		std::string 				_alias;
		bool						_filled;
		bool						_remove;
		std::string					_return;
		std::vector<std::string>	_cgi_pass;
		std::string 				_location;
		bool						_autoindex;
		bool						_autoindex_set;
		size_t						_client_max_body_size;
	public:
		Location( void );
		virtual ~Location( void );

		Location( Location const &copy );
		Location const &operator=( Location const &copy );

		/* getters */

		bool const	&getGet( void ) const;
		bool const	&getPost( void ) const;
		bool const	&getRemove( void ) const;
		bool const	&getFilled( void ) const;
		bool const	&getAutoindex( void ) const;
		bool const	&getAutoindexSet( void ) const;
		std::string const	&getRoot( void ) const;
		std::string const	&getIndex( void ) const;
		std::string const	&getAlias( void ) const;
		std::string const	&getReturn( void ) const;
		std::string const	&getLocation( void ) const;
		std::vector<std::string>	&getCgiPass( void );
		size_t const	&getMaxClientBody( void ) const;
		std::vector<std::string> const	&getCgiPass( void ) const;

		/* setters */

		void	setGet( bool const & );
		void	setPost( bool const & );
		void	setRemove( bool const & );
		void	setFilled( bool const & );
		void	setAutoindex( bool const & );
		void	setRoot( std::string const & );
		void	setAutoindexSet( bool const & );
		void	setIndex( std::string const & );
		void	setAlias( std::string const & );
		void	setReturn( std::string const & );
		void	setMaxClientBody( size_t const & );
		void	setLocation( std::string const & );
		void	setCgiPass( std::vector<std::string> const & );

		/* access methods for vector/struct */

		void	clearLocation( void );
		void	pushCgiPass( std::string const &cgiExt );
		bool	isAllowedMethod( std::string const &method );
};

std::ostream &operator<<(std::ostream &out, Location &obj);

#endif