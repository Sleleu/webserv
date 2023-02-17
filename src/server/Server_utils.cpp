#include "../../header/server/Server.hpp"

void*	Server::get_addr(sockaddr *s_addr)
{
	if (s_addr->sa_family == AF_INET)
		return &(((sockaddr_in *)s_addr)->sin_addr);
	else
		return &(((sockaddr_in6 *)s_addr)->sin6_addr);
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


// void	Server::add_pollfd(int& fd_count, int& fd_size)
// {
// 	if (fd_count == fd_size)
// 		fd_size *= 2;
// 	pollfd new_fd;
// 	new_fd.fd = this->_accept_socketfd;
// 	new_fd.events = POLLIN;
// 	_pollfd.push_back(new_fd);
// 	fd_count++;
// }

// void	Server::delete_pollfd(pollfd *poll_fd, int i, int& fd_count)
// {
// 	poll_fd[i] = poll_fd[fd_count - 1];
// 	fd_count--;
// }