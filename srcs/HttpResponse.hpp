#ifndef HTTRESPONSE_H
#define HTTRESPONSE_H

# include <iostream>
# include <map>
# include <string>
# include <vector>
# include <sstream>
# include <cstddef>
# include "httpRequest.hpp"
# include <ctime>
# include "webServHttp.hpp"

class HttpResponse
{
	public:
		HttpResponse(HttpRequest const & request)
		{
			_controlData["version"] = request.getVersion();
			_controlData["code"] = "200";
			_controlData["status"] = "OK";
			_headers["content-type"] = "text/html"; // set with request
			_headers["content-length"] = "1024"; // set with request (sizeof)
			_headers["server"] = "webserv"; // set with config
			
			_targetPath = "./test_files" + request.getTarget(); // <root> set with config
		}

		std::string getResponseString()
		{
			std::string controlDataString = _controlData["version"] + " " \
				+ _controlData["code"] + " " + _controlData["status"] + "\n";

			std::string headersString;
			for (std::map<std::string, std::string>::const_iterator it = _headers.begin() ; \
				it != _headers.end() ; it ++)
			{
				if (it->second != "")
					headersString += it->first + ": " + it->second + "\n";
			}
			return (controlDataString + headersString + _body);
		}

		std::string getTargetPath() const { return _targetPath; }
		void		setBody(std::string src) { _body = src; }

	private :

		std::string	_cgiAccepted; // set with config
		std::string _targetPath;

		std::map<std::string, std::string> 	_controlData;
		std::map<std::string, std::string> 	_headers;
		std::string							_body;

};


#endif