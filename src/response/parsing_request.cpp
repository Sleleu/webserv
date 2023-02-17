#include "../../header/response/response.hpp"
#include "../../header/parser/parser.hpp"
#include "../../header/utils/color.hpp"


std::map< std::string, std::vector< std::string > > getLocationInfo( HttpRequest & request,\
std::map< std::string, std::map< std::string, std::vector<std::string> > > & serverMap )
{

	std::map< std::string, std::vector< std::string > > locationInfo;

	std::vector<std::string> locationList = request.getLocation();
	std::vector<std::string>::const_iterator it = locationList.begin();
	for (; it != locationList.end(); it ++)
	{
		if (serverMap.find(*it) != serverMap.end())
		{
			locationInfo = serverMap[*it];
			break;
		}
	}
	if (it == locationList.end())
		locationInfo = serverMap["def"];
	return locationInfo;

}
