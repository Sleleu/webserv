#include "../../header/parser/parser.hpp"
#include "../../header/utils/color.hpp"

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
	int 								line = 1;

	initDefaultVector();
	tmp_it = _conf.begin();
	end_it = _conf.end();
	std::cout << "\nParsing Conf File...\n";
	while (tmp_it != end_it)
	{
		if (tmp_it->size() == 0);
		else if (*tmp_it == "server")
		{
			tmp_it ++;
			line ++;
			try
			{
				server_block_parsing(tmp_it, end_it, &line);
			}
			catch (std::exception &e)
			{
				throw;
			}
		}
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

void Parser::server_block_parsing(vector_iterator &tmp_it, vector_iterator &end_it, int *line)
{
	int accolad;

	if (tmp_it == end_it || *tmp_it != "{")
	{
		std::cerr << "\'{\' token expected after server; line: " << *line << ":";
		std::cerr << " [" << BOLDRED << "KO" << RESET << "]" << std::endl;
		throw std::exception();
	}
	accolad = *line;
	_parsingVector.push_back(initmap());
	while (tmp_it != end_it && *tmp_it != "}" )
	{
		tmp_it ++;
		(*line) ++;
	}
	if (tmp_it == end_it)
	{
		std::cerr << "\'}' token expected to match \'{\'; line: " << accolad << ":";
		std::cerr << " [" << BOLDRED << "KO" << RESET << "]" << std::endl;
		throw std::exception();
	}
}

map_vector Parser::initmap(void)
{
	map_vector 					map;

	//map[server_name] = 
}

void	Parser::initDefaultVector(void)
{

	//finir de remplir les vecteur pour apres aller initialiser la map par default
	std::vector<std::string>	server_name;
	std::vector<std::string>	listen;
	std::vector<std::string>	root;
	std::vector<std::string>	body_size;
	std::vector<std::string>	location;
	std::vector<std::string>	error;
	std::vector<std::string>	method;
	std::vector<std::string>	directory_listing;
	std::vector<std::string>	gci;
	std::vector<std::string>	default_file;
	std::vector<std::string>	host;

	_default_vec[SERVER_NAME] = webserv
}
