#include "../../header/server/Server.hpp"

void	print_vector(std::vector<int> &vector)
{
	std::vector<int>::iterator it = vector.begin();

	for (; it < vector.end(); it++)
		std::cout << *it << " | ";
	std::cout << std::endl;
}

int	display_error(const std::string error_message)
{
	std::cerr << error_message;
	std::cerr << RESET << " [" << BOLDRED << "KO" << RESET << "]" << std::endl;
	return (0);
}

void display_ok(const std::string ok_message)
{
	std::cout << ok_message;
	std::cout << RESET << " [" << BOLDGREEN << "OK" << RESET << "]" << std::endl;
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