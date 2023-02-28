#include "../../header/response/response.hpp"
#include "../../header/parser/parser.hpp"
#include "../../header/utils/color.hpp"


std::string parseBody(HttpRequest & request)
{
	std::string const body = request.getBody();
	size_t boundary = body.find("------WebKitFormBoundary");
	if (boundary == std::string::npos)
		return body;

	std::string newBody = body.substr(boundary);

	std::string filename = newBody.substr(newBody.find("filename") + 10);
	filename = filename.substr(0, filename.find_first_of('"'));
	request.setFileName(filename);
	// std::cout << BOLDBLUE << filename << RESET << std::endl;

	newBody = newBody.substr(newBody.find("\r\n\r\n") + 4);
	newBody = newBody.substr(0, newBody.find("------WebKitFormBoundary") - 4);
	// std::cout << BOLDMAGENTA << newBody << RESET << std::endl;

	request.setContent(newBody);
	return newBody;
}

bool fileExist(std::string & pathString)
{
	const char* path = pathString.c_str();
	struct stat s;

	if (stat(path, &s) == 0)
			return 1;
	return 0;
}

std::map< std::string, std::vector< std::string > > getLocationInfo( HttpRequest & request,\
std::map< std::string, std::map< std::string, std::vector<std::string> > > & locationMap )
{

	std::map< std::string, std::vector< std::string > > locationInfo;

	std::vector<std::string> locationList = request.getLocation();
	std::vector<std::string>::const_iterator it = locationList.begin();
	for (; it != locationList.end(); it ++)
	{
		// std::cout << BOLDBLUE << it->substr(0, it->size() - 1) << RESET << std::endl;
		if (locationMap.find(*it) != locationMap.end())
		{
			request.setLocationBlocName(*it);
			locationInfo = locationMap[*it];
			break;
		}
		if (locationMap.find(it->substr(0, it->size() - 1)) != locationMap.end())
		{
			request.setLocationBlocName(it->substr(0, it->size() - 1));
			locationInfo = locationMap[it->substr(0, it->size() - 1)];
			break;
		}
	}
	if (it == locationList.end())
	{
		request.setLocationBlocName("/");
		locationInfo = locationMap["/"];
	}
	return locationInfo;
}

std::map< std::string, std::vector< std::string > > getServerMap (std::map< std::string, std::vector< std::string > > locationInfo,\
std::map< std::string, std::vector< std::string > > defaultMap)
{
	std::map< std::string, std::vector< std::string > > serverMap;

	for (std::map< std::string, std::vector< std::string > >::const_iterator itDef = defaultMap.begin(); itDef != defaultMap.end(); itDef++)
		serverMap[itDef->first] = itDef->second;

	for (std::map< std::string, std::vector< std::string > >::const_iterator itLoc = locationInfo.begin(); itLoc != locationInfo.end(); itLoc++)
		serverMap.find(itLoc->first)->second = itLoc->second;

	return serverMap;
}
