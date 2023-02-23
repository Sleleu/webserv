#include "Server.hpp"
#include "server_includes.hpp"

#ifndef CLIENT_HPP
# define CLIENT_HPP

#define RESPONSE_SIZE 15000

class Client
{
	public:

	Client(std::string ip, std::string port, std::string request);
	~Client();
	
	int				init_client(void);
	int				request_to_server(int socket, std::string request);
	
	std::string get_ip(void) const;
	std::string get_port(void) const;
	std::string	get_request(void) const;
	std::string	get_response(void) const;
	int			get_sockfd(void) const;

	private:

	Client();

	addrinfo	_server_info;
	addrinfo*	_ptr_info;
	std::string _ip;
	std::string _port;
	std::string	_request;
	std::string	_response;
	int			_sockfd;

};

#endif