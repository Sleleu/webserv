#include "../../header/server/Server.hpp"


/*----------------- CONSTRUCTORS ------------------------*/
Server::Server()
{
	std::cout << "Server default constructor called" << std::endl;
	std::memset(&_addrinfo, 0, sizeof(_addrinfo)); // initialiser tous les membres a 0
	this->_addrinfo.ai_family = AF_UNSPEC; // Pour IPv4 et IPv6, IF_INET pour seulement v4
	this->_addrinfo.ai_socktype = SOCK_STREAM; // Pour TCP
	this->_addrinfo.ai_flags = AI_PASSIVE; // 
	this->_addrinfo.ai_protocol = 0; // peut renvoyer des adresses de socket de n'importe quel type
}


Server::Server(std::string ip, int port) : _port(port), _ip_str(ip)
{
	std::cout << "Server assign constructor called" << std::endl;
	std::memset(&_addrinfo, 0, sizeof(_addrinfo)); // initialiser tous les membres a 0
	this->_addrinfo.ai_family = AF_UNSPEC; // Pour IPv4 et IPv6, IF_INET pour seulement v4
	this->_addrinfo.ai_socktype = SOCK_STREAM; // Pour TCP
	this->_addrinfo.ai_flags = AI_PASSIVE; // Se lie a l'IP de l'hote sur lequel le programme s'execute
	this->_addrinfo.ai_protocol = 0; // peut renvoyer des adresses de socket de n'importe quel type
}

Server::~Server()
{
	// freeaddrinfo(_ptr_info); // free la liste chainee pointee par _serv_info
	std::cout << "end server" << std::endl;
}
/*-------------------------------------------------------*/

int	Server::init_server(void)
{
	int status;

	// Met en place les structures
	status = getaddrinfo(_ip_str.c_str(), "8080", &_addrinfo, &_ptr_info); // param a specifier
	if (status != 0)
	{
		std::cout << "error status : " << status << " ";
		return (server_error("<- getaddrinfo error"));
	}
	//test affichage ip avec getaddrinfo
	display_ip(_ip_str);

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
		return (server_error("Error when initialise socket"));

	// Set le socketfd en non-bloquant
	fcntl(_socketfd, F_SETFL, O_NONBLOCK);

	// associer le socket a un port sur le localhost
	if ((bind(_socketfd, _ptr_info->ai_addr, _ptr_info->ai_addrlen)) == -1) // inutile en tant que client car on se soucie pas du port local
		return (server_error("Error when bind socket"));

	int optval = 1;
	if (setsockopt(_socketfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(int)) == -1) // return 0 si success
		return (server_error("Setsockopt error"));

	freeaddrinfo(_ptr_info); // free la liste chainee pointee par _serv_info
	return (1);
}

int Server::start_server(void)
{
	if ((listen(_socketfd, 5)) == -1)
		return (server_error("Error when listenning socket"));
	if (server_routine() == 0) // lancement de la loop
		return (0);
	return (1);
}

int	Server::server_routine(void)
{
	int event_count; // numero de fd actif
	int epoll_fd; // pour le fd epoll

	epoll_fd = epoll_create1(0); // init du socket_server avec epoll()
	_server_event.data.fd = _socketfd;
	_server_event.events = EPOLLIN; // rendre le fd disponible en lecture
	epoll_ctl(epoll_fd, EPOLL_CTL_ADD, _socketfd, &_server_event); // ajouter le fd a l'ensemble events

	while (42)
	{
		int event_status;

		event_count = epoll_wait(epoll_fd, _events, EVENTS_HANDLED, -1);
		if (event_count == -1)
			return (server_error("epoll_wait : error"));
		for (int i = 0; i < event_count; i++)
		{
			if (_events[i].data.fd == _socketfd) // Si event sur le socket du server : nouvelle connexion
				event_status = accept_connect(epoll_fd, i);
			else
				event_status = handle_request(epoll_fd, i); // Traiter requete du client
			if (event_status == 0)
				return (0);
		}
	}
	return (1);
}

int	Server::accept_connect(int epoll_fd, int i)
{
	sockaddr_in	remote_addr; // les infos sur la connexion entrante iront ici
	socklen_t addrlen = sizeof(remote_addr); // taille de la struct remote_addr
	char remoteip[INET6_ADDRSTRLEN]; // pour affichage de l'ip de la connexion entrante

	this->_remote_socketfd = accept(_socketfd, (sockaddr *)&remote_addr, &addrlen); // On accepte la connexion et on cree un nouveau socket pour le client
	if (_remote_socketfd < 0)
		return (server_error("Error : failed to accept connexion from client"));
	fcntl(_remote_socketfd, F_SETFL, O_NONBLOCK); // On rend le socket non-bloquant	
	_server_event.data.fd = _remote_socketfd;
	_server_event.events = EPOLLIN;
	epoll_ctl(epoll_fd, EPOLL_CTL_ADD, _remote_socketfd, &_server_event); // l'ajouter a l'ensemble de fd
				
	// affichage
	std::cout << " New connection from "
	<< inet_ntop(remote_addr.sin_family, get_addr((sockaddr *)&remote_addr), remoteip, INET6_ADDRSTRLEN)
	<< " on socket " << _events[i].data.fd << std::endl;
	return (1);
}

int	Server::handle_request(int epoll_fd, int i)
{
	char msg_to_recv[BODY_SIZE] = {0}; 

	ssize_t bytes_received = recv(_events[i].data.fd, msg_to_recv, BODY_SIZE, 0);
	if (bytes_received <= 0)
	{
		if (bytes_received == -1)
			return (server_error("Error : Could not receive data from client"));
		close(_events[i].data.fd);
		epoll_ctl(epoll_fd, EPOLL_CTL_DEL, _events[i].data.fd, NULL);
	}
	else
	{
		this->_msg_to_send = "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: 12\n\n <body> TEST </body>";

		std::cout << msg_to_recv << std::endl;
		ssize_t bytes = send(_events[i].data.fd, _msg_to_send.c_str(), _msg_to_send.size(), 0);
		if (((unsigned long)bytes != _msg_to_send.size()) || bytes == -1)
			return (server_error("Error sending response to client"));
	}
	return (1);
}
