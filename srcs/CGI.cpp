/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbelomet <sbelomet@42lausanne.ch>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/04 10:43:35 by sbelomet          #+#    #+#             */
/*   Updated: 2024/09/04 14:27:55 by sbelomet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CGI.hpp"

CGI::CGI() {}
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

void CGI::fillEnv()
{
	_env["i dont know"] = "what to put here";
	_env["so here"] = "is a random env variable";
	_env["filled with"] = "random stuff";

	_malloc_env = new char*[_env.size() + 1];
	size_t i = 0;
	for (std::map<std::string, std::string>::iterator it = _env.begin(); it != _env.end(); it++)
	{
		_malloc_env[i] = new char[it->first.size() + it->second.size() + 2];
		strcpy(_malloc_env[i], (it->first + "=" + it->second).c_str());
		i++;
	}
	_malloc_env[i] = NULL;

	_argv = new char*[3];
	_argv[0] = strdup("/usr/bin/python3");
	_argv[1] = strdup("cgi.py");
	_argv[2] = NULL;
}

void CGI::executeCGI()
{
	pid_t pid;
	int pipefd[2];

	if (pipe(pipefd) == -1)
	{
		// error 500
		return ;
	}
	pid = fork();
	if (pid == -1)
	{
		// error 500
		return ;
	}
	else if (pid == 0)
	{
		dup2(pipefd[0], STDIN_FILENO);
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[0]);
		close(pipefd[1]);
		int exit_status = execve(_argv[0], _argv, _malloc_env);
		exit(exit_status);
	}
}