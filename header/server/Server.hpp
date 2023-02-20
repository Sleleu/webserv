#include <iostream>
#include <vector>
#include <map>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <csignal>

#include <sys/socket.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h> // struct sockaddr_in
#include <netdb.h> // struct addrinfo
#include <arpa/inet.h>
#include <sys/epoll.h>
#include "../../header/utils/colors.hpp"
#include "../../header/response/response.hpp"
#include "signal.hpp"

#ifndef SERVER_HPP
# define SERVER_HPP

# define B_SIZE 			15000 // a changer avec body_size du .conf
# define EVENTS_HANDLED		100 // pour le tableau de fd

class Server
{
	public:

	typedef std::size_t			size_type;
	typedef	int					Socket;
	typedef std::map<std::string, std::vector<std::string> > 									  	map_server;
	typedef std::map<std::string, std::map<std::string, std::vector<std::string> > >				location_server;

	Server(std::string ip, std::string port);
	Server(map_server map, location_server location);
	~Server();

	/*---- MEMBER FUNCTIONS ----*/
	int		init_server(void);
	int		init_socket(void);
	int		start_server(void);

	int		handle_server(void);
	int		accept_connect(int epoll_fd);
	int		epoll_add(int epoll_fd, int socket);
	int 	handle_request(int epoll_fd, int i);

	void*	get_addr(sockaddr *s_addr);
	void	server_ok(const std::string ok_message) const;
	int		server_error(const std::string error_message) const;
	void	display_ip(std::string domain);
	/*--------------------------*/

	private:

	Server();

	map_server		_map_server;
	location_server _location_server;

	/*---- CONFIGURATION VARIABLES ----*/
	std::string		_port;
	std::string 	_ip;
	std::string 	_serv_name;
	unsigned int 	_body_size;
	/*---------------------------------*/

	/*---- SERVER VARIABLES ----*/
	Socket				_socketfd;
	Socket				_sender_fd;
	std::string			_msg_to_send;
	std::string			_msg_to_recv;
	addrinfo		_addrinfo;
	addrinfo		*_ptr_info; // va recuperer le resultat de getaddrinfo
	struct sockaddr_in _sockaddr;
	struct epoll_event _server_event;
	struct epoll_event _events[EVENTS_HANDLED];
	/*--------------------------*/
};

#endif