#ifndef HTTRESPONSE_H
#define HTTRESPONSE_H

# include <iostream>
# include <map>
# include <string>
# include <vector>
# include <sstream>
# include <cstddef>
# include "httpRequest.hpp"

class HttpResponse
{
	public:
		HttpResponse(HttpRequest request)
		{
			_responseMap["version"] = request.getVersion();
			_responseMap["code"] = "200";
			_responseMap["status"] = "OK";
			_responseMap["server"] = "webserv"; // set with config file
		}

	private :
		std::string	cgiAccepted; // set with config file

		std::map<std::string, std::string> _responseMap;
};


#endif