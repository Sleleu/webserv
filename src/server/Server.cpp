#include "../../header/server/Server.hpp"


/*----------------- CONSTRUCTORS ------------------------*/
Server::Server() {}

Server::Server(std::string ip, std::string port) : _port(port), _ip(ip)
{
	std::memset(&_addrinfo, 0, sizeof(_addrinfo)); // initialiser tous les membres a 0
	this->_addrinfo.ai_family = AF_INET; // Pour IPv4 et IPv6, IF_INET pour seulement v4
	this->_addrinfo.ai_socktype = SOCK_STREAM; // Pour TCP
	this->_addrinfo.ai_flags = AI_PASSIVE; // Se lie a l'IP de l'hote sur lequel le programme s'execute
	this->_addrinfo.ai_protocol = 0; // peut renvoyer des adresses de socket de n'importe quel type
}

Server::Server(map_server map, location_server location, int id, bool verbose)
: _map_server(map), _location_server(location), _id_server(id + 1), _verbose(verbose)
{
	std::memset(&_addrinfo, 0, sizeof(_addrinfo)); // initialiser tous les membres a 0
	this->_addrinfo.ai_family = AF_INET; // Pour IPv4 et IPv6, IF_INET pour seulement v4
	this->_addrinfo.ai_socktype = SOCK_STREAM; // Pour TCP
	this->_addrinfo.ai_flags = AI_PASSIVE; // Se lie a l'IP de l'hote sur lequel le programme s'execute
	this->_addrinfo.ai_protocol = 0; // peut renvoyer des adresses de socket de n'importe quel type

	_port = map.find("listen")->second[0]; // assigner port au fichier conf
	_serv_name = map.find("server_name")->second[0];
	_body_size = std::atoi(map.find("body_size")->second[0].c_str()); // A AJOUTER
	_ip = "localhost";

	/*--- Affichage initialisation du serveur ---*/
	std::cout << BOLDCYAN << "Server [" << BOLDYELLOW << get_serv_name()
			  << BOLDCYAN << "] id [" << BOLDGREEN << get_id()
			  << BOLDCYAN << "] on port [" << BOLDBLUE << get_port()
			  << BOLDCYAN << "] initialised" << RESET << std::endl;
}

Server::~Server()
{

}
/*-------------------------------------------------------*/

int	Server::init_server(void)
{
	int status;

	// Met en place les structures
	status = getaddrinfo(_ip.c_str(), _port.c_str(), &_addrinfo, &_ptr_info);
	if (status != 0)
	{
		std::cout << "error status : " << status << " ";
		return (display_error("<- getaddrinfo error"));
	}

	// initialisation du socket serveur
	if ((init_socket()) == 0)
		return (0);
	return (1); // Retourne 1 si success
}

int	Server::init_socket(void)
{
	// Creation du socket
	// Utilise les resultats de getaddrinfo et les mettre dans socket
	if ((_socketfd = socket(AF_INET, _ptr_info->ai_socktype, _ptr_info->ai_protocol)) == -1) // renvoie un descripteur de socket
		return (freeaddrinfo(_ptr_info),display_error("Error when initialise socket"));
	std::cout << "[" << BOLDYELLOW << _serv_name << RESET;
	display_ok("] Initialise server socket:");

	// Set le socketfd en non-bloquant
	fcntl(_socketfd, F_SETFL, O_NONBLOCK);

	int optval = 1;
	if (setsockopt(_socketfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &optval, sizeof(int)) == -1) // return 0 si success
		return (freeaddrinfo(_ptr_info), display_error("Setsockopt error"));

	// associer le socket a un port sur le localhost
	if ((bind(_socketfd, _ptr_info->ai_addr, _ptr_info->ai_addrlen)) == -1) // inutile en tant que client car on se soucie pas du port local
		return (freeaddrinfo(_ptr_info), display_error("Error when bind socket"));
	std::cout << "[" << BOLDYELLOW << _serv_name << RESET;
	display_ok("] Bind server socket:");

	freeaddrinfo(_ptr_info); // free la liste chainee pointee par _serv_info
	return (1);
}

int Server::start_server(void)
{
	if ((listen(_socketfd, 20)) == -1)
		return (display_error("Error when listenning socket"));
	std::cout << "[" << BOLDYELLOW << _serv_name << RESET;
	display_ok("] Start listening:");
	return (1);
}

int	Server::add_socket_to_events(int epoll_fd)
{
	_server_event.data.fd = _socketfd;
	_server_event.events = EPOLLIN; // rendre le fd disponible en lecture
	if ((epoll_ctl(epoll_fd, EPOLL_CTL_ADD, _socketfd, &_server_event)) == -1) // ajouter le fd a l'ensemble events
		return (display_error("epoll_fd epoll_ctl() error"));
	return (1);
}

