#include "../../header/response/response.hpp"
#include "../../header/parser/parser.hpp"
#include "../../header/utils/color.hpp"


std::string get_response( std::string const & requestMsg, Server::location_server & locationMap,\
Server::map_server defaultMap, bool verbose)
{
	if (verbose)
	{
		std::cout << BOLDYELLOW << "\nRequest Message :\n" << RESET << std::endl;
		std::cout << YELLOW << requestMsg << RESET << std::endl;
		std::cout << BOLDWHITE << "\nConstructing Response...\n" << RESET << std::endl;
	}

	HttpRequest request;
	HttpResponse response;
	std::map< std::string, std::vector< std::string > > locationInfo;
	std::map< std::string, std::vector< std::string > > serverMap;

	try
	{
		std::cout << "\nReading Request:";
		request.setRequestInfo(requestMsg);
		locationInfo = getLocationInfo(request, locationMap); //probleme de '/' dans le parsing
		serverMap = getServerMap(locationInfo, defaultMap);
		request.checkParsing();
		std::cout << " [" << BOLDGREEN << "OK" << RESET << "]";
		std::cout << YELLOW << " " << request.getTarget()  << RESET << std::endl;

		//
		if (verbose)
		{
			std::cout << BOLDBLUE << "\nLOCATION INFORMATIONS :" << RESET << std::endl;
			for (std::map< std::string, std::vector< std::string > >::const_iterator itServ = serverMap.begin();\
			itServ != serverMap.end(); itServ++)
			{
				std::cout << itServ->first << " -> ";
				for (std::vector< std::string >::const_iterator itVec = itServ->second.begin();\
				itVec != itServ->second.end(); itVec++)
				{
					std::cout << *itVec << " ";
				}
				std::cout << std::endl;
			}
		}
		try
		{
			std::cout << "Parsing Request :";
			request.checkParsing();
			std::cout << " [" << BOLDGREEN << "OK" << RESET << "]" << std::endl;
			std::cout << "Constructing Response :";
			response.setResponseInfo(request, serverMap);
			std::cout << " [" << BOLDGREEN << "OK" << RESET << "]" << std::endl;
			if (verbose)
			{
				std::cout << "Target Path : '" << BLUE <<response.getTargetPath() << RESET << "'"<< std::endl;
				std::cout << "Error Path : '" << RED << response.getErrorPath() << RESET << "'"<< std::endl;
			}
			std::cout << "Setting Headers :";
			response.setHeader();
			std::cout << " [" << BOLDGREEN << "OK" << RESET << "]" << std::endl;
			std::cout << "Executing method:";
			acceptMethod(request, response, serverMap);
			std::cout << " [" << BOLDGREEN << "OK" << RESET << "] ";
			std::cout << BOLDGREEN << response.getCode() << " " << response.getStatus() << "\n" << RESET << std::endl;
			std::cout << std::endl;
		}
		catch(const HttpResponse::RedirectException e)
		{
			std::cout << " [" << BOLDGREEN << "OK" << RESET << "] ";
			std::cout << BOLDGREEN << response.getCode() << " " << response.getStatus() << "\n" << RESET << std::endl;
		}
		catch(const std::exception& e)
		{
			std::cout << " [" << BOLDRED << "KO" << RESET << "] ";
			response.errorReturn();
			std::cout << BOLDRED << response.getCode() << " " << response.getStatus() << "\n" << RESET << std::endl;
		}
	}
	catch(const std::exception& e) // On ne peut pas rediriger les erreurs de request !
	{
		request.parsing = 0;
		response.setError("400", "Bad Request");
		response.setBody(BODY_400);

		std::cout << " [" << BOLDRED << "KO" << RESET << "] " << std::endl;
	}

	std::string responseString = response.getResponseString();
	std::vector<std::string> packetsToSend;
	if (request.parsing)
		packetsToSend = response.getPackets(serverMap, responseString, verbose);
	else
		packetsToSend.push_back(responseString);
	if (verbose)
		std::cout << BOLDWHITE << "\n\n-- RESPONSE --\n\n" << RESET << responseString << std::endl;
	return (responseString); // il faudra return "packetsToSend"
}
