#include "../../header/server/Server.hpp"

void	signal_handler(int signal)
{
	if (signal == SIGINT)
		std::exit(EXIT_SUCCESS);
}
