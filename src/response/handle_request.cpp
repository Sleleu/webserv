#include "../../header/response/response.hpp"
#include "../../header/parser/parser.hpp"

std::string handle_request(std::string const & requestMsg)
{
	HttpRequest request(requestMsg);
	HttpResponse response(request);

	void (*useMethod[3])(HttpRequest const & request, HttpResponse & response) = {methodGET, methodPOST, methodDELETE};
	std::string acceptedMethods[] = {"GET", "POST", "DELETE"};
	for (int i = 0; i < 3; i++)
		if (acceptedMethods[i] == request.getMethod())
			useMethod[i](request, response);
	std::cout << "-- RESPONSE --\n\n" << response.getResponseString() << std::endl;
	std::cout << "method -- " << request.getMethod() << std::endl;
	return (response.getResponseString());
}

void simul_request()
{
	std::string const requestMsg = "\
GET /index.html HTTP/1.1\n\
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
	serverMap["def"];
	serverMap["/"];
	serverMap["/files"] ;

	serverMap["def"]["server_name"].push_back("webserv");
	serverMap["def"]["listen"].push_back("webserv");
	serverMap["def"]["root"].push_back("webserv");
	serverMap["def"]["body_size"].push_back("webserv");
	serverMap["def"]["redirect"].push_back("webserv");
	serverMap["def"]["server_name"].push_back("webserv");
	serverMap["def"]["server_name"].push_back("webserv");
	serverMap["def"]["server_name"].push_back("webserv");
	serverMap["def"]["server_name"].push_back("webserv");
	serverMap["def"]["server_name"].push_back("webserv");
	serverMap["def"]["server_name"].push_back("webserv");
	handle_request(requestMsg);
}
