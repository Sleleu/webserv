#include "../../header/server/Client.hpp"

// int	g_exit_code;

void signal_client_handler(int signal)
{
	if (signal == SIGINT || signal == SIGQUIT)
		exit(EXIT_SUCCESS);
}

Client::Client()
{
	
}

Client::Client(std::string ip, std::string port, std::string request) : _ip(ip), _port(port), _request(request)
{
	std::memset(&_server_info, 0, sizeof(_server_info));
	_server_info.ai_family = AF_UNSPEC;
	_server_info.ai_socktype = SOCK_STREAM;
	_server_info.ai_flags = AI_PASSIVE;
	_server_info.ai_protocol = 0;
}

Client::~Client(void)
{
	std::cout << BOLDMAGENTA << "end client" << RESET << std::endl;
}

int	Client::init_client(void)
{
	int optval = 1;

	if ((getaddrinfo(get_ip().c_str(), get_port().c_str(), &_server_info, &_ptr_info)) != 0)
		return (std::cout << BOLDRED, display_error("getaddrinfo() error"));
	_sockfd = socket(AF_INET, _ptr_info->ai_socktype, _ptr_info->ai_protocol);
	if (_sockfd == -1)
		return (freeaddrinfo(_ptr_info), std::cout << BOLDRED, display_error("socket() error"));
	std::cout << BOLDGREEN;
	display_ok("socket:");
	
	if (setsockopt(_sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(int)) == -1)
	 	return (freeaddrinfo(_ptr_info), std::cout << BOLDRED, display_error("Setsockopt error"));
	
	if ((connect(_sockfd, _ptr_info->ai_addr, _ptr_info->ai_addrlen)) == -1)
		return (freeaddrinfo(_ptr_info), std::cout << BOLDRED, display_error("connect()"));
	std::cout << BOLDGREEN;
	display_ok("connect to server:");
	freeaddrinfo(_ptr_info);
	return (1);
}

int	Client::request_to_server(int socket, std::string request)
{
	char response[RESPONSE_SIZE] = {0};
	ssize_t bytes_sent = 0;
	unsigned long total_bytes_sent = 0;
	
	ssize_t bytes_received;

	while (total_bytes_sent != request.size() && request.size() != 0)
	{
		signal(SIGINT, signal_client_handler);
		signal(SIGQUIT, signal_client_handler);
		bytes_sent = send(socket, request.c_str(), request.size(), 0);
		if (bytes_sent < 0)
			return (std::cout << BOLDRED, display_error("send() error"));
		if (bytes_sent == 0)
			break ;
		total_bytes_sent += bytes_sent;
	}
	std::cout << BOLDGREEN;
	display_ok("request sent:");
	std::cout << BOLDYELLOW << "\nResponse: \n" << RESET;
	signal(SIGINT, signal_client_handler);
	signal(SIGQUIT, signal_client_handler);
	bytes_received = recv(socket, response, RESPONSE_SIZE, 0);
	if (bytes_received < 0)
	 	return (std::cout << BOLDRED, display_error("recv() error"));
	_response.append(response);
	std::cout << _response;
	return (1);
}

//-------------- GETTERS -----------------------------

std::string	Client::get_ip(void) const
{
	return (_ip);
}

std::string Client::get_port(void) const
{
	return (_port);
}

std::string Client::get_request(void) const
{
	return (_request);
}

std::string Client::get_response(void) const
{
	return (_response);
}

int	Client::get_sockfd(void) const
{
	return (_sockfd);
}

//----------------------------------------------------