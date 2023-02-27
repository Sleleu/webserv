#include "Server.hpp"
#include "server_includes.hpp"

#ifndef HANDLER_HPP
# define HANDLER_HPP

class Handler
{

	public:

	typedef std::vector<std::map<std::string, std::vector<std::string> > > 			 				conf_server;
	typedef std::vector<std::map<std::string, std::vector<std::string> > >::iterator 				conf_server_iterator;
	typedef std::vector<std::map<std::string, std::map<std::string, std::vector<std::string> > > >	conf_location;
	typedef std::vector<std::map<std::string, std::map<std::string, std::vector<std::string> > > >::iterator	conf_location_iterator;

	Handler();
	Handler(conf_server c_server, conf_location c_location, bool);
	~Handler();

	int		launch_servers(void);
	int		waiting_process(void);
	int		handle_servers(void);
	void	close_servers_sockfd(void);

	private:

	conf_server			_conf_server;
	conf_location		_conf_location;
	std::vector<Server>	_v_server;
	struct epoll_event	_events[EVENTS_HANDLED];
	int					_epollfd;
	bool				_verbose;

};

#endif
