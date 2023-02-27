#include "../../header/response/response.hpp"
#include "../../header/parser/parser.hpp"
#include "../../header/utils/color.hpp"


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
		if (locationMap.find(*it) != locationMap.end())
		{
			request.setLocationBlocName(*it);
			locationInfo = locationMap[*it];
			break;
		}
	}
	if (it == locationList.end())
		locationInfo = locationMap["/"];
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
