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
#include "signal.hpp"

#ifndef SERVER_HPP
# define SERVER_HPP

# define BODY_SIZE 			15000 // a changer avec body_size du .conf
# define EVENTS_HANDLED		50 // pour le tableau de fd
typedef std::size_t			size_type;
typedef	int					Socket;

class Server
{

	private:

	Server();

	public:

	Server(std::string ip, int port);
	~Server();

	/*---- MEMBER FUNCTIONS ----*/
	int		init_server(void);
	int		init_socket(void);
	int		start_server(void);

	int		handle_server(void);
	int		accept_connect(int epoll_fd, int i);
	int 	handle_request(int epoll_fd, int i);

	void*	get_addr(sockaddr *s_addr);
	void	server_ok(const std::string ok_message) const;
	int		server_error(const std::string error_message) const;
	void	display_ip(std::string domain);
	/*--------------------------*/

	/*---- CONFIGURATION VARIABLES ----*/
	int				_port;
	unsigned int 	_ip;
	std::string 	_ip_str;
	std::string 	_serv_name;
	unsigned int 	_body_size;
	bool 			_default_error;
	std::string 	_path_error_file;
	std::map<int, std::string> _available_method;
	std::string 	_redir_path;
	std::string 	_workdir_path;
	bool 			_auto_index;
	/*---------------------------------*/

	/*---- SERVER VARIABLES ----*/
	Socket				_socketfd;
	Socket				_remote_socketfd;
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