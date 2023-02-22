#include "../../header/server/handler.hpp"

/*
	Points

	- La size big_location est-elle toujours de la meme taille que celle de big_vector ?
*/

Handler::Handler()
{
	std::cout << "Default constructor called" << std::endl;
}

Handler::Handler(conf_server c_server, conf_location c_location) : _conf_server(c_server), _conf_location(c_location)
{
	conf_server_iterator   conf_server_it = _conf_server.begin();
	conf_location_iterator conf_location_it = _conf_location.begin();

	for (size_t id = 0; conf_server_it < _conf_server.end(); conf_server_it++, conf_location_it++, id++)
	{
		Server new_server(*conf_server_it, *conf_location_it, id);
		_v_server.push_back(new_server);
	}
	std::cout << "Servers added to handler" << std::endl;
}

Handler::~Handler()
{
	std::cout << "End handler" << std::endl;
}

int	Handler::launch_servers(void)
{
	std::vector<Server>::iterator it;
	for (it = _v_server.begin(); it < _v_server.end(); it++)
	{
		(*it).init_server();
		(*it).start_server();
	}
	return (1);
}

bool is_client_owned(Server& server, int event_fd)
{
	int i = 0;

	if (server.get_client_fd().size() == 0)
		return (false);
	for (; i < (int)server.get_client_fd().size(); i++)
		if (server.get_client_fd()[i] == event_fd)
			return (true);
	return (false);
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
		std::signal(SIGINT, signal_handler);
		event_count = epoll_wait(_epollfd, _events, EVENTS_HANDLED, -1);
		if (event_count == -1)
			return (display_error("epoll_wait"));
		for (int i = 0; i < event_count; i++)
		{
			for (it = _v_server.begin(); it < _v_server.end(); it++)
			{
				if (_events[i].data.fd == (*it).get_socketfd())
					event_status = (*it).accept_connect(_epollfd);
			}
			if (it == _v_server.end())
			{
				for (it = _v_server.begin(); it < _v_server.end(); it++)
				{
					if (is_client_owned(*it, _events[i].data.fd))
						event_status = (*it).handle_request(_events, _epollfd, i);
				}
			}
			
			if (event_status == 0)
				return (0);
			//}
		}
	}
	return (1);
}