int	Server::accept_connect(int epoll_fd)
{
	Server::Socket client_socket;
	sockaddr_in	remote_addr; // les infos sur la connexion entrante iront ici
	socklen_t addrlen = sizeof(remote_addr); // taille de la struct remote_addr

	client_socket = accept(_socketfd, (sockaddr *)&remote_addr, &addrlen); // On accepte la connexion et on cree un nouveau socket pour le client
	if (client_socket < 0)
		return (display_error("Error : failed to accept connexion from client"));
	fcntl(client_socket, F_SETFL, O_NONBLOCK); // On rend le socket non-bloquant

	int optval = 1;
	if (setsockopt(client_socket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &optval, sizeof(int)) == -1) // return 0 si success
		return (display_error("Setsockopt error"));

	if (!epoll_add(epoll_fd, client_socket))
	{
		close(client_socket);
		return (0);
	}
	_client_fd.push_back(client_socket); // ajouter le socket a ce serveur

	/*--- Affichage nouvelle connexion au serveur ---*/
	std::cout << BOLDCYAN << "New connection to server [" << BOLDYELLOW << _serv_name
			  << BOLDCYAN << "] id [" << BOLDGREEN << _id_server
			  << BOLDCYAN << "] on socket [" << BOLDMAGENTA << client_socket
			  << BOLDCYAN << "]" << RESET << std::endl;

	return (1);
}

int	Server::epoll_add(int epoll_fd, int socket)
{
	std::memset(&_server_event, 0, sizeof(epoll_event));
	_server_event.data.fd = socket;
	_server_event.events = EPOLLIN;
	if ((epoll_ctl(epoll_fd, EPOLL_CTL_ADD, socket, &_server_event)) == -1) // l'ajouter a l'ensemble de fd
		return (display_error("new connexion epoll_ctl() error"));
	return (1);
}

int Server::send_message_to_client(int client_fd)
{
	unsigned long total_bytes = 0; // total de bytes envoyes
	size_t		  packet_sent = 0; // nombre de paquets envoyes


	while (total_bytes < _msg_to_send.size())
	{
		size_t packet_size = MTU < _msg_to_send.size() - total_bytes ? \
			MTU : _msg_to_send.size() - total_bytes; // send la valeur < entre MTU et bytes restants
		ssize_t bytes = send(client_fd, _msg_to_send.c_str() + total_bytes, packet_size, MSG_NOSIGNAL);
		if (bytes == -1)
		 	return (display_error("Unable to sent data to client"));
		total_bytes += bytes;
		packet_sent++;
	}
	std::cout << "Packet sent : " << packet_sent << std::endl;
	std::cout << BOLDCYAN << "Response from server [" << BOLDYELLOW << _serv_name
			  << BOLDCYAN << "] id [" << BOLDGREEN << _id_server
			  << BOLDCYAN << "] on socket [" << BOLDMAGENTA << client_fd
			  << BOLDCYAN << "] successfully sent" << RESET << std::endl;
	return (1);
}

std::string body_size_error()
{
	std::string message;

	message = "HTTP/1.1 200 OK\n\
	Content-Type: text/html\nContent-Length: 179\n\n<!DOCTYPE html>\n<html>\n\
	<head>\n<title>413 Request Entity Too Large</title>\n</head>\n\
	<body>\n<h1>Request Entity Too Large</h1>\n\
	<p>The request entity is too large.</p>\n</body>\n</html>\n";
	std::cout << "INVALID REQUEST FOR BODY SIZE\n";
	return (message);
}

int	Server::handle_request(int& epoll_fd, int i)
{
	char msg_to_recv[B_SIZE] = {0};
	ssize_t bytes_received;

	bytes_received = recv(_client_fd[i], msg_to_recv, B_SIZE, 0);
	if (bytes_received <= 0)
	{
		if ((epoll_ctl(epoll_fd, EPOLL_CTL_DEL, _client_fd[i], NULL)) == -1)
		{
			close(_client_fd[i]);
			return (display_error("recv data epoll_ctl() error"));
		}
		close(_client_fd[i]);
		_client_fd.erase(_client_fd.begin() + i);
	}
	if (bytes_received != 0)
	{
		std::cout << BOLDCYAN << "Message from socket [" << BOLDMAGENTA << _client_fd[i]
				  << BOLDCYAN << "] on server [" << BOLDGREEN << _id_server
				  << BOLDCYAN << "] successfully received" << RESET << std::endl;
		if (convert_char_to_string(msg_to_recv).size() > _body_size)
			_msg_to_send = body_size_error();
		else
			_msg_to_send = get_response(msg_to_recv, _location_server, _map_server, _verbose);
		if (!send_message_to_client(_client_fd[i]))
			return (1);
	}
	return (1);
}

void*	Server::get_addr(sockaddr *s_addr)
{
	if (s_addr->sa_family == AF_INET)
		return &(((sockaddr_in *)s_addr)->sin_addr);
	else
		return &(((sockaddr_in6 *)s_addr)->sin6_addr);
}

//------------------- GETTERS ------------------------------
std::string	Server::get_ip(void) const
{
	return (_ip);
}

std::string	Server::get_port(void) const
{
	return (_port);
}

std::string Server::get_serv_name(void) const
{
	return (_serv_name);
}

unsigned int Server::get_id(void) const
{
	return (_id_server);
}

Server::Socket	Server::get_socketfd(void) const
{
	return (_socketfd);
}

Server::Socket	Server::get_sender_fd(void) const
{
	return (_sender_fd);
}

std::vector<Server::Socket> Server::get_client_fd(void) const
{
	return (_client_fd);
}
//----------------------------------------------------------
