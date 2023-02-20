#include "../../header/server/Server.hpp"

int	Server::handle_server(void)
{
	int event_count; // numero de fd actif
	int epoll_fd; // pour le fd epoll

	epoll_fd = epoll_create1(0); // init du socket_server avec epoll()

	_server_event.data.fd = _socketfd;
	_server_event.events = EPOLLIN; // rendre le fd disponible en lecture
	if ((epoll_ctl(epoll_fd, EPOLL_CTL_ADD, _socketfd, &_server_event)) == -1) // ajouter le fd a l'ensemble events
		return (display_error("epoll_fd epoll_ctl() error"));

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
				event_status = handle_request(epoll_fd, i); // Traiter requete du client
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

int	Server::handle_request(int epoll_fd, int i)
{
	char msg_to_recv[B_SIZE] = {0};

	ssize_t bytes_received = recv(_events[i].data.fd, msg_to_recv, B_SIZE, 0);
	if (bytes_received <= 0)
	{
		if (bytes_received == -1)
			return (display_error("Error : Could not receive data from client"));
		if ((epoll_ctl(epoll_fd, EPOLL_CTL_DEL, _events[i].data.fd, NULL)) == -1)
			return (display_error("recv data epoll_ctl() error"));
	}
	else
	{
		_msg_to_send = get_response(msg_to_recv, _location_server, _map_server);
// 		_msg_to_send = "HTTP/1.1 200 OK\n\
// content-length: 180\n\
// content-type: text/html\n\
// server: webSerV\n\n\
// <!DOCTYPE html>\n\
// <html>\n\
//         <head>\n\
//                 <title>This is index.html page !!</title>\n\
//         </head>\n\
//         <body>\n\
//         <p> -- Skyblog de Sleleu -- </p>\n\
//         </body>\n\
// </html>";
		std::cout << "RESPONSE : \n" << _msg_to_send << std::endl;
		ssize_t bytes = send(_events[i].data.fd, _msg_to_send.c_str(), _msg_to_send.size(), 0);
		if (((unsigned long)bytes != _msg_to_send.size()) || bytes == -1)
			return (display_error("Error sending response to client"));
	}
	return (1);
}
