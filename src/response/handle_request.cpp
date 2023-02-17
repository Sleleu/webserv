#include "../../header/response/response.hpp"
#include "../../header/parser/parser.hpp"
#include "../../header/utils/color.hpp"


std::string handle_request( std::string const & requestMsg,\
std::map< std::string, std::map< std::string, std::vector<std::string> > > & serverMap )
{
	std::cout << BOLDWHITE << "\nConstructing Response...\n" << RESET << std::endl;

	// ---------------- REQUEST
	std::cout << "Parsing Request:";
	HttpRequest request(requestMsg);
	std::map< std::string, std::vector< std::string > > locationInfo;
	try
	{
		locationInfo = getLocationInfo(request, serverMap);
		std::cout << " [" << BOLDGREEN << "OK" << RESET << "]" << std::endl;
	}
	catch(const std::exception& e)
	{
		std::cout << " [" << BOLDRED << "KO" << RESET << "]" << std::endl;
	}

	// ---------------- RESPONSE
	HttpResponse response(request, locationInfo);
	std::cout << "Executing method:";
	try
	{
		acceptMethod(request, response, locationInfo);
		std::cout << " [" << BOLDGREEN << "OK" << RESET << "]" << std::endl;
	}
	catch(const std::exception& e)
	{
		std::cout << " [" << BOLDRED << "KO" << RESET << "]" << std::endl;
	}

	std::cout << BOLDWHITE << "\n\n-- RESPONSE --\n\n" << RESET << response.getResponseString() << std::endl;
	return (response.getResponseString());
}



// --------------------------------- FOR TEST --------------------------------------------
void simul_request()
{
	std::string const requestMsg = "\
GET /files/test.html HTTP/1.1\n\
Host: localhost:8080\n\
Connection: keep-alive\n\
Cache-Control: max-age=0\n\
Upgrade-Insecure-Requests: 1\n\
User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_13_3) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/65.0.3325.162 Safari/537.36\n\
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8\n\
DNT: 1\n\
Accept-Encoding: gzip, deflate, br\n\
Accept-Language: en-US,en;q=0.9\n\
\n\
<html>\n\
	<head>\n\
		<title>Ma page : d'exemple</title>\n\
	</head>\n\
	<body>\n\
	Voici le contenu\n\
	</body>\n\
</html>";

	std::map< std::string, std::map< std::string, std::vector<std::string> > > serverMap;
	serverMap["/"];
	serverMap["/files/"] ;
	serverMap["/configz/"] ;

	serverMap["def"]["server_name"].push_back("webserv");
	serverMap["def"]["listen"].push_back("8080");
	serverMap["def"]["root"].push_back("/html/");
	serverMap["def"]["body_size"].push_back("1000");
	// serverMap["def"]["redirect"].push_back("webserv"); marche pas encore
	serverMap["def"]["error"].push_back("/error.html");
	serverMap["def"]["method"].push_back("GET");
	serverMap["def"]["method"].push_back("POST");
	serverMap["def"]["method"].push_back("DELETE");
	serverMap["def"]["directory_listing"].push_back("on");
	serverMap["def"]["cgi"].push_back("python");
	serverMap["def"]["cgi"].push_back("perle");
	serverMap["def"]["default_file"].push_back("/html/index.html");
	serverMap["def"]["upload_file"].push_back("off");

	serverMap["/"]["server_name"].push_back("webserv");
	serverMap["/"]["listen"].push_back("8080");
	serverMap["/"]["root"].push_back("/html/");
	serverMap["/"]["body_size"].push_back("1000");
	// serverMap["/"]["redirect"].push_back("webserv"); marche pas encore
	serverMap["/"]["error"].push_back("/error.html");
	serverMap["/"]["method"].push_back("GET");
	serverMap["/"]["method"].push_back("POST");
	serverMap["/"]["method"].push_back("DELETE");
	serverMap["/"]["directory_listing"].push_back("on");
	serverMap["/"]["cgi"].push_back("python");
	serverMap["/"]["cgi"].push_back("perle");
	serverMap["/"]["default_file"].push_back("/html/index.html");
	serverMap["/"]["upload_file"].push_back("off");

	serverMap["/files/"]["server_name"].push_back("webserv version 2");
	serverMap["/files/"]["listen"].push_back("8080");
	serverMap["/files/"]["root"].push_back("/html/");
	serverMap["/files/"]["body_size"].push_back("1000");
	// serverMap["/files/"]["redirect"].push_back("webserv"); marche pas encore
	serverMap["/files/"]["error"].push_back("/error.html");
	serverMap["/files/"]["method"].push_back("GET");
	serverMap["/files/"]["method"].push_back("DELETE");
	serverMap["/files/"]["directory_listing"].push_back("on");
	serverMap["/files/"]["cgi"].push_back("python");
	serverMap["/files/"]["default_file"].push_back("/html/index.html");
	serverMap["/files/"]["upload_file"].push_back("on");

	serverMap["/configz/"]["server_name"].push_back("webserv");
	serverMap["/configz/"]["listen"].push_back("8080");
	serverMap["/configz/"]["root"].push_back("/html/");
	serverMap["/configz/"]["body_size"].push_back("1000");
	// serverMap["/configz/"]["redirect"].push_back("webserv"); marche pas encore
	serverMap["/configz/"]["error"].push_back("/error.html");
	serverMap["/configz/"]["method"].push_back("GET");
	serverMap["/configz/"]["method"].push_back("POST");
	serverMap["/configz/"]["directory_listing"].push_back("off");
	serverMap["/configz/"]["cgi"].push_back("perle");
	serverMap["/configz/"]["default_file"].push_back("/html/index.html");
	serverMap["/configz/"]["upload_file"].push_back("off");

	handle_request(requestMsg, serverMap);
}
