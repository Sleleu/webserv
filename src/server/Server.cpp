#include "../../header/server/Server.hpp"


/*----------------- CONSTRUCTORS ------------------------*/
Server::Server() {}

Server::Server(std::string ip, std::string port) : _port(port), _ip(ip)
{
	std::cout << "Server assign constructor called" << std::endl;
	std::memset(&_addrinfo, 0, sizeof(_addrinfo)); // initialiser tous les membres a 0
	this->_addrinfo.ai_family = AF_UNSPEC; // Pour IPv4 et IPv6, IF_INET pour seulement v4
	this->_addrinfo.ai_socktype = SOCK_STREAM; // Pour TCP
	this->_addrinfo.ai_flags = AI_PASSIVE; // Se lie a l'IP de l'hote sur lequel le programme s'execute
	this->_addrinfo.ai_protocol = 0; // peut renvoyer des adresses de socket de n'importe quel type
}

Server::Server(map_server map, location_server location) : _map_server(map), _location_server(location)
{
	std::cout << "Server assign constructor called" << std::endl;
	std::memset(&_addrinfo, 0, sizeof(_addrinfo)); // initialiser tous les membres a 0
	this->_addrinfo.ai_family = AF_UNSPEC; // Pour IPv4 et IPv6, IF_INET pour seulement v4
	this->_addrinfo.ai_socktype = SOCK_STREAM; // Pour TCP
	this->_addrinfo.ai_flags = AI_PASSIVE; // Se lie a l'IP de l'hote sur lequel le programme s'execute
	this->_addrinfo.ai_protocol = 0; // peut renvoyer des adresses de socket de n'importe quel type

	_port = "8080";
	_ip = "0.0.0.0";
	// _ip = map[ip]; // a continuer ici
}

Server::~Server()
{
	std::cout << "end server" << std::endl;
}
/*-------------------------------------------------------*/

int	Server::init_server(void)
{
	int status;

	// Met en place les structures
	status = getaddrinfo(_ip.c_str(), _port.c_str(), &_addrinfo, &_ptr_info); // param a specifier
	if (status != 0)
	{
		std::cout << "error status : " << status << " ";
		return (display_error("<- getaddrinfo error"));
	}
	//test affichage ip avec getaddrinfo
	display_ip(_ip);

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
		return (display_error("Error when initialise socket"));
	display_ok("Initialise server socket:");

	// Set le socketfd en non-bloquant
	fcntl(_socketfd, F_SETFL, O_NONBLOCK);

	// associer le socket a un port sur le localhost
	if ((bind(_socketfd, _ptr_info->ai_addr, _ptr_info->ai_addrlen)) == -1) // inutile en tant que client car on se soucie pas du port local
		return (display_error("Error when bind socket"));
	display_ok("Bind server socket:");

	int optval = 1;
	if (setsockopt(_socketfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(int)) == -1) // return 0 si success
		return (display_error("Setsockopt error"));

	freeaddrinfo(_ptr_info); // free la liste chainee pointee par _serv_info
	return (1);
}

int Server::start_server(void)
{
	if ((listen(_socketfd, 5)) == -1)
		return (display_error("Error when listenning socket"));
	display_ok("Start listening:");
	if (handle_server() == 0) // lancement de la loop
		return (0);
	return (1);
}
