#include "Server.hpp"


/*----------------- CONSTRUCTORS ------------------------*/
Server::Server()
{
	return ;
}


Server::Server(std::string ip, int port) : _ip_str(ip), _port(port)
{
	std::cout << "Server assign constructor called" << std::endl;
	std::memset(&_addrinfo, 0, sizeof(_addrinfo)); // initialiser tous les membres a 0
	this->_addrinfo.ai_family = AF_INET; // Pour IPv4
	this->_addrinfo.ai_socktype = SOCK_STREAM; // Pour TCP
	this->_addrinfo.ai_flags = AI_PASSIVE; // 
	this->_addrinfo.ai_protocol = 0; // peut renvoyer des adresses de socket de n'importe quel type
}

Server::~Server()
{
	freeaddrinfo(_ptr_info); // free la liste chainee pointee par _serv_info
	std::cout << "end server" << std::endl;
}
/*-------------------------------------------------------*/

int	Server::start_server(void)
{
	int status;

	// Creation du socket
	if ((_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
		return (server_error("Error when initialise socket"));

	// Met en place les structures
	status = getaddrinfo(_ip_str.c_str(), "8080", &_addrinfo, &_ptr_info); // param a specifier
	if (status != 0)
	{
		std::cout << "error status : " << status << " ";
		return (server_error("getaddrinfo error"));
	}
	return (1);
}

int	Server::server_error(const std::string error_message) const
{
	std::cout << error_message << std::endl;
	return (0);
}