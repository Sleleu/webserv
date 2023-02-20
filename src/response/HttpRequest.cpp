#include "../../header/response/HttpRequest.hpp"

HttpRequest::HttpRequest(std::string const requestMsg)
{
	parsing = 1;
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

	parsing = (_controlData.size() == 3) ? 1 : 0;
}

std::vector<std::string> HttpRequest::getLocation() const
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

void	HttpRequest::checkParsing() const
{
	if (!parsing)
		throw std::exception();
}

std::string HttpRequest::getMethod() const {return _controlData[0];}
std::string HttpRequest::getTarget() const {return _controlData[1];}
std::string HttpRequest::getVersion() const {return _controlData[2];}
std::string HttpRequest::getHeader(std::string key) {return _headerMap[key];}
std::string HttpRequest::getBody() const {return _body;}
void		HttpRequest::setLocationBlocName(std::string src) { _locationBlocName = src; }
std::string HttpRequest::getLocationBlocName() const { return _locationBlocName; }
