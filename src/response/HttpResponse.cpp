#include "../../header/response/HttpResponse.hpp"
#include "../../header/response/response.hpp"
#include "../../header/utils/colors.hpp"

HttpResponse::HttpResponse()
{
	_controlData["version"] = "HTTP/1.1"; //Forcémment ?
	_controlData["code"] = "200";
	_controlData["status"] = "OK";
	_headers["server"] = "webserv";
	_headers["content-type"] = "text/html";
	_headers["content-length"] = "0";
	directoryListing = 0;
	_errorPath = "";
}

void HttpResponse::setResponseInfo(HttpRequest const & request, std::map< std::string, std::vector< std::string > > & serverMap)
{
	_headers["server"] = serverMap["server_name"][0];
	if (!request.parsing)
	{
		_controlData["code"] = "400";
		_controlData["status"] = "Bad Request";
		_errorPath = "./html" + serverMap["error"][0];
		return ;
	}

	_root = "./html" + serverMap["root"][0];
	_root = (_root[_root.length() -1] == '/') ? _root : _root + "/";

	_targetPath = (serverMap["root"][0] == "/") ? \
		"./html" + request.getTarget() : "./html" + serverMap["root"][0] + request.getTarget();

	if (_targetPath.find("_IMAGE_") != std::string::npos)
		_targetPath = "./html/image" + _targetPath.substr(_targetPath.find("_IMAGE_") + 7);

	if (serverMap["redirect"].size() == 2)
		redirectTargetPath(serverMap["redirect"][0], serverMap["redirect"][1]); // code redirected ?
	if (isDirectory())
	{
		if (serverMap["directory_listing"][0] == "on") // Pas certain de cette partie
			directoryListing = 1;
		else
		{
			_targetPath = (_targetPath[_targetPath.length() -1] == '/') ?\
				_targetPath + serverMap["default_file"][0] : _targetPath + "/" + serverMap["default_file"][0];
		}
	}

	_errorConf = serverMap["error"];
	// _errorPath = "./html" + serverMap.find("error")->second[0];

	setUpload(serverMap);
	setCgi(request, serverMap);
}

bool	HttpResponse::findInCgiBin()
{
	std::string inCgiBin = "./html/cgi-bin/" + _targetPath.substr(_targetPath.find_last_of('/') + 1);
	const char* path =inCgiBin.c_str();
	struct stat s;

	if (stat(path, &s) == 0)
	{
		_targetPath = inCgiBin;
		return 1;
	}
	return 0;
	// {
	// 	if (S_ISREG(s.st_mode))
	// }
}

void	HttpResponse::setUpload(std::map< std::string, std::vector< std::string > > & serverMap)
{
	std::vector<std::string> const upload_file = serverMap["upload_file"];
	_isUpload = (upload_file[0] == "on") ? "on":"off";
	_uploadPath = (upload_file.size() >= 2) ? upload_file[1]:"";
	std::cout << BOLDMAGENTA << "\nUpload PATH : " << _uploadPath << RESET << std::endl;
}

void	HttpResponse::setCgi(HttpRequest const & request,\
std::map< std::string, std::vector< std::string > > & serverMap)
{
	(void) request;
	std::vector<std::string> const cgi = serverMap["cgi"];
	std::string const fileType = getTargetPath().substr(getTargetPath().find_last_of('.') + 1);
	_extension = fileType;

	_cgiPath = (fileType == "php") ? "./html/cgi-bin/php.cgi":"";
	for (std::vector<std::string>::const_iterator it = cgi.begin(); it != cgi.end() ; it++)
	{
		if (*it == fileType)
			if (it + 1 != cgi.end())
				_cgiPath = *(it + 1);
	}
	std::cout << BOLDMAGENTA << "CGI PATH : " << _cgiPath << RESET << std::endl;
}

void	HttpResponse::errorReturn()
{
	if (_errorConf.size() >= 2 && _errorConf.size() % 2 == 0)
	{
		for (std::vector<std::string>::const_iterator it = _errorConf.begin(); it != _errorConf.end() ; it += 2)
		{
			if (*it == _controlData["code"])
			{
				std::string errPath = "./html/" + *(it + 1);
				std::ifstream ifs(errPath.c_str());
				if (!ifs.is_open())
				{
					setError("500", "Internal Server Error");
					setBody(BODY_500);
					return ;
				}
				std::string errorFileContent;
				std::string tmp;
				while (std::getline(ifs, tmp))
					errorFileContent += tmp + "\n";
				ifs.close();
				setBody("\n" + errorFileContent);
				return ;
			}
		}
	}
}

void		HttpResponse::setHeader()
{
	std::string fileType = getTargetPath().substr(getTargetPath().find_last_of('.') + 1);
	std::string img = "ico png apng avif webp";
	if (img.find(fileType) != std::string::npos)			//un peu bancal
		_headers["content-type"] = "image/" + fileType;
	if (fileType == "html")
		_headers["content-type"] = "text/" + fileType;
}

std::string HttpResponse::getResponseString()
{
	std::string controlDataString = _controlData["version"] + " " \
		+ _controlData["code"] + " " + _controlData["status"] + "\n";

	_headers["content-length"] = (_body.size() == 0) ? "0" : toString(sizeof(char) * _body.size() - 2);

	std::string headersString;
	for (std::map<std::string, std::string>::const_iterator it = _headers.begin() ; \
		it != _headers.end() ; it ++)
	{
		if (it->second != "")
			headersString += it->first + ": " + it->second + "\n";
	}
	return (controlDataString + headersString + _body);
}

std::vector<std::string> HttpResponse::getPackets(map_server serverMap, std::string responseString, bool verbose)
{
	std::vector<std::string> packets;
	if (_body == "")
	{
		packets.push_back(responseString);
		return packets;
	}
	size_t maxBody = std::atoi(serverMap["body_size"][0].c_str());
	size_t headerSize = responseString.find("\n\n") + 1;
	std::string onePacket = responseString.substr(0, headerSize + 1)\
				+ responseString.substr(headerSize + 1, maxBody);
	size_t allPacketSize = onePacket.size();
	while (1)
	{
		packets.push_back(onePacket);
		onePacket = responseString.substr(allPacketSize, maxBody);
		if (allPacketSize >= responseString.size())
			break ;
		allPacketSize += onePacket.size();
	}

	if (verbose)
	{
		std::cout << BOLDBLUE << "Packets to send : " << RESET << std::endl;
		for (std::vector<std::string>::const_iterator it = packets.begin() ; it != packets.end() && verbose == true ; it++)
			std::cout << "|" << BLUE << *it << RESET;
		std::cout << "\n" <<BOLDBLUE << allPacketSize - headerSize << " bits SEND" << RESET << std::endl;
	}
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
std::string HttpResponse::getCgiPath() const { return _cgiPath; }
std::string HttpResponse::getUploadPath() const { return _uploadPath; }
std::string HttpResponse::getIsUpload() const { return _isUpload; }
std::string HttpResponse::getExtension() const { return _extension; }
std::string HttpResponse::getRoot() const { return _root; }

void		HttpResponse::setError(std::string code, std::string status) { setCode(code); setStatus(status); }
void		HttpResponse::setCode(std::string content) { _controlData["code"] = content; }
void		HttpResponse::setStatus(std::string content) { _controlData["status"] = content; }
void		HttpResponse::setBody(std::string src) { _body = src; }
