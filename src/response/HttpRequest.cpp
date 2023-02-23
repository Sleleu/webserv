#include "../../header/response/HttpRequest.hpp"

HttpRequest::HttpRequest(): parsing(1), _body(""){}

void HttpRequest::setRequestInfo(std::string const requestMsg)
{
	std::string const firstHeaderLine(requestMsg, 0, requestMsg.find("\n"));
	std::string substringTmp;
	std::stringstream ss(firstHeaderLine);
	while (ss >> substringTmp)
		_controlData.push_back(substringTmp);
	if (_controlData.size() != 3 || _controlData[2].find("HTTP/") == std::string::npos)
		throw std::exception();

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

	size_t bodyBegin = requestMsg.find("\r\n\r\n");
	if (bodyBegin != std::string::npos)
		_body = requestMsg.substr(bodyBegin + 2);

	setTarget();
}

void		HttpRequest::setTarget() //PARSING PB '?'
{
	std::string	argsString;
	if (getMethod() == "GET")
	{
		std::string const oldTarget = getTarget();
		size_t const argsBegin = oldTarget.find("?");
		if (argsBegin == std::string::npos)
			return ;
		_controlData[1] = oldTarget.substr(0, argsBegin);
		argsString = oldTarget.substr(argsBegin);
	}
	else if (getMethod() == "POST")
		argsString = _body;
	else
		return ;
	_content = argsString;
	for (size_t pos = 0 ; pos != std::string::npos;)
	{
		size_t beginArg = pos;
		pos = argsString.find("&", pos + 1);
		std::string oneArg = argsString.substr(beginArg + 1, pos - 1); //Segfault peut etre
		_args[oneArg.substr(0, oneArg.find("="))] = oneArg.substr(oneArg.find("=") + 1);
	}
	std::cout << std::endl;
	for (std::map<std::string, std::string>::const_iterator it = _args.begin() ; it != _args.end() ; it++)
		std::cout << it->first << " -> "<< it->second << std::endl;
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

std::string HttpRequest::getContent() const { return _content; }

std::string HttpRequest::getMethod() const { return _controlData[0]; }

std::string HttpRequest::getTarget() const { return _controlData[1]; }

std::string HttpRequest::getVersion() const { return _controlData[2]; }

std::string HttpRequest::getHeader(std::string key) { return _headerMap[key]; }

std::string HttpRequest::getBody() const { return _body; }

void	HttpRequest::setLocationBlocName(std::string src) {  _locationBlocName = src; }

std::string HttpRequest::getLocationBlocName() const { return _locationBlocName; }

std::map<std::string, std::string> HttpRequest::getArgs() const { return _args; }
