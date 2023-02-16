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
	std::cout << *this;
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

	map["server_name"] = _default_vec[SERVER_NAME];
	map["listen"] = _default_vec[LISTEN];
	map["root"] = _default_vec[ROOT];
	map["body_size"] = _default_vec[BODY_SIZE];
	map["location"] = _default_vec[LOCATION];
	map["error"] = _default_vec[ERROR];
	map["method"] = _default_vec[METHOD];
	map["directory_listing"] = _default_vec[DIRECTORY_LISTING];
	map["gci"] = _default_vec[GCI];
	map["default_file"] = _default_vec[DEFAULT_FILE_IF_REQUEST_IS_DIRECTORY];
	map["upload_file"] = _default_vec[UPLOAD_FILE];
	return (map);
}

void	Parser::initDefaultVector(void)
{
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
	std::vector<std::string>	upload_file;

	server_name.push_back("webserv");
	listen.push_back("8080");
	root.push_back("/html/");
	body_size.push_back("1");
	location.push_back("");
	error.push_back("/html/error.html");
	method.push_back("GET");
	method.push_back("POST");
	method.push_back("DELETE");
	directory_listing.push_back("off");
	gci.push_back("");
	default_file.push_back("/html/index.html");
	upload_file.push_back("off");

	_default_vec.push_back(server_name);
	_default_vec.push_back(listen);
	_default_vec.push_back(root);
	_default_vec.push_back(body_size);
	_default_vec.push_back(location);
	_default_vec.push_back(error);
	_default_vec.push_back(method);
	_default_vec.push_back(directory_listing);
	_default_vec.push_back(gci);
	_default_vec.push_back(default_file);
	_default_vec.push_back(upload_file);
}

std::ostream	&operator<<(std::ostream &o, Parser &src)
{
	big_vector vec = src.getBigVector();
	int	server_nbr = 0;
	std::vector<std::string>	tmp;
	std::vector<std::string>	all_string;
	map_vector					tmp_map;
	
	src.fill_vector_with_name(all_string);

	o << BOLDWHITE << "\nPrint of _parsingVector:" << RESET;
	for (big_vector::iterator tmp_it = vec.begin(); tmp_it != vec.end(); tmp_it ++)
	{
		server_nbr ++;
		tmp_map = *tmp_it;
		o << BOLDCYAN << "\nServer number " << server_nbr << ":\n" << RESET;
		o << GREEN << "std::map" << RESET << "{\n";
		for (int i = 0; i < 11; i ++)
		{
			tmp = tmp_map[all_string[i]];
			o << "[" << RED << "\"" << all_string[i] << "\"" << RESET << "], ";
			for (std::vector<std::string>::iterator it = tmp.begin(); it != tmp.end(); it ++)
			{
				o << "[" << RED << "\"" << *it << "\"" << RESET << "]";
			}
			o << "\n";
		}
		o << "}\n";
	}
	return o;
}

big_vector	&Parser::getBigVector(void)
{
	return (_parsingVector);
}

void	Parser::fill_vector_with_name(std::vector<std::string> &vec)
{
	vec.push_back("server_name");
	vec.push_back("listen");
	vec.push_back("root");
	vec.push_back("body_size");
	vec.push_back("location");
	vec.push_back("error");
	vec.push_back("method");
	vec.push_back("directory_listing");
	vec.push_back("gci");
	vec.push_back("default_file");
	vec.push_back("upload_file");
}
