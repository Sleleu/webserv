#include "../../header/server/Server.hpp"

void*	Server::get_addr(sockaddr *s_addr)
{
	if (s_addr->sa_family == AF_INET)
		return &(((sockaddr_in *)s_addr)->sin_addr);
	else
		return &(((sockaddr_in6 *)s_addr)->sin6_addr);
}

//------------------ DISPLAY STATUS -------------------------------------------
int	Server::server_error(const std::string error_message) const
{
	std::cout << "Extension:";
	std::cout << " [" << BOLDGREEN << "OK" << RESET << "]" << std::endl;

	std::cerr << error_message;
	std::cerr << " [" << BOLDRED << "KO" << RESET << "]" << std::endl;
	return (0);
}

void Server::server_ok(const std::string ok_message) const
{
	std::cout << ok_message;
	std::cout << " [" << BOLDGREEN << "OK" << RESET << "]" << std::endl;
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
//-------------------------------------------------------------------