#ifndef HTTRESPONSE_H
#define HTTRESPONSE_H

# include <iostream>
# include <map>
# include <string>
# include <vector>
# include <sstream>
# include <cstddef>
# include <ctime>
# include <fstream>
# include <sys/stat.h>
# include "HttpRequest.hpp"

class HttpResponse
{
	public:
		HttpResponse(HttpRequest const & request, std::map< std::string, std::vector< std::string > > & serverMap);

		std::string getResponseString();

		void		errorReturn();
		void		redirectTargetPath(std::string first, std::string second);

		std::string getErrorPath() const { return _errorPath; }
		std::string getCode() { return _controlData["code"]; }
		std::string getStatus() { return _controlData["status"]; }
		std::string getTargetPath() const { return _targetPath; }
		void		setError(std::string code, std::string status) { setCode(code); setStatus(status); }
		void		setCode(std::string content) { _controlData["code"] = content; }
		void		setStatus(std::string content) { _controlData["status"] = content; }
		void		setBody(std::string src) { _body = src; }

		bool		isDirectory();
		bool		canUpload;
		bool		canCgi;

	private :

		std::string	_cgiAccepted; // set with config
		std::string _targetPath;
		std::string _errorPath;
		
		std::map<std::string, std::string> 	_controlData;
		std::map<std::string, std::string> 	_headers;
		std::string							_body;

};


#endif
