#include "Server.hpp"


/*----------------- CONSTRUCTORS ------------------------*/
Server::Server()
{
	return ;
}

Server::~Server()
{
	std::cout << "end server" << std::endl;
}
/*-------------------------------------------------------*/

int	Server::start_server(void)
{
	/* Creation du socket */
	if ((_socket = socket(AF_UNSPEC, SOCK_STREAM, 0)) == -1)
		return (server_error("Error when initialise socket"));



	return (1);
}

int	Server::server_error(const std::string error_message) const
{
	std::cout << error_message << std::endl;
	return (0);
}