#include "../../header/server/Server.hpp"

void	signal_handler(int signal)
{
	if (signal == SIGINT || signal == SIGQUIT)
		g_CONTINUE = false;
}
