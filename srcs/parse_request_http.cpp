# include <iostream>
# include <map>
# include <string>
# include <vector>
# include <sstream>
# include <cstddef>
# include "httpRequest.hpp"

void httpMessage(std::string const requestMsg)
{
	HttpRequest requestInfo(requestMsg);
	std::string response;

	
}

int main ()
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

	httpMessage(requestMsg);
}
