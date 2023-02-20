#include "../../header/server/Server.hpp"

void*	Server::get_addr(sockaddr *s_addr)
{
	if (s_addr->sa_family == AF_INET)
		return &(((sockaddr_in *)s_addr)->sin_addr);
	else
		return &(((sockaddr_in6 *)s_addr)->sin6_addr);
}