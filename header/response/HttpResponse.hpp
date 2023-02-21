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
		typedef std::map<std::string, std::vector<std::string> > 	map_server;

		HttpResponse();

		void setResponseInfo(HttpRequest const & request, std::map< std::string, std::vector< std::string > > & serverMap);

		std::string getResponseString();
		void		setHeader();

		std::vector<std::string> getPackets(map_server serverMap, std::string responseString);

		void		errorReturn();
		void		redirectTargetPath(std::string first, std::string second);

		std::string getErrorPath() const;
		std::string getCode();
		std::string getStatus();
		std::string getTargetPath() const;
		void		setError(std::string code, std::string status);
		void		setCode(std::string content);
		void		setStatus(std::string content);
		void		setBody(std::string src);

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
