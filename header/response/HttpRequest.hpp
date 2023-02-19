#ifndef HTTPREQUEST_H
#define HTTPREQUEST_H

# include <iostream>
# include <map>
# include <string>
# include <vector>
# include <sstream>
# include <cstddef>

class HttpRequest
{
	public:
		HttpRequest(std::string const requestMsg);

		std::vector<std::string> getLocation() const;

		std::string getMethod() const {return _controlData[0];}
		std::string getTarget() const {return _controlData[1];}
		std::string getVersion() const {return _controlData[2];}
		std::string getHeader(std::string key) {return _headerMap[key];}
		std::string getBody() const {return _body;}
		void		setLocationBlocName(std::string src) { _locationBlocName = src; }
		std::string getLocationBlocName() const { return _locationBlocName; }


	private :
		std::vector<std::string>            _controlData;
		std::map<std::string, std::string>  _headerMap;
		std::string                         _body;
		std::string 						_locationBlocName;
};


#endif
