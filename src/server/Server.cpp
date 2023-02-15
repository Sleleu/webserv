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
	freeaddrinfo(_ptr_info); // free la liste chainee pointee par _serv_info
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
	int optval = 1; // necessaire pour setsockopt

	// Creation du socket
	// Utilise les resultats de getaddrinfo et les mettre dans socket
	if ((_socketfd = socket(AF_INET, _ptr_info->ai_socktype, _ptr_info->ai_protocol)) == -1) // renvoie un descripteur de socket
		return (server_error("Error when initialise socket"));

	// Set le socketfd en non-bloquant
	fcntl(_accept_socketfd, F_SETFL, O_NONBLOCK);

	// associer le socket a un port sur le localhost
	if ((bind(_socketfd, _ptr_info->ai_addr, _ptr_info->ai_addrlen)) == -1) // inutile en tant que client car on se soucie pas du port local
		return (server_error("Error when bind socket"));
	
	// Rendre le port reutilisable par le programme car les sockets ne sont pas dans un état partagé par défaut
	// SO_REUSEADDR permet a un socket de se lier de force a un port utilise par un autre socket
	
	if (setsockopt(_socketfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(int)) == -1) // return 0 si success
		return (server_error("Setsockopt error"));
	return (1);
}

int Server::start_server(void)
{
	int listen_status;
	sockaddr_in	accept_addr; // les infos sur la connexion entrante iront ici
	
	// attendre les connexions entrantes sur le host::port defini pour le socket
	listen_status = listen(_socketfd, 5);
	if (listen_status == -1)
		return (server_error("Error when listenning socket"));
	
	// Lancement de la routine du serveur
	while (42)
	{
		socklen_t sin_size = sizeof(accept_addr); // pour accept
		char msg_to_recv[BODY_SIZE] = {0}; // Initialiser le buffer

		this->_accept_socketfd = accept(_socketfd, (sockaddr *)&accept_addr, &sin_size);
		if (_accept_socketfd < 0)
			return (server_error("Error : failed to accept connexion from client"));

		int optval = 1;
		if (setsockopt(_accept_socketfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(int)) == -1) // return 0 si success
		return (server_error("Setsockopt error"));

		// Recevoir la requete du client
		ssize_t bytes_received = recv(_accept_socketfd, msg_to_recv, BODY_SIZE, 0);
		if (bytes_received < 0)
			return (server_error("Error : Could not receive data from client"));

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

int	Server::server_error(const std::string error_message) const
{
	std::cout << error_message << std::endl;
	return (0);
}

void	Server::display_ip(std::string domain)
{
	addrinfo *tmp;
	void *addr;
	std::string ip_version; // affichage du type d'ip

	for (tmp = _ptr_info; tmp != NULL; tmp = tmp->ai_next) // parcourir la liste chainee d'adresses
	{

		if (tmp->ai_family == AF_INET) // si famille = ipv4
		{
			sockaddr_in *ipv4 = (sockaddr_in *)tmp->ai_addr;
			addr = &(ipv4->sin_addr);
			ip_version = "IPv4";
		}
		else if (tmp->ai_family == AF_INET6) // si famille = ipv6
		{
			sockaddr_in6 *ipv6 = (sockaddr_in6 *)tmp->ai_addr;
			addr = &(ipv6->sin6_addr);
			ip_version = "IPv6";
		}
		char ipstr[INET6_ADDRSTRLEN]; // 	
		inet_ntop(tmp->ai_family, addr, ipstr, INET6_ADDRSTRLEN);
		std::cout << domain << " " <<  ip_version << " " << ipstr << std::endl;
	}
}