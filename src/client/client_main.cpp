#include "../../header/server/Client.hpp"

int main(int argc, char **argv)
{
	if (argc != 4)
		return (std::cerr << BOLDYELLOW << "Client usage : ./client <ip> <port> <request>\n" << RESET, 1); 
	Client client(argv[1], argv[2], argv[3]);

	int socket;

	if (argv[3][0] == '\0')
		return (0);
	socket = client.init_client();
	if (socket != 0)
		client.request_to_server(client.get_sockfd(), client.get_request());

	return (0);
}