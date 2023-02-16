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
	//int optval = 1; // necessaire pour setsockopt

	// Creation du socket
	// Utilise les resultats de getaddrinfo et les mettre dans socket
	if ((_socketfd = socket(AF_INET, _ptr_info->ai_socktype, _ptr_info->ai_protocol)) == -1) // renvoie un descripteur de socket
		return (server_error("Error when initialise socket"));

	// Set le socketfd en non-bloquant
	fcntl(_accept_socketfd, F_SETFL, O_NONBLOCK);

	// associer le socket a un port sur le localhost
	if ((bind(_socketfd, _ptr_info->ai_addr, _ptr_info->ai_addrlen)) == -1) // inutile en tant que client car on se soucie pas du port local
		return (server_error("Error when bind socket"));

	freeaddrinfo(_ptr_info); // free la liste chainee pointee par _serv_info
	return (1);
}

int Server::start_server(void)
{
	int listen_status;
	
	// attendre les connexions entrantes sur le host::port defini pour le socket
	listen_status = listen(_socketfd, 5);
	if (listen_status == -1)
		return (server_error("Error when listenning socket"));
	
	// Lancement de la routine du serveur
	if (server_routine() == 0)
		return (0);
	return (1);
}

int	Server::server_routine(void)
{
	struct pollfd poll_fd[2]; // 2 connections ?
	int fd_count = 1; // 1 pour le socket du serveur
	int fd_size = 5;
	sockaddr_in	remote_addr; // les infos sur la connexion entrante iront ici
	
	poll_fd[0].fd = _socketfd;
	poll_fd[0].events = POLLIN; // pret pour recevoir une connexion

	while (42)
	{
		char msg_to_recv[BODY_SIZE] = {0}; // Initialiser le buffer
		int status;

		int poll_count = poll(poll_fd, fd_count, -1);
		if (poll_count == -1)
			return (server_error("Error : poll failed"));

		for (int i = 0; i < fd_count; i++)
		{
			if (poll_fd[i].revents & POLLIN) // si on doit recevoir des donnees
			{
				if (poll_fd[i].fd == this->_socketfd) // si c'est le socket du serveur, on ajoute une nouvelle connexion
					status = accept_connect(poll_fd, remote_addr, fd_count, fd_size);
				else // C'est un client qui se connecte au serveur
					status = receive_data(poll_fd, msg_to_recv);
				if (!status)
					return (0);
			}
			else // C'est au serveur d'envoyer la reponse
			{
					/*--- ENVOI A GAB ICI---*/

					// Reponse du serveur apres la requete
					this->_msg_to_send = "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: 12\n\n <body> TEST </body>";
					ssize_t bytes = send(_accept_socketfd, _msg_to_send.c_str(), _msg_to_send.size(), 0);
					if (((unsigned long)bytes != _msg_to_send.size()) || bytes == -1)
						return (server_error("Error sending response to client"));
					else
						std::cout << "Message sent\n";
			}
		}
	}
	return (1);
}

int	Server::accept_connect(pollfd *poll_fd, sockaddr_in& remote_addr, int& fd_count, int& fd_size)
{
	socklen_t addrlen = sizeof(remote_addr);
	char remoteip[INET6_ADDRSTRLEN];

	this->_accept_socketfd = accept(_socketfd, (sockaddr *)&remote_addr, &addrlen);
	if (_accept_socketfd < 0)
		return (server_error("Error : failed to accept connexion from client"));
				
	add_pollfd(poll_fd[1], fd_count, fd_size); // nouvelle connexion A CHANGER
	std::cout << " New connection from "
	<< inet_ntop(remote_addr.sin_family, get_addr((sockaddr *)&remote_addr), remoteip, INET6_ADDRSTRLEN)
	<< " on socket " << _accept_socketfd << std::endl;
	return (1);
}

int	Server::receive_data(pollfd *poll_fd, char *msg_to_recv)
{
	// Recevoir la requete du client
	ssize_t bytes_received = recv(poll_fd[1].fd, msg_to_recv, BODY_SIZE, 0);
	if (bytes_received < 0)
		return (server_error("Error : Could not receive data from client"));
	else
		std::cout << msg_to_recv << "\n\n\n";

	int optval = 1;
	if (setsockopt(_accept_socketfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(int)) == -1) // return 0 si success
		return (server_error("Setsockopt error"));
	return (1);
}