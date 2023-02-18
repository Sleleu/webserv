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
		HttpRequest(std::string const requestMsg)
		{

			std::string const firstHeaderLine(requestMsg, 0, requestMsg.find("\n"));
			std::string substringTmp;
			std::stringstream ss(firstHeaderLine);
			while (ss >> substringTmp)
				_controlData.push_back(substringTmp);

			std::istringstream streamMap(requestMsg);
			std::string headerField;
			while (std::getline(streamMap, headerField))
			{
				if (headerField == "")
					break ;
				size_t separator = headerField.find_first_of(':');
				if (separator != std::string::npos)
				{
					std::string key(headerField, 0, separator);
					std::string content(headerField, separator + 2);
					_headerMap[key] = content;
				}
			}

			size_t bodyBegin = requestMsg.find("\n\n");
			if (bodyBegin != std::string::npos)
				_body = requestMsg.substr(bodyBegin + 2);
		}

		HttpRequest & operator=(HttpRequest & rhs)
		{
			_controlData = rhs._controlData;
			_headerMap = rhs._headerMap;
			_body = rhs._body;
			return (*this);
		}

		std::vector<std::string> getLocation() const
		{
			std::vector<std::string> location;
			std::string target = getTarget();

			location.push_back(target);
			while (target != "")
			{
				target = target.substr(0, target.find_last_of('/'));
				location.push_back(target + "/");
			}
			return location;
		}

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
