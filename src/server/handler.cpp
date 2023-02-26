#include "../../header/server/handler.hpp"

/*
	Points
	- La size big_location est-elle toujours de la meme taille que celle de big_vector ?
*/

int g_exit_code;

void	signal_handler(int signal)
{
	if ((signal == SIGINT) || (signal == SIGQUIT))
	{
		g_exit_code = 1;
		std::cout << std::endl;
	}
}

Handler::Handler()
{
	std::cout << "Default constructor called" << std::endl;
}

Handler::Handler(conf_server c_server, conf_location c_location, bool verbose) : _conf_server(c_server), _conf_location(c_location), _verbose(verbose)
{
	conf_server_iterator   conf_server_it = _conf_server.begin();
	conf_location_iterator conf_location_it = _conf_location.begin();

	for (size_t id = 0; conf_server_it < _conf_server.end(); conf_server_it++, conf_location_it++, id++)
	{
		Server new_server(*conf_server_it, *conf_location_it, id, _verbose);
		_v_server.push_back(new_server);
	}
}

Handler::~Handler()
{
	close(_epollfd);
	std::cout << "End handler" << std::endl;
}

int	Handler::launch_servers(void)
{
	std::vector<Server>::iterator it;

	for (it = _v_server.begin(); it < _v_server.end(); it++)
		if (!(*it).init_server() || !(*it).start_server())
			return (0);
	return (1);
}

int	is_client_owned(Server& server, int event_fd)
{
	int i = 0;

	if (server.get_client_fd().size() == 0)
		return (-1);
	for (; i < (int)server.get_client_fd().size(); i++)
		if (server.get_client_fd()[i] == event_fd)
			return (i); // return l'index du client_fd
	return (-1);
}

void	Handler::close_servers_sockfd(void)
{
	std::vector<Server>::iterator	it;

	for (it = _v_server.begin(); it < _v_server.end(); it++)
		close((*it).get_socketfd());
}

int	Handler::waiting_process(void)
{
	int event_count;

	std::signal(SIGINT, signal_handler);
	std::signal(SIGQUIT, signal_handler);
	event_count = epoll_wait(_epollfd, _events, EVENTS_HANDLED, -1);
	if (event_count == -1) // -1 = erreur | 0 = timeout
	{	
	 	if (errno != EINTR) // EINTR : un signal a été reçu
			return (display_error("epoll_wait"));
	}
	return (event_count);
}

int	Handler::handle_servers(void)
{
	int event_count;

	_epollfd = epoll_create1(0);

	std::vector<Server>::iterator it;
	for (it = _v_server.begin(); it < _v_server.end(); it++)
		(*it).add_socket_to_events(_epollfd);
	while (42)
	{
		int event_status = 1;

		if (g_exit_code == 1) // on sort de la loop du serveur
			break;
		if ((event_count = waiting_process()) == 0)
			return (close_servers_sockfd(), 0);
		for (int i = 0; i < event_count; i++)
		{
			for (it = _v_server.begin(); it < _v_server.end(); it++)
			{
				if (_events[i].data.fd == (*it).get_socketfd()) // S'il s'agit de l'un des sockets d'un serveur
					event_status = (*it).accept_connect(_epollfd); // accepter la nouvelle connexion et ajouter le socketfd du client au serveur vise
				if (event_status == 0)
				{
					close_servers_sockfd();	
					return (0);
				}
			}
			if (it == _v_server.end()) // si l'event ne correspond pas a une nouvelle connexion
			{
				for (it = _v_server.begin(); it < _v_server.end(); it++) // Chercher a quel serveur correspond l'event
				{
					int index_client_fd = is_client_owned(*it, _events[i].data.fd);
					if (index_client_fd != -1) // Une fois lien client|serveur decouvert
							event_status = (*it).handle_request(_epollfd, index_client_fd); // traiter la requete
					if (event_status == 0)
					{
						close_servers_sockfd();
						return (0);
					}
				}
			}
		}
	}
	close_servers_sockfd();
	return (1);
}

