#include "../../header/response/HttpResponse.hpp"
#include "../../header/response/response.hpp"
#include "../../header/utils/colors.hpp"

HttpResponse::HttpResponse(HttpRequest const & request, std::map< std::string, std::vector< std::string > > & serverMap)
{
	// _controlData["version"] = request.getVersion();
	_headers["server"] = serverMap["server_name"][0];
	if (!request.parsing)
	{
		_controlData["version"] = "HTTP/1.1"; //Forcémment ?
		_controlData["code"] = "400";
		_controlData["status"] = "Bad Request";
		_errorPath = "./html" + serverMap["error"][0];
		return ;
	}
	_controlData["version"] = "HTTP/1.1"; //Forcémment ?
	_controlData["code"] = "200";
	_controlData["status"] = "OK";
	_headers["content-type"] = "text/html";
	_headers["content-length"] = "1000";

	_targetPath = (serverMap["root"][0] == "/") ? \
		"./html" + request.getTarget() : "./html" + serverMap["root"][0] + request.getTarget();
	if (serverMap["redirect"].size() == 2)
		redirectTargetPath(serverMap["redirect"][0], serverMap["redirect"][1]);
	if (isDirectory())
		_targetPath = (_targetPath[_targetPath.length() -1] == '/') ?\
			_targetPath + serverMap["default_file"][0] : _targetPath + "/" + serverMap["default_file"][0];

	_errorPath = "./html" + serverMap["error"][0];

	canUpload = (serverMap["upload_file"][0] == "on") ? 1 : 0;
}

void	HttpResponse::errorReturn()
{
	std::string errPath = this->getErrorPath();
	std::ifstream ifs(errPath.c_str());
	if (!ifs.is_open())
	{
		setError("500", "Internal Server Error"); //voir quelle page on display dans ce cas là
		return ;
	}

	std::string errorFileContent;
	std::string tmp;
	while (std::getline(ifs, tmp))
		errorFileContent += tmp + "\n";
	ifs.close();
	setBody("\n" + errorFileContent);
}

std::string HttpResponse::getResponseString()
{
	std::string controlDataString = _controlData["version"] + " " \
		+ _controlData["code"] + " " + _controlData["status"] + "\n";

	//SET HEADERS
	_headers["content-length"] = toString(sizeof(char) * _body.size() - 2);

	std::string fileType = getTargetPath().substr(getTargetPath().find_last_of('.'));
	// if (fileType == ".ico")
	// 	_headers["content-type"] = "image/*";
	// _headers["content-type"] = getTargetPath().substr(getTargetPath().find_last_of('.') + 1); // PAS SUR

	std::string headersString;
	for (std::map<std::string, std::string>::const_iterator it = _headers.begin() ; \
		it != _headers.end() ; it ++)
	{
		if (it->second != "")
			headersString += it->first + ": " + it->second + "\n";
	}
	return (controlDataString + headersString + _body);
}

std::vector<std::string> HttpResponse::getPackets(map_server serverMap, std::string responseString)
{
	std::vector<std::string> packets;
	if (_body == "")
	{
		packets.push_back(responseString);
		return packets;
	}
	size_t maxBody = std::atoi(serverMap["body_size"][0].c_str());
	size_t headerSize = responseString.find("\n\n");
	std::string onePacket = responseString.substr(0, headerSize)\
				+ responseString.substr(headerSize + 1, maxBody);
	size_t allPacketSize = onePacket.size();
	while (allPacketSize < responseString.size())
	{
		packets.push_back(onePacket);
		onePacket = responseString.substr(allPacketSize, maxBody);
		if (allPacketSize > responseString.size())
			break ;
		allPacketSize += onePacket.size();
	}

	std::cout << BOLDBLUE << "Packets to send : " << RESET << std::endl;
	for (std::vector<std::string>::const_iterator it = packets.begin() ; it != packets.end() ; it++)
		std::cout << "--> " << BLUE << *it << RESET << std::endl;
	std::cout << BOLDBLUE << allPacketSize - headerSize << " bits SEND" << RESET << std::endl;

	return packets;
}


void	HttpResponse::redirectTargetPath(std::string first, std::string second)
{
	size_t pos = _targetPath.find(first);
	if (pos != std::string::npos)
		_targetPath.replace(pos, first.length(), second);
}

bool	HttpResponse::isDirectory()
{
	const char* path =_targetPath.c_str();
	struct stat s;

	if (stat(path, &s) == 0)
	{
		if (S_ISDIR(s.st_mode))
			return 1;
	}
	return 0;
}

std::string HttpResponse::getErrorPath() const { return _errorPath; }
std::string HttpResponse::getCode() { return _controlData["code"]; }
std::string HttpResponse::getStatus() { return _controlData["status"]; }
std::string HttpResponse::getTargetPath() const { return _targetPath; }
void		HttpResponse::setError(std::string code, std::string status) { setCode(code); setStatus(status); }
void		HttpResponse::setCode(std::string content) { _controlData["code"] = content; }
void		HttpResponse::setStatus(std::string content) { _controlData["status"] = content; }
void		HttpResponse::setBody(std::string src) { _body = src; }
