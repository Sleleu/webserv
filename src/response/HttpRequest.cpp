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
	if (_controlData[2].find("HTTP/1.1") == std::string::npos) // NON TESTE
		throw HttpRequest::HttpVersion();
	if (_controlData[1][0] != '/') // NON TESTE
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

void		HttpRequest::setTarget()
{
	std::string	argsString;
	if (getMethod() == "GET")
	{
		std::string const oldTarget = getTarget();
		size_t const argsBegin = oldTarget.find("?");
		if (argsBegin == std::string::npos)
			return ;
		_controlData[1] = oldTarget.substr(0, argsBegin);
		argsString = oldTarget.substr(argsBegin + 1);
	}
	else if (getMethod() == "POST")
		argsString = _body.substr(2);
	else
		return ;
	_content = argsString;

	while (argsString != "")
	{
		std::string const line = argsString.substr(0, argsString.find("&"));
		_args[line.substr(0, line.find("="))] = line.substr(line.find("=") + 1);
		argsString = (argsString.find("&") != std::string::npos) ? argsString.substr(argsString.find("&") + 1):"";
	}

	// std::cout << std::endl;
	// std::cout << "CONTENT = " << _content << std::endl;
	// std::cout << "ARGS MAP" << std::endl;
	// for (std::map<std::string, std::string>::const_iterator it = _args.begin() ; it != _args.end() ; it++)
	// 	std::cout << it->first << " -> "<< it->second << std::endl;
	// std::cout << std::endl;
}

std::vector<std::string> HttpRequest::getLocation() const
{
	std::vector<std::string> location;
	std::string target = getTarget();

	target = (target[target.size() - 1] == '/') ? target : target + "/";
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



void	HttpRequest::setBody(std::string src) { _body = src; }

std::string HttpRequest::getFileName() const { return _filename; }
void	HttpRequest::setFileName(std::string src) { _filename = src; }

std::string HttpRequest::getContent() const { return _content; }
void	HttpRequest::setContent(std::string src) { _content = src; }

std::string HttpRequest::getMethod() const { return _controlData[0]; }

std::string HttpRequest::getTarget() const { return _controlData[1]; }

std::string HttpRequest::getVersion() const { return _controlData[2]; }

std::string HttpRequest::getHeader(std::string key) { return _headerMap[key]; }

std::string HttpRequest::getBody() const { return _body; }

void	HttpRequest::setLocationBlocName(std::string src) {  _locationBlocName = src; }

std::string HttpRequest::getLocationBlocName() const { return _locationBlocName; }

std::map<std::string, std::string> HttpRequest::getArgs() const { return _args; }
