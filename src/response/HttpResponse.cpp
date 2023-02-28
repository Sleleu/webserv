#include "../../header/response/HttpResponse.hpp"
#include "../../header/response/response.hpp"
#include "../../header/utils/colors.hpp"

//controlDataString + headersString + _body

HttpResponse::HttpResponse() : _body("")
{
	_controlData["version"] = "HTTP/1.1";
	_controlData["code"] = "200";
	_controlData["status"] = "OK";
	_headers["Server"] = "webserv";
	_headers["Content-type"] = "text/html";
	_headers["Content-length"] = "0";
	directoryListing = 0;
	_errorPath = "";
}

void HttpResponse::setResponseInfo(HttpRequest & request, std::map< std::string, std::vector< std::string > > & serverMap)
{
	_errorConf = serverMap["error"];

	_headers["Server"] = serverMap["server_name"][0];
	if (!request.parsing)
	{
		_controlData["code"] = "400";
		_controlData["status"] = "Bad Request";
		_errorPath = "./html" + serverMap["error"][0];
		return ;
	}
	if (static_cast<long>(request.getBody().size() - 2) > std::atol(serverMap.find("body_size")->second[0].c_str()))
	{
		setError("413", "Payload Too Large");
		setBody(BODY_413);
		throw HttpResponse::BodySizeException();
	}

	// std::cout << BOLDMAGENTA << request.getLocationBlocName() << RESET << std::endl;
	//TARGETPATH
	_root = "./html" + serverMap["root"][0];
	_root = (_root[_root.length() -1] == '/') ? _root : _root + "/";

	if (!fileExist(_root))
	{
		setError("404", "Not Found");
		setBody(BODY_404);
		throw std::exception();
	}

	_targetPath = _root.substr(0, _root.size() - 1) + request.getTarget();
	std::string bloc = request.getLocationBlocName();
	bloc = (bloc[bloc.length() -1] == '/') ? bloc : bloc + "/";
	bloc = bloc.substr(0, bloc.size() - 1);
	_targetPath.erase(_targetPath.find(bloc), bloc.size());
	//

	if (_targetPath.find("_IMAGE_") != std::string::npos)
		_targetPath = "./html/image" + _targetPath.substr(_targetPath.find("_IMAGE_") + 7);

	if (serverMap["redirect"].size() != 0 && serverMap["redirect"][0] != "")
		redirectTargetPath(serverMap["redirect"][0]);
	if (isDirectory())
	{
		_targetPath = (_targetPath[_targetPath.length() -1] == '/') ? _targetPath : _targetPath + "/";
		std::string defaultPage = (_targetPath[_targetPath.length() -1] == '/') ?\
			_targetPath + serverMap["default_file"][0] : _targetPath + "/" + serverMap["default_file"][0];
		if (fileExist(defaultPage))
			_targetPath = defaultPage;
		else
		{
			if (serverMap["directory_listing"][0] == "on")
				directoryListing = 1;
			else
			{
				setError("403", "Forbidden");
				setBody(BODY_403);
				throw std::exception();
			}
		}
	}
	setUpload(serverMap);
	setCgi(request, serverMap);
	_accept = request.getHeader("Accept");
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
}

void	HttpResponse::setUpload(std::map< std::string, std::vector< std::string > > & serverMap)
{
	std::vector<std::string> const upload_file = serverMap["upload_file"];
	_isUpload = (upload_file[0] == "on") ? "on":"off";
	_uploadPath = (upload_file.size() >= 2) ? upload_file[1]:"";
	// std::cout << BOLDMAGENTA << "\nUpload PATH : " << _uploadPath << RESET << std::endl;
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
				_cgiPath = _root + "cgi-bin/" + *(it + 1);
	}
	// std::cout << BOLDMAGENTA << "CGI PATH : " << _cgiPath << RESET << std::endl;
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
	std::string img = "ico png apng avif jpeg";
	if (img.find(fileType) != std::string::npos)			//un peu bancal
		_headers["Content-type"] = "image/" + fileType;
	if (fileType == "html")
		_headers["Content-type"] = "text/" + fileType;
	if (_accept == "")
		return ;
	std::string const typeOfFile = _headers["Content-type"];
	if (_accept.find(typeOfFile) == std::string::npos)
	{
		std::string const subType = typeOfFile.substr(0, typeOfFile.find("/")) + "/*";
		if (_accept.find(subType) != std::string::npos || _accept.find("*/*") != std::string::npos)
			return ;
		setBody(BODY_406);
		setError("406", "Not Acceptable");
		throw std::exception();
	}
}

std::string HttpResponse::getResponseString()
{
	std::string controlDataString = _controlData["version"] + " " \
		+ _controlData["code"] + " " + _controlData["status"] + "\n";

	_headers["Content-length"] = (_body.size() == 0) ? "0" : toString(sizeof(char) * _body.size() - 2);

	std::string headersString = "";
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

void	HttpResponse::redirectTargetPath(std::string url)
{
	_headers["Location"] = url;
	setError("301", "Moved Permanently");
	setBody(std::string(BODY_301) + "<p>The document has moved <a href=" + url + ">here</a>.</p>\n</body></html>\n");
	throw HttpResponse::RedirectException();
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
