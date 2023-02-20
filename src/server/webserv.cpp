#include <iostream>
#include <vector>
#include <map>
#include "../../header/server/Server.hpp"

int main (int argc, char **argv)
{
	if (argc != 3)
		return (std::cout << "Error bad arguments\n", 1);

	Server server = Server(argv[1], argv[2]);
	
	if (!server.init_server())
		return (1);
	server.start_server();
	return (0);
}