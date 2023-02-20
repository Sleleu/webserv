#include "Server.hpp"
#include "server_includes.hpp"

#ifndef CLIENT_HPP
# define CLIENT_HPP

class Client
{
	public:

	Client(std::string ip, std::string port);
	~Client();
	
	int		request_to_server(void);
	void	get_server_info(addrinfo);
	std::string get_ip(void) const { return (_ip); }
	std::string get_port(void) const { return (_port); }

	private:

	Client();

	addrinfo	_server_info;
	addrinfo*	_ptr_info;
	std::string _ip;
	std::string _port;

};

#endif