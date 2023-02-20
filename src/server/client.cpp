#include "../../header/server/Client.hpp"

Client::Client()
{

}

Client::Client(std::string ip, std::string port) : _ip(ip), _port(port)
{
	std::cout << "Assign client constructor called" << std::endl;
	
	std::memset(&_server_info, 0, sizeof(_server_info));
	_server_info.ai_family = AF_UNSPEC;
	_server_info.ai_socktype = SOCK_STREAM;
	_server_info.ai_flags = AI_PASSIVE;
	_server_info.ai_protocol = 0;
}

Client::~Client(void)
{
	std::cout << "end client" << std::endl;
}

int	Client::request_to_server(void)
{

	int optval = 1;
	int client_socket;

	if ((getaddrinfo(get_ip().c_str(), get_port().c_str(), &_server_info, &_ptr_info)) != 0)
		return (display_error("getaddrinfo() error"));
	client_socket = socket(AF_INET, _ptr_info->ai_socktype, _ptr_info->ai_protocol);
	if (client_socket == -1)
		return (display_error("socket() error"));
	fcntl(client_socket, F_SETFL, O_NONBLOCK);
	if (setsockopt(client_socket, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(int)) == -1)
		return (display_error("Setsockopt error"));
	
}

