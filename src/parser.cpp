#include "../header/parser.hpp"
#include "../header/color.hpp"

Parser::Parser(std::string conf_file)
{
	try
	{
		std::cout << BOLDWHITE << "Reading conf file...\n" << RESET;
		fill_conf(conf_file);
		fill_vector();
	}
	catch (std::exception &e)
	{
	}
}

Parser::~Parser()
{
}

void	Parser::fill_conf(std::string conf_file)
{
	std::fstream	conf_fd;
	std::string tmp_buff;

	std::cout << "\nChecking extension name...\n";
	if (!extension(conf_file))
	{
		std::cerr << "Bad extension";
		std::cerr << " [" << BOLDRED << "KO" << RESET << "]" << std::endl;
		throw std::exception();
	}
	std::cout << "Extension:";
	std::cout << " [" << BOLDGREEN << "OK" << RESET << "]" << std::endl;

	//Si le fichier est un directory le programme fonctionne ? ;
	
	std::cout << "\nOpen Conf File...\n";
	conf_fd.open(conf_file.c_str(), std::ios::in);
	if (!conf_fd.is_open())
	{
		std::perror("Configuration file");
		std::cerr << "Open file:";
		std::cerr << " [" << BOLDRED << "KO" << RESET << "]" << std::endl;
		throw std::exception();
	}
	while (getline(conf_fd, tmp_buff))
		_conf.push_back(tmp_buff);
	std::cout << "File open:";
	std::cout << " [" << BOLDGREEN << "OK" << RESET << "]" << std::endl;
	conf_fd.close();
}

bool	Parser::extension(std::string file)
{
	int i = -1;

	while (file[++i] && file[i] != '.');
	if (file[i] != '.')
		return (0);
	i ++;
	if (!std::strncmp(file.c_str() + i, "conf", 5))
		return (1);
	return (0);

}

void	Parser::fill_vector(void)
{
	std::vector<std::string>::iterator	tmp_it;	
	std::vector<std::string>::iterator	end_it;	
	int 								line = 0;

	tmp_it = _conf.begin();
	end_it = _conf.end();
	std::cout << "\nParsing Conf File...\n";
	while (tmp_it != end_it)
	{
		if (tmp_it->size() == 0)
		{
			tmp_it ++;
			line ++;
		}
		else if (*tmp_it == "server")
			std::cout << "server detected\n";
		else
		{
			std::cerr << "Server token expected line: " << line << ":";
			std::cerr << " [" << BOLDRED << "KO" << RESET << "]" << std::endl;
			throw std::exception();
		}
		tmp_it ++;
		line ++;
	}
}
