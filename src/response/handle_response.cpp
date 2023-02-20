#include "../../header/response/response.hpp"
#include "../../header/parser/parser.hpp"
#include "../../header/utils/color.hpp"


std::string get_response( std::string const & requestMsg, Server::location_server & locationMap,\
Server::map_server defaultMap )
{
	std::cout << BOLDYELLOW << "\nRequest Message :\n" << RESET << std::endl;
	std::cout << YELLOW << requestMsg << RESET << std::endl;
	std::cout << BOLDWHITE << "\nConstructing Response...\n" << RESET << std::endl;

	// ---------------- REQUEST ----------------
	HttpRequest request(requestMsg);
	std::map< std::string, std::vector< std::string > > locationInfo;
	std::map< std::string, std::vector< std::string > > serverMap;
	try
	{
		std::cout << "Reading Request:";
		locationInfo = getLocationInfo(request, locationMap);
		serverMap = getServerMap(locationInfo, defaultMap);
		request.checkParsing();
		std::cout << " [" << BOLDGREEN << "OK" << RESET << "]" << std::endl;

		//
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
		//
	}
	catch(const std::exception& e)
	{
		std::cout << " [" << BOLDRED << "KO" << RESET << "]" << std::endl;
	}

	// ---------------- RESPONSE --------------

	HttpResponse response(request, serverMap);
	try
	{
		std::cout << "\nParsing Request :";
		request.checkParsing();
		std::cout << " [" << BOLDGREEN << "OK" << RESET << "]" << std::endl;
		std::cout << "\nTarget Path : '" << BLUE <<response.getTargetPath() << RESET << "'"<< std::endl;
		std::cout << "Error Path : '" << RED << response.getErrorPath() << RESET << "'"<< std::endl;
		std::cout << "\nSetting Headers :";
		//SetHeaders ici
		std::cout << " [" << BOLDGREEN << "OK" << RESET << "]" << std::endl;
		std::cout << "Executing method:";
		acceptMethod(request, response, serverMap);
		std::cout << " [" << BOLDGREEN << "OK" << RESET << "]" << std::endl;
	}
	catch(const std::exception& e)
	{
		std::cout << " [" << BOLDRED << "KO" << RESET << "]" << std::endl;
		response.errorReturn();
		std::cout << BOLDRED << response.getCode() << " " << response.getStatus() << RESET << std::endl;
	}
	std::string responseString = response.getResponseString();
	std::cout << BOLDWHITE << "\n\n-- RESPONSE --\n\n" << RESET << responseString << std::endl;
	return (responseString);
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


// --------------------------------- FOR TEST --------------------------------------------
// void simul_request()
// {
// 	std::string const requestMsg = "\
// GET /files/test.html HTTP/1.1\n\
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

// 	std::map< std::string, std::map< std::string, std::vector<std::string> > > locationMap;
// 	locationMap["/"];
// 	locationMap["/files/"] ;
// 	locationMap["/configz/"] ;

// 	locationMap["def"]["server_name"].push_back("webserv");
// 	locationMap["def"]["listen"].push_back("8080");
// 	locationMap["def"]["root"].push_back("/html/");
// 	locationMap["def"]["body_size"].push_back("1000");
// 	// locationMap["def"]["redirect"].push_back("webserv"); marche pas encore
// 	locationMap["def"]["error"].push_back("/error.html");
// 	locationMap["def"]["method"].push_back("GET");
// 	locationMap["def"]["method"].push_back("POST");
// 	locationMap["def"]["method"].push_back("DELETE");
// 	locationMap["def"]["directory_listing"].push_back("on");
// 	locationMap["def"]["cgi"].push_back("python");
// 	locationMap["def"]["cgi"].push_back("perle");
// 	locationMap["def"]["default_file"].push_back("/html/index.html");
// 	locationMap["def"]["upload_file"].push_back("off");

// 	locationMap["/"]["server_name"].push_back("webserv");
// 	locationMap["/"]["listen"].push_back("8080");
// 	locationMap["/"]["root"].push_back("/html/");
// 	locationMap["/"]["body_size"].push_back("1000");
// 	// locationMap["/"]["redirect"].push_back("webserv"); marche pas encore
// 	locationMap["/"]["error"].push_back("/error.html");
// 	locationMap["/"]["method"].push_back("GET");
// 	locationMap["/"]["method"].push_back("POST");
// 	locationMap["/"]["method"].push_back("DELETE");
// 	locationMap["/"]["directory_listing"].push_back("on");
// 	locationMap["/"]["cgi"].push_back("python");
// 	locationMap["/"]["cgi"].push_back("perle");
// 	locationMap["/"]["default_file"].push_back("/html/index.html");
// 	locationMap["/"]["upload_file"].push_back("off");

// 	locationMap["/files/"]["server_name"].push_back("webserv version 2");
// 	locationMap["/files/"]["listen"].push_back("8080");
// 	locationMap["/files/"]["root"].push_back("/html/");
// 	locationMap["/files/"]["body_size"].push_back("1000");
// 	// locationMap["/files/"]["redirect"].push_back("webserv"); marche pas encore
// 	locationMap["/files/"]["error"].push_back("/error.html");
// 	locationMap["/files/"]["method"].push_back("GET");
// 	locationMap["/files/"]["method"].push_back("DELETE");
// 	locationMap["/files/"]["directory_listing"].push_back("on");
// 	locationMap["/files/"]["cgi"].push_back("python");
// 	locationMap["/files/"]["default_file"].push_back("/html/index.html");
// 	locationMap["/files/"]["upload_file"].push_back("on");

// 	locationMap["/configz/"]["server_name"].push_back("webserv");
// 	locationMap["/configz/"]["listen"].push_back("8080");
// 	locationMap["/configz/"]["root"].push_back("/html/");
// 	locationMap["/configz/"]["body_size"].push_back("1000");
// 	// locationMap["/configz/"]["redirect"].push_back("webserv"); marche pas encore
// 	locationMap["/configz/"]["error"].push_back("/error.html");
// 	locationMap["/configz/"]["method"].push_back("GET");
// 	locationMap["/configz/"]["method"].push_back("POST");
// 	locationMap["/configz/"]["directory_listing"].push_back("off");
// 	locationMap["/configz/"]["cgi"].push_back("perle");
// 	locationMap["/configz/"]["default_file"].push_back("/html/index.html");
// 	locationMap["/configz/"]["upload_file"].push_back("off");

// 	handle_request(requestMsg, locationMap);
// }