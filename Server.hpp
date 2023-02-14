#include <iostream>
#include <vector>
#include <map>
#include <cstring>
#include <cstdlib>

#include <sys/socket.h>
#include <sys/types.h>
#include <fcntl.h>
#include <netinet/in.h> // struct sockaddr_in
#include <netdb.h> // struct addrinfo
#include <arpa/inet.h>

#ifndef SERVER_HPP
# define SERVER_HPP

typedef	int					Socket;

class Server
{

	private:

	Server();

	public:

	Server(std::string ip, int port);
	~Server();

	/*---- MEMBER FUNCTIONS ----*/
	int		start_server(void);
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
	Socket				_socket;
	Socket				_accept_socket;
	addrinfo		_addrinfo;
	addrinfo		*_ptr_info; // va recuperer le resultat de getaddrinfo
/*
struct addrinfo {
    int              ai_flags; // AI_PASSIVE, AI_CANONNAME, etc
    int              ai_family; // AF_INET , AF_UNSPEC pour tout use
    int              ai_socktype; // SOCK_STREAM
    int              ai_protocol; // 0 pour "tout"
    size_t           ai_addrlen; // sizeof(sockaddr_in)
    struct sockaddr *ai_addr; // struct sockaddr_in
    char            *ai_canonname; // canonical hostname
    struct addrinfo *ai_next; // liste chainee, next node
};

struct sockaddr {
    unsigned short    sa_family;    // address family, AF_xxx
    char              sa_data[14];  // 14 bytes of protocol address
}; 
*/
	struct sockaddr_in _sockaddr;
/*
struct sockaddr_in {
    short            sin_family;   // e.g. AF_INET
    unsigned short   sin_port;     // e.g. htons(3490)
    struct in_addr   sin_addr;     // Internet address
    char             sin_zero[8];  // zize of struct sockaddr
};

struct in_addr {
    uint32_t s_addr; // that's a 32-bit int (4 bytes)
};
*/


	/*--------------------------*/


};

#endif