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
		std::map<std::string, std::string>	getArgs() const;
		void								setLocationBlocName(std::string src);
		std::string 						getLocationBlocName() const;

		void								checkParsing() const;
		bool								parsing;
	private :
		std::vector<std::string>            _controlData;
		std::map<std::string, std::string>  _headerMap;
		std::string                         _body;
		std::string 						_locationBlocName;
		std::string 						_target;
		std::map<std::string, std::string> 	_args;
};


#endif
