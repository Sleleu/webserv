#ifndef CGIHANDLER_H
#define CGIHANDLER_H

# include <iostream>
# include <map>
# include <string>
# include <map>
# include <vector>
# include <sstream>
# include <fstream>
# include <sys/types.h>
# include <unistd.h>
# include <sys/types.h>
# include <sys/wait.h>
# include "HttpResponse.hpp"
# include "body.hpp"
# include "../utils/colors.hpp"
# include <cstring>

class CgiHandler
{
	public:

		typedef std::map<std::string, std::string> 				stringMap;
		typedef std::map<std::string, std::map<std::string, std::string> > 	doubleMap;
		CgiHandler() //test
		{
			try
			{
				_scriptPath = "./html/script.sh";
				_envMap["UPLOAD"] = "upload";
				_envMap["UPLOAD_PATH"] = "upploadpath";
				_envMap["CONTENT"] = "content";
				setEnvChar();
				executeCgi();
			}
			catch (std::exception e)
			{
				std::cout << BOLDRED << "\nCGI_HANDLER ERROR\n" << RESET << std::endl;
				exit(0);
			}
		}

		CgiHandler(HttpResponse & response, HttpRequest const & request)
		{
			try
			{
				(void) response;
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

		void		executeCgi() const
		{
			int				pipeFd[2];

			if (pipe(pipeFd) < 0)
				throw std::exception();
			int idFork = fork();

			if (idFork < 0)
				throw std::exception();
			std::cout << idFork << std::endl;
			if (idFork == 0)
			{
				// char ** nullTab = new char*[2];
				// nullTab[0] = new char[_scriptPath.size() + 1];
				// nullTab[1] = NULL;
				// strcpy(nullTab[0], (const char*)_scriptPath.c_str());
				dup2(pipeFd[1], 1);
				close(pipeFd[0]);
				close(pipeFd[1]);
				execve(_scriptPath.c_str(), nullTab, _env);
				perror("execve");
				std::cout << "PROBLEM" << std::endl;
				exit (0);
			}
			waitpid(-1, NULL, 0);
			std::cout << "fork fini" << std::endl;

			char buf[100];
			read(pipeFd[0], buf, 100);
			std::cout << BLUE << "BUF = " << buf << RESET << std::endl;
		}



		std::string	getCgiReturn() const { return "h";}

	private :

		char**			_env;
		stringMap		_envMap;
		std::string		_scriptPath;
		// int				_pipeFd[2];
		// int				_idFork;
		std::string		_returnString;
};


#endif
