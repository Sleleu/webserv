#ifndef CGIHANDLER_H
#define CGIHANDLER_H

# include <unistd.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <iostream>
# include <map>
# include <string>
# include <map>
# include <vector>
# include <sstream>
# include <fstream>
# include "HttpResponse.hpp"
# include "body.hpp"
# include "../utils/colors.hpp"
# include <cstring>

#define BUFFER_SIZE 4096

class CgiHandler
{
	public:

		typedef std::map<std::string, std::string> 							stringMap;
		typedef std::map<std::string, std::map<std::string, std::string> > 	doubleMap;
		// CgiHandler() //test
		// {
		// 	try
		// 	{
		// 		_scriptPath = "./html/cgi-bin/env.sh";
		// 		_envMap["UPLOAD"] = "upload";
		// 		_envMap["UPLOAD_PATH"] = "upploadpath";
		// 		_envMap["CONTENT"] = "content";
		// 		setEnvChar();
		// 		executeCgi();
		// 	}
		// 	catch (std::exception e)
		// 	{
		// 		std::cout << BOLDRED << "\nCGI_HANDLER ERROR\n" << RESET << std::endl;
		// 		exit(0);
		// 	}
		// }

		CgiHandler(HttpResponse & response, HttpRequest const & request) : _output("")
		{
			if (response.getCgiPath() == "" && response.getExtension() != "cgi")
			{
				std::cout << BOLDRED << " NO CGI" << RESET;
				return ;
			}
			try
			{
				setEnvMap(response, request);
				_scriptPath = (response.getCgiPath() != "") ? response.getCgiPath():response.getTargetPath();
				setEnvChar();
				executeCgi();
			}
			catch (std::exception e)
			{
				std::cout << BOLDRED << "\nCGI_HANDLER ERROR\n" << RESET << std::endl;
				response.setError("500", "Internal Server Error");
				response.setBody(BODY_500);
			}
		}

		void		setEnvMap(HttpResponse & response, HttpRequest const & request)
		{
			stringMap	argMap = request.getArgs();
			for (stringMap::const_iterator it = argMap.begin() ; it != argMap.end() ; it ++)
				_envMap[it->first] = _envMap[it->second];
			_envMap["UPLOAD"] = response.getIsUpload();
			_envMap["UPLOAD_PATH"] = response.getUploadPath();
			_envMap["CONTENT"] = request.getContent();
		}

		void		setEnvChar()
		{
			_env = new char*[_envMap.size() + 1];
			int i = 0;
			for (stringMap::const_iterator it = _envMap.begin() ; it != _envMap.end() ; it ++)
			{
				std::string line = it->first + "=" + it->second;
				_env[i] = new char[line.size() + 1];
				strcpy(_env[i], (const char*)line.c_str());
				i ++;
			}
			_env[i] = NULL;
		}

		void		executeCgi()
		{
			int pipefd[2];
			pid_t pid;

			std::vector<char*> args;
			args.push_back((char*)"/bin/bash");
			args.push_back((char*)_scriptPath.c_str());
			args.push_back(NULL);

			//env
			for (int i = 0; _env[i] != NULL ; i++)
			{
				std::cout << _env[i] << std::endl;
			}

			if (pipe(pipefd) == -1)
			{
				std::cerr << "Error: failed to create pipe" << std::endl;
				return ;
			}

			pid = fork();
			if (pid == -1) {
				std::cerr << "Error: failed to fork process" << std::endl;
				return ;
			}
			else if (pid == 0)
			{

				close(pipefd[0]);
				dup2(pipefd[1], STDOUT_FILENO);
				close(pipefd[1]);

				char** argv = &args[0];
				execve(argv[0], argv, _env);
				std::cerr << "Error: failed to execute script" << std::endl;
				return ;
			}
			else
			{
				close(pipefd[1]);
				char buffer[BUFFER_SIZE];
				_output = "";
				int nbytes;
				while ((nbytes = read(pipefd[0], buffer, BUFFER_SIZE)) > 0)
					_output += std::string(buffer, nbytes);
				std::cout << "Child process output: " << _output << std::endl;
				close(pipefd[0]);

				int status;
				waitpid(pid, &status, 0);
				if (WIFEXITED(status))
					std::cout << "Child process exited with status " << WEXITSTATUS(status) << std::endl;
			}
		}

		std::string	getOutput() const { return _output;}

	private :

		char**			_env;
		stringMap		_envMap;
		std::string		_scriptPath;
		std::string		_output;
};


#endif
