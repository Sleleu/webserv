#include "../../header/server/Server.hpp"

int	Server::add_socket_to_events(int epoll_fd)
{
	_server_event.data.fd = _socketfd;
	_server_event.events = EPOLLIN; // rendre le fd disponible en lecture
	if ((epoll_ctl(epoll_fd, EPOLL_CTL_ADD, _socketfd, &_server_event)) == -1) // ajouter le fd a l'ensemble events
		return (display_error("epoll_fd epoll_ctl() error"));
	return (1);
}

int	Server::handle_server(void)
{
	int event_count; // numero de fd actif
	int epoll_fd; // pour le fd epoll

	epoll_fd = epoll_create1(0); // init du socket_server avec epoll()

	if (!add_socket_to_events(epoll_fd))
		return (0);
	while (42)
	{
		int event_status;

		std::signal(SIGINT, signal_handler); // Quitter proprement le serveur
		event_count = epoll_wait(epoll_fd, _events, EVENTS_HANDLED, -1);
		if (event_count == -1)
			return (display_error("epoll_wait : error"));
		for (int i = 0; i < event_count; i++)
		{
			if (_events[i].data.fd == _socketfd) // Si event sur le socket du server : nouvelle connexion
				event_status = accept_connect(epoll_fd);
			else
				event_status = handle_request(_events, epoll_fd, i); // Traiter requete du client
			if (event_status == 0)
				return (0);
		}
	}
	return (1);
}

int	Server::accept_connect(int epoll_fd)
{
	int client_socket;
	sockaddr_in	remote_addr; // les infos sur la connexion entrante iront ici
	socklen_t addrlen = sizeof(remote_addr); // taille de la struct remote_addr

	client_socket = accept(_socketfd, (sockaddr *)&remote_addr, &addrlen); // On accepte la connexion et on cree un nouveau socket pour le client
	if (client_socket < 0)
		return (display_error("Error : failed to accept connexion from client"));
	fcntl(client_socket, F_SETFL, O_NONBLOCK); // On rend le socket non-bloquant
	if (!epoll_add(epoll_fd, client_socket))
		return (0);

	_client_fd.push_back(client_socket); // ajouter le socket a ce serveur

	std::cout << BOLDCYAN << "New connection on server [" << BOLDGREEN << _id_server
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

int	Server::handle_request(epoll_event* events, int epoll_fd, int i)
{
	char msg_to_recv[B_SIZE] = {0};

	ssize_t bytes_received = recv(events[i].data.fd, msg_to_recv, B_SIZE, 0);
	if (bytes_received <= 0)
	{
		if (bytes_received == -1)
			return (std::cerr << "Server [" << get_id() << "] ", display_error("Error : Could not receive data from client"));
		if ((epoll_ctl(epoll_fd, EPOLL_CTL_DEL, events[i].data.fd, NULL)) == -1)
			return (display_error("recv data epoll_ctl() error"));
	}
	else
	{
		std::cout << BOLDCYAN << "Message from socket [" << BOLDMAGENTA << events[i].data.fd
				  << BOLDCYAN << "] on server [" << BOLDGREEN << _id_server
				  << BOLDCYAN << "] successfully sent" << RESET << std::endl;

		_msg_to_send = get_response(msg_to_recv, _location_server, _map_server);
		std::cout << "RESPONSE : \n" << _msg_to_send << std::endl;
		ssize_t bytes = send(events[i].data.fd, _msg_to_send.c_str(), _msg_to_send.size(), 0);
		if (((unsigned long)bytes != _msg_to_send.size()) || bytes == -1)
			return (display_error("Error sending response to client"));
	}
	return (1);
}
