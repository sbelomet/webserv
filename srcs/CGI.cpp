/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbelomet <sbelomet@42lausanne.ch>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/04 10:43:35 by sbelomet          #+#    #+#             */
/*   Updated: 2024/09/13 14:50:49 by sbelomet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CGI.hpp"

CGI::CGI() :_output(std::string()) {}
CGI::CGI(CGI const &copy) { (void)copy; }
CGI const &CGI::operator=(CGI const &copy) { (void)copy; return *this; }

CGI::~CGI()
{
	if (_malloc_env)
	{
		for (size_t i = 0; _malloc_env[i]; i++)
			delete [] _malloc_env[i];
		delete [] _malloc_env;
	}
	if (_argv)
	{
		for (size_t i = 0; _argv[i]; i++)
			delete [] _argv[i];
		delete [] _argv;
	}
}

std::map<std::string, std::string> const &CGI::getEnv( void ) const { return (_env); }
std::string const &CGI::getOutput( void ) const { return (_output); }

/**
 * Fills the _binaries map with the binaries that can be executed by the CGI
 */
void	CGI::fillBinaries(const std::vector<std::string> &cgiPass)
{
	std::map<std::string, std::string> tmp_binaries;
	
	tmp_binaries["/usr/bin/python3"] = ".py";
	tmp_binaries["/usr/bin/php"] = ".php";
	tmp_binaries["/usr/bin/perl"] = ".pl";
	tmp_binaries["/usr/bin/ruby"] = ".rb";
	tmp_binaries["/usr/bin/node"] = ".js";
	tmp_binaries["/usr/bin/bash"] = ".sh";
	tmp_binaries["/usr/bin/sh"] = ".sh";
	tmp_binaries["/usr/bin/zsh"] = ".sh";

	tmp_binaries["/bin/python3"] = ".py";
	tmp_binaries["/bin/php"] = ".php";
	tmp_binaries["/bin/perl"] = ".pl";
	tmp_binaries["/bin/ruby"] = ".rb";
	tmp_binaries["/bin/node"] = ".js";
	tmp_binaries["/bin/bash"] = ".sh";
	tmp_binaries["/bin/sh"] = ".sh";
	tmp_binaries["/bin/zsh"] = ".sh";

	for (std::map<std::string, std::string>::iterator it = tmp_binaries.begin(); it != tmp_binaries.end(); it++)
	{
		if (find(cgiPass.begin(), cgiPass.end(), it->first) != cgiPass.end())
			_binaries[it->first] = it->second;
	}
}

/**
 * Returns a binary based on the extension of the script, returns "500" if none found
 */
std::string	CGI::getBinary(std::string const &script)
{
	std::string extension = script.substr(script.find_last_of('.'));
	for (std::map<std::string, std::string>::iterator it = _binaries.begin(); it != _binaries.end(); it++)
	{
		if (it->second == extension)
			return (it->first);
	}
	return ("500");
}

/**
 * 	std::string			string;
	HTTPResponse		&response = client.getResponse();
	HTTPHeader			&header = client.getHeader();
	std::string			file = sock.getRealUrl(sockNbr, response.getUrl());
    
	client.setEnvValue("SERVER_NAME", sock.getServerName(sockNbr));
    client.setEnvValue("GATEWAY_INTERFACE", "CGI/1.1");
	client.setEnvValue("PATH_INFO", file);
	client.setEnvValue("REQUEST_METHOD", response.getMethod());
	client.setEnvValue("SCRIPT_FILENAME", file);
	client.setEnvValue("SERVER_PROTOCOL", "HTTP/1.1");
	client.setEnvValue("REDIRECT_STATUS", "200");
	if (header.getContentType() == "")
		client.setEnvValue("CONTENT_TYPE", "application/x-www-form-urlencoded");
	else
		client.setEnvValue("CONTENT_TYPE", header.getContentType());
	if (client.getEnvValue("REQUEST_METHOD") == "POST")
		client.setEnvValue("CONTENT_LENGTH", toString(client.getBodySize()));
	else
	{
		if (client.isQueryString() == false)
			client.setEnvValue("QUERY_STRING", "");
	}
 */

/**
 * Fills and allocates the nessessary variables for the CGI execution
 * !!! Check status code after !!!
 */
void CGI::fillEnv(httpRequest const &request, std::string const &script, Location const *cgiLocation)
{
	if (cgiLocation == NULL)
	{
		_env["REDIRECT_STATUS"] = "404";
		return ;
	}
	
	fillBinaries(cgiLocation->getCgiPass());
	
	std::map<std::string, std::string> headers = request.getHeaders();
	int pos = headers["host"].find(":");
	_env["SERVER_NAME"] = headers["host"].substr(0, pos);
	_env["SERVER_PORT"] = headers["host"].substr(pos + 1);
	_env["GATEWAY_INTERFACE"] = "CGI/1.1";
	_env["PATH_INFO"] = script;
	_env["REQUEST_METHOD"] = request.getMethod();
	_env["SCRIPT_FILENAME"] = script;
	_env["SERVER_PROTOCOL"] = request.getVersion();
	_env["REDIRECT_STATUS"] = "200";
	_env["CONTENT_TYPE"] = headers["content-type"];
	_env["CONTENT_LENGTH"] = headers["content-length"];
	//_env["QUERY_STRING"] = "";

	for (std::map<std::string, std::string>::iterator it = _env.begin(); it != _env.end(); it++)
	{
		std::cout << it->first << " : " << it->second << std::endl;
	}

	_malloc_env = new char*[_env.size() + 1];
	size_t i = 0;
	for (std::map<std::string, std::string>::iterator it = _env.begin(); it != _env.end(); it++)
	{
		_malloc_env[i] = new char[it->first.size() + it->second.size() + 2];
		strcpy(_malloc_env[i], (it->first + "=" + it->second).c_str());
		i++;
	}
	_malloc_env[i] = NULL;

	std::string binary = getBinary(script);
	if (binary == "500")
	{
		_env["REDIRECT_STATUS"] = "500";
		return ;
	}
	_argv = new char*[3];
	_argv[0] = strdup(binary.c_str());
	_argv[1] = strdup(script.c_str());
	_argv[2] = NULL;
}

/**
 * Executes the CGI script and writes the response to the client
 * !!! Check status code after !!!
 */
void CGI::executeCGI(void)
{
	pid_t pid;
	int status;
	FILE *tmpfile = std::tmpfile();
	int tmpfd = fileno(tmpfile);

	pid = fork();
	if (pid == -1)
	{
		_env["REDIRECT_STATUS"] = "500";
		return ;
	}
	else if (pid == 0)
	{
		dup2(tmpfd, STDOUT_FILENO);
		if (execve(_argv[0], _argv, _malloc_env) == -1)
		{
			_env["REDIRECT_STATUS"] = "500";
			exit(1);
		}
	}
	else
	{
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
		{
			if (WEXITSTATUS(status) != 0)
			{
				_env["REDIRECT_STATUS"] = "500";
				std::cout << "CGI script exited with status " << WEXITSTATUS(status) << std::endl;
				return ;
			}
		}
		else
		{
			_env["REDIRECT_STATUS"] = "500";
			return ;
		}
	}

	char buffer[1024];
	rewind(tmpfile);
	while (!feof(tmpfile))
	{
		if (fgets(buffer, 1024, tmpfile) == NULL)
			break;
		_output += buffer;
	}
	close(tmpfd);
}