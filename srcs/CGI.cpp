/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbelomet <sbelomet@42lausanne.ch>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/04 10:43:35 by sbelomet          #+#    #+#             */
/*   Updated: 2024/09/26 11:28:52 by sbelomet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CGI.hpp"

CGI::CGI() : _malloc_env(NULL), _argv(NULL), _output(std::string()) {}
CGI::CGI(CGI const &copy) { (void)copy; }
CGI const &CGI::operator=(CGI const &copy) { (void)copy; return *this; }

CGI::~CGI()
{
	if (_malloc_env)
	{
		for (size_t i = 0; _malloc_env[i]; i++)
			free(_malloc_env[i]);
		free(_malloc_env);
	}
	if (_argv)
	{
		for (size_t i = 0; _argv[i]; i++)
			free(_argv[i]);
		free(_argv);
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
	tmp_binaries["/usr/bin/zsh"] = ".sh";
	tmp_binaries["/usr/bin/sh"] = ".sh";

	tmp_binaries["/bin/python3"] = ".py";
	tmp_binaries["/bin/php"] = ".php";
	tmp_binaries["/bin/perl"] = ".pl";
	tmp_binaries["/bin/ruby"] = ".rb";
	tmp_binaries["/bin/node"] = ".js";
	tmp_binaries["/bin/bash"] = ".sh";
	tmp_binaries["/bin/zsh"] = ".sh";
	tmp_binaries["/bin/sh"] = ".sh";

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
 * Fills the _env map with the nessessary variables for the CGI execution
 */
void CGI::fillEnv(httpRequest const &request, std::string const &script)
{
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
}

/**
 * Fills and allocates the nessessary variables for the CGI execution
 * !!! Check status code after !!!
 */
void CGI::setupCGI(httpRequest const &request, std::string const &script, Location const cgiLocation)
{	
	fillBinaries(cgiLocation.getCgiPass());
	
/* 	for (std::map<std::string, std::string>::iterator it = _binaries.begin(); it != _binaries.end(); it++)
	{
		std::cout << it->first << " : " << it->second << std::endl;
	} */

	fillEnv(request, script);

/* 	for (std::map<std::string, std::string>::iterator it = _env.begin(); it != _env.end(); it++)
	{
		std::cout << it->first << " : " << it->second << std::endl;
	} */

	_malloc_env = (char **)malloc(sizeof(char *) * (_env.size() + 1));
	size_t i = 0;
	for (std::map<std::string, std::string>::iterator it = _env.begin(); it != _env.end(); it++)
	{
		_malloc_env[i] = (char *)malloc(it->first.size() + it->second.size() + 2);
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
	_argv = (char **)malloc(sizeof(char *) * 3);
	_argv[0] = strdup(binary.c_str());
	_argv[1] = strdup(script.c_str());
	_argv[2] = NULL;
}

/**
 * Executes the CGI script and writes the response to the client
 * !!! Check status code after !!!
 */
void CGI::executeCGI(std::string const &body)
{
	pid_t pid;
	int status;
	FILE *tmpfile = std::tmpfile();
	int tmpfd = fileno(tmpfile);

	FILE *bodyfile = std::tmpfile();
	int bodyfd = fileno(bodyfile);
	if (write(bodyfd, body.c_str(), body.size()) == -1)
	{
		perror("write()");
		throw (Webserv::NoException());
	}
	rewind(bodyfile);

	pid = fork();
	if (pid == -1)
	{
		_env["REDIRECT_STATUS"] = "500";
		return ;
	}
	else if (pid == 0)
	{
		pid_t execPID = fork();
		if (execPID == -1)
		{
			perror("fork()");
			throw (Webserv::NoException());
		}
		else if (execPID == 0)
		{
			if (dup2(bodyfd, STDIN_FILENO) == -1)
			{
				perror("dup2()");
				exit(1);
			}
			if (dup2(tmpfd, STDOUT_FILENO) == -1)
			{
				perror("dup2()");
				exit(1);
			}
			if (dup2(tmpfd, STDERR_FILENO) == -1)
			{
				perror("dup2()");
				exit(1);
			}
			close(bodyfd);
			close(tmpfd);
			execve(_argv[0], _argv, _malloc_env);
			perror("execve()");
			throw (Webserv::NoException());
		}
		pid_t timerPID = fork();
		if (timerPID == -1)
		{
			perror("fork()");
			throw (Webserv::NoException());
		}
		else if (timerPID == 0)
		{
			sleep(CGITIMEOUT);
			throw (Webserv::NoException());
		}
		pid_t killerPID = wait(&status);
		if (killerPID == execPID)
		{
			fclose(tmpfile);
			fclose(bodyfile);
			kill(timerPID, SIGKILL);
		}
		else
		{
			std::cout << YELLOW << "CGI timeout" << RESET << std::endl;
			kill(execPID, SIGKILL);
			if (write(tmpfd, "CGI timeout", 11) == -1)
			{
				perror("write()");
				throw (Webserv::NoException());
			}
		}
		close(bodyfd);
		close(tmpfd);
		wait(NULL);
		throw (Webserv::NoException());

	}
	else
	{
		waitpid(pid, &status, 0);
	}

	char buffer[1024];
	rewind(tmpfile);
	while (!feof(tmpfile))
	{
		if (fgets(buffer, 1024, tmpfile) == NULL)
			break;
		_output += buffer;
	}
	if (_output == "CGI timeout")
		_env["REDIRECT_STATUS"] = "504";
	close(bodyfd);
	close(tmpfd);
	fclose(bodyfile);
	fclose(tmpfile);
}