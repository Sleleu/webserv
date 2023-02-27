#ifndef HTTPREQUEST_H
#define HTTPREQUEST_H

# include <iostream>
# include <map>
# include <string>
# include <map>
# include <vector>
# include <sstream>
# include <cstddef>

class HttpRequest
{
	public:
		HttpRequest();

		void setRequestInfo(std::string const requestMsg);
		std::vector<std::string> getLocation() const;
		void		setTarget();

		std::string 						getMethod() const;
		std::string 						getTarget() const;
		std::string 						getVersion() const;
		std::string 						getHeader(std::string key);
		std::string 						getBody() const;
		void 								setBody(std::string src);
		void 								setContent(std::string src);
		std::map<std::string, std::string>	getArgs() const;
		void								setLocationBlocName(std::string src);
		std::string 						getLocationBlocName() const;
		std::string 						getContent() const;
		std::string 						getFileName() const;

		void 								setFileName(std::string src);
		void								checkParsing() const;
		bool								parsing;

		class HttpVersion : public std::exception {};

	private :
		std::vector<std::string>            _controlData;
		std::map<std::string, std::string>  _headerMap;
		std::string                         _body;
		std::string 						_locationBlocName;
		std::string 						_target;
		std::map<std::string, std::string> 	_args;
		std::string						 	_content;
		std::string                         _filename;
};


#endif
