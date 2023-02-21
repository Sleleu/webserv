#include "../../header/response/response.hpp"
#include "../../header/parser/parser.hpp"
#include "../../header/utils/color.hpp"


std::string get_response( std::string const & requestMsg, Server::location_server & locationMap,\
Server::map_server defaultMap )
{
	std::cout << BOLDYELLOW << "\nRequest Message :" << RESET << std::endl;
	std::cout << YELLOW << requestMsg << RESET;

	HttpRequest request;
	HttpResponse response;
	std::map< std::string, std::vector< std::string > > locationInfo;
	std::map< std::string, std::vector< std::string > > serverMap;

	try
	{
		std::cout << "\nReading Request:";
		request.setRequestInfo(requestMsg);
		locationInfo = getLocationInfo(request, locationMap);
		serverMap = getServerMap(locationInfo, defaultMap);
		request.checkParsing();
		std::cout << " [" << BOLDGREEN << "OK" << RESET << "]" << std::endl;

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
		try
		{			
			std::cout << "\nParsing Request :";
			request.checkParsing();
			std::cout << " [" << BOLDGREEN << "OK" << RESET << "]" << std::endl;
			std::cout << "Constructing Response :";
			response.setResponseInfo(request, serverMap);
			std::cout << " [" << BOLDGREEN << "OK" << RESET << "]" << std::endl;
			std::cout << "Target Path : '" << BLUE <<response.getTargetPath() << RESET << "'"<< std::endl;
			std::cout << "Error Path : '" << RED << response.getErrorPath() << RESET << "'"<< std::endl;
			std::cout << "Setting Headers :";
			response.setHeader();
			std::cout << " [" << BOLDGREEN << "OK" << RESET << "]" << std::endl;
			std::cout << "Executing method:";
			acceptMethod(request, response, serverMap);
			std::cout << " [" << BOLDGREEN << "OK" << RESET << "]" << std::endl;
			std::cout << std::endl;
		}
		catch(const std::exception& e)
		{
			std::cout << " [" << BOLDRED << "KO" << RESET << "]" << std::endl;
			response.errorReturn();
			std::cout << BOLDRED << response.getCode() << " " << response.getStatus() << RESET << std::endl;
		}
	}
	catch(const std::exception& e)
	{
		request.parsing = 0;
		response.setError("400", "Bad Request");
		response.setBody(BODY_400);
		std::cout << " [" << BOLDRED << "KO" << RESET << "]" << std::endl;
	}

	std::string responseString = response.getResponseString();
	std::vector<std::string> packetsToSend;
	if (request.parsing)
		packetsToSend = response.getPackets(serverMap, responseString);
	else
		packetsToSend.push_back(responseString);
	std::cout << BOLDWHITE << "\n\n-- RESPONSE --\n\n" << RESET << responseString << std::endl;
	return (responseString); // il faudra return "packetsToSend"
}













// void simul_request(std::map< std::string, std::map< std::string, std::vector<std::string> > > & locationMap,\
// std::map < std::string, std::vector<std::string> > defaultMap )
// {
// 	std::string const requestMsg = "\
// GET /files/index.html HTTP/1.1\n\
// Host: localhost:8080\n\
// Connection: keep-alive\n\
// Cache-Control: max-age=0\n\
// Upgrade-Insecure-Requests: 1\n\
// User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_13_3) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/65.0.3325.162 Safari/537.36\n\
// Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8\n\
// DNT: 1\n\
// Accept-Encoding: gzip, deflate, br\n\
// Accept-Language: en-US,en;q=0.9\n\
// \n\
// <html>\n\
// 	<head>\n\
// 		<title>Ma page : d'exemple</title>\n\
// 	</head>\n\
// 	<body>\n\
// 	Voici le contenu\n\
// 	</body>\n\
// </html>";


// 	handle_response(requestMsg, locationMap, defaultMap);
// }
