#include "../../header/parser/parser.hpp"
#include "../../header/utils/color.hpp"
#include "../../header/response/response.hpp"
#include "../../header/server/Server.hpp"
#include "../../header/server/handler.hpp"

bool	parse_argv(int argc, char **argv)
{
	std::string v;
	int 		verbose = false;

	if (argc == 1)
	{
		std::cerr << "Configuraion file is missing";
		std::cerr << " [" << BOLDRED << "KO" << RESET << "]" << std::endl;
		std::cerr << GREEN << "Usage: " << BOLDWHITE << "./webserv conf_file.conf <--verbose>\n" << RESET;
		throw std::exception();
	}
	if (argc > 2)
	{
		v = argv[2];
		if (v != "--verbose" || argc > 3)
		{
			std::cerr << "Unknow flags";
			std::cerr << " [" << BOLDRED << "KO" << RESET << "]" << std::endl;
			std::cerr << GREEN << "Usage: " << BOLDWHITE << "./webserv conf_file.conf <--verbose>\n" << RESET;	
			throw std::exception();
		}
		verbose = true;
	}
	return verbose;
}

int main(int argc, char **argv)
{
	bool		verbose = false;
	
	try
	{
		if (parse_argv(argc, argv))
			verbose = true;
		Parser parser(argv[1]);
		if (verbose)
			std::cout << parser << std::endl;
		Handler handler(parser.getBigVector(), parser.getBigLocation(), verbose);

		if (handler.launch_servers())
			handler.handle_servers();
	}
	catch (std::exception &e)
	{
		e.what();
	}
	return (0);
	//simul_request(parser.getBigLocation()[0], parser.getBigVector()[0]);
}
