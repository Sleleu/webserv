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

		CgiHandler(HttpResponse & response, HttpRequest const & request);

		void		setEnvMap(HttpResponse & response, HttpRequest const & request);
		void 		convertMap();
		void		setEnvChar();
		void		executeCgi();
		std::string	getOutput() const;
		std::string	getOutputBody() const;
		std::string	getOutputHeader() const;

	private :

		char**			_env;
		stringMap		_envMap;
		std::string		_scriptPath;
		std::string		_output;
		std::string		_filename;
};


#endif
