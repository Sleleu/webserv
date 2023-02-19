#include "../../header/response/HttpResponse.hpp"

HttpResponse::HttpResponse(HttpRequest const & request, std::map< std::string, std::vector< std::string > > & serverMap)
{
	_controlData["version"] = request.getVersion();
	_controlData["code"] = "200";
	_controlData["status"] = "OK";
	_headers["content-type"] = "text/html"; // <- dans la fonction SET HEADER avant l'envoi de la response
	_headers["content-length"] = "1000";  // <- dans la fonction SET HEADER avant l'envoi de la response
	_headers["server"] = serverMap["server_name"][0];

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

	std::string headersString;
	for (std::map<std::string, std::string>::const_iterator it = _headers.begin() ; \
		it != _headers.end() ; it ++)
	{
		if (it->second != "")
			headersString += it->first + ": " + it->second + "\n";
	}
	return (controlDataString + headersString + _body);
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