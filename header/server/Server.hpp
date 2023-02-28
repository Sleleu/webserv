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
#include "server_includes.hpp"

#ifndef SERVER_HPP
# define SERVER_HPP

# define B_SIZE 			1500000 // a changer avec body_size du .conf
# define EVENTS_HANDLED		10000 // pour le tableau de fd
# define MTU				1500 // maximum transmission unit
class Server
{
	public:

	typedef std::size_t			size_type;
	typedef	int					Socket;
	typedef std::map<std::string, std::vector<std::string> > 							map_server;
	typedef std::map<std::string, std::map<std::string, std::vector<std::string> > >	location_server;
	typedef std::map<std::string, std::vector<std::string> >::iterator					map_iterator;

	Server(std::string ip, std::string port);
	Server(map_server map, location_server location, int id, bool verbose);
	~Server();

	/*---- MEMBER FUNCTIONS ----*/
	int		init_server(void);
	int		init_socket(void);
	int		start_server(void);
	int		accept_connect(int epoll_fd);
	int		epoll_add(int epoll_fd, int socket);
	int		epoll_mod(int epoll_fd, int socket, int event);
	void	send_client_request(int epoll_fd, int i);
	int		send_message_to_client(int epollfd, int client_fd);
	int 	handle_request(int& epoll_fd, int i);
	void*	get_addr(sockaddr *s_addr);
	void	display_ip(std::string domain);
	int		add_socket_to_events(int epoll_fd);

	std::string get_ip(void) const;
	std::string	get_port(void) const;
	unsigned int get_id(void) const;
	std::string get_serv_name(void) const;
	Socket		get_socketfd(void) const;
	Socket		get_sender_fd(void) const;
	std::vector<Socket> 	get_client_fd(void) const; // new version

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
	unsigned int	_id_server;
	bool			_verbose;
	/*---------------------------------*/

	/*---- SERVER VARIABLES ----*/
	Socket				_socketfd;
	Socket				_sender_fd;
	std::vector<Socket>	_client_fd;
	std::vector<std::string> _client_request;
	std::vector<size_t>	_total_bytes;
	std::string			_msg_to_send;
	std::string			_msg_to_recv;
	struct addrinfo		_addrinfo;
	struct addrinfo		*_ptr_info; // va recuperer le resultat de getaddrinfo
	struct sockaddr_in _sockaddr;
	struct epoll_event _server_event;
	struct epoll_event _events[EVENTS_HANDLED];
	/*--------------------------*/
};

#endif
