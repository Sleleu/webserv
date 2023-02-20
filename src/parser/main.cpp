#include "../../header/parser/parser.hpp"
#include "../../header/utils/color.hpp"
#include "../../header/response/response.hpp"
#include "../../header/server/Server.hpp"

int main(int argc, char **argv)
{
	(void)argc;
	(void)argv;
	
	Parser parser(argv[1]);
	// std::cout << parser << std::endl;
	Server server = Server(parser.getBigVector()[0], parser.getBigLocation()[0]);
	
	if (!server.init_server())
		return (1);
	server.start_server();
	//simul_request(parser.getBigLocation()[0], parser.getBigVector()[0]);
}
