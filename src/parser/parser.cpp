#include "../../header/parser/parser.hpp"
#include "../../header/utils/color.hpp"

Parser::Parser(std::string conf_file)
{
	try
	{
		std::cout << BOLDWHITE << "Reading conf file...\n" << RESET;
		fill_conf(conf_file);
		fill_vector();
		tcheck_listen();
	}
	catch (std::exception &e)
	{
		throw;
	}
}

Parser::~Parser()
{
}

void	Parser::fill_conf(std::string conf_file)
{
	std::fstream	conf_fd;
	std::string tmp_buff;

	if (!extension(conf_file))
	{
		std::cerr << "Bad extension";
		std::cerr << " [" << BOLDRED << "KO" << RESET << "]" << std::endl;
		throw std::exception();
	}
	std::cout << "Extension:";
	std::cout << " [" << BOLDGREEN << "OK" << RESET << "]" << std::endl;

	//Si le fichier est un directory le programme fonctionne ? ;

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
	int									server = 0;

	initDefaultVector();
	tmp_it = _conf.begin();
	end_it = _conf.end();
	while (tmp_it != end_it)
	{
		if (tmp_it->size() == 0 || tmp_it->c_str()[0] == '#');
		else if (*tmp_it == "server")
		{
			tmp_it ++;
			line ++;
			server ++;
			try
			{
				server_block_parsing(tmp_it, end_it, &line, server);
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
	if (server == 0)
		_parsingVector.push_back(initmap());
	std::cout << "Parsing stuct filled:";
	std::cout << " [" << BOLDGREEN << "OK" << RESET << "]" << std::endl;
}

void Parser::server_block_parsing(vector_iterator &tmp_it, vector_iterator &end_it, int *line, int server)
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
	_locationVector.push_back(std::map<std::string, std::map<std::string, std::vector<std::string> > >());
	tmp_it ++;
	(*line) ++;
	while (tmp_it != end_it && *tmp_it != "}" )
	{
		try
		{
			tmp_it = new_conf(*tmp_it, server, *line, tmp_it, end_it);
		}
		catch (std::exception &e)
		{
			throw;
		}
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
	map["redirect"] = _default_vec[REDIRECT];
	map["error"] = _default_vec[ERROR];
	map["method"] = _default_vec[METHOD];
	map["directory_listing"] = _default_vec[DIRECTORY_LISTING];
	map["cgi"] = _default_vec[CGI];
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
	std::vector<std::string>	redirect;
	std::vector<std::string>	error;
	std::vector<std::string>	method;
	std::vector<std::string>	directory_listing;
	std::vector<std::string>	cgi;
	std::vector<std::string>	default_file;
	std::vector<std::string>	upload_file;

	server_name.push_back("webserv");
	listen.push_back("8080");
	root.push_back("/");
	body_size.push_back("50000");
	redirect.push_back("");
	error.push_back("404");
	error.push_back("error.html");
	method.push_back("GET");
	method.push_back("POST");
	method.push_back("DELETE");
	directory_listing.push_back("off");
	cgi.push_back("");
	default_file.push_back("index.html");
	upload_file.push_back("off");

	_default_vec.push_back(server_name);
	_default_vec.push_back(listen);
	_default_vec.push_back(root);
	_default_vec.push_back(body_size);
	_default_vec.push_back(redirect);
	_default_vec.push_back(error);
	_default_vec.push_back(method);
	_default_vec.push_back(directory_listing);
	_default_vec.push_back(cgi);
	_default_vec.push_back(default_file);
	_default_vec.push_back(upload_file);
}

std::ostream	&operator<<(std::ostream &o, Parser &src)
{
	big_vector 					vec = src.getBigVector();
	big_location				_locationVector = src.getBigLocation();
	int							server_nbr = 0;
	std::vector<std::string>	tmp;
	std::vector<std::string>	all_string;
	map_vector					tmp_map;

	src.fill_vector_with_name(all_string);

	o << BOLDWHITE << "\nPrint of _parsingVector:" << RESET;
	for (big_vector::iterator tmp_it = vec.begin(); tmp_it != vec.end(); tmp_it ++)
	{
		server_nbr ++;
		tmp_map = *tmp_it;
		o << BOLDCYAN << "\n Server number " << server_nbr << ":\n" << RESET;
		o << YELLOW << "  Location: " << RED << "\"/\"" << RESET << ":\n";
		o << GREEN << "   std::map" << RESET << "\n   {\n";
		for (int i = 0; i < 11; i ++)
		{
			tmp = tmp_map[all_string[i]];
			o << "      [" << RED << "\"" << all_string[i] << "\"" << RESET << "], ";
			for (std::vector<std::string>::iterator it = tmp.begin(); it != tmp.end(); it ++)
			{
				o << "[" << RED << "\"" << *it << "\"" << RESET << "]";
			}
			o << "\n";
		}
		o << "   }\n";
		for (it_loca it_location= _locationVector[server_nbr - 1].begin(); it_location != _locationVector[server_nbr - 1].end(); it_location ++)
		{
			o << YELLOW << "\n  Location: " << RED << "\"" << it_location->first << "\"" << RESET << ":\n";
			o << GREEN << "   std::map" << RESET << "\n   {\n";
			for (map_vector::iterator map_it = _locationVector[server_nbr - 1][it_location->first].begin(); map_it != _locationVector[server_nbr - 1][it_location->first].end() ; map_it ++)
			{
				o << "      [" << RED << "\"" << map_it->first << "\"" << RESET << "], ";
				for (std::vector<std::string>::iterator it_loc = map_it->second.begin(); it_loc != map_it->second.end(); it_loc ++)
				{
					o << "[" << RED << "\"" << *it_loc << "\"" << RESET << "]";
				}
				o << "\n";
			}
			o << "   }\n";
		}
	}
	return o;
}

big_vector	&Parser::getBigVector(void)
{
	return (_parsingVector);
}

big_location	&Parser::getBigLocation(void)
{
	return (_locationVector);
}

void	Parser::fill_vector_with_name(std::vector<std::string> &vec)
{
	vec.push_back("server_name");
	vec.push_back("listen");
	vec.push_back("root");
	vec.push_back("body_size");
	vec.push_back("redirect");
	vec.push_back("error");
	vec.push_back("method");
	vec.push_back("directory_listing");
	vec.push_back("cgi");
	vec.push_back("default_file");
	vec.push_back("upload_file");
}

vector_iterator	&Parser::new_conf(std::string str, int server, int &line, vector_iterator &tmp_it, vector_iterator &end_it)
{
	int 					i = 0;
	int						skip = 0;
	int						add_conf = 0;
	std::string				token;
	std::string				left_token;
	map_vector::iterator	tmp_map_it;

	server --;
	if (str == "")
	{
		tmp_it ++;
		line ++;
		return (tmp_it);
	}
	while (str[i] == '\t' || str[i] == ' ')
		i ++;
	if (str[i] == '#')
	{
		tmp_it ++;
		line ++;
		return (tmp_it);
	}
	token = str.c_str() + i;
	skip += i;
	i = 0;
	while (token[i] && (token[i] != ' ' && token[i] != '\t'))
		i ++;
	try
	{
		left_token = token;
		left_token.erase(left_token.begin()+i, left_token.end());
		if (left_token == "location")
			return (location_bloc(tmp_it, server + 1, line, end_it));
	}
	catch (std::exception &e)
	{
		throw;
	}
	if (!token[i])
	{
		std::cerr << "Token " << RED << "\""<< token << "\"" << RESET << " need a right value line " << line;
		std::cerr << " [" << BOLDRED << "KO" << RESET << "]" << std::endl;
		throw std::exception();
	}
	token.erase(token.begin()+i, token.end());
	tmp_map_it = _parsingVector[server].find(token);
	if ( tmp_map_it == _parsingVector[server].end())
	{
		std::cerr << "Token " << RED << "\"" << token << "\"" << RESET << " isn't a validated token line " << line;
		std::cerr << " [" << BOLDRED << "KO" << RESET << "]" << std::endl;
		throw std::exception();
	}
	left_token = token;
	token = str.c_str() + i + 1 + skip;
	skip += i;
	i = 0;
	while (token[i])
	{
		while(token[i] && (token[i] == ' ' || token[i] == '\t'))
			i++;
		token.erase(0, i);
		skip += i;
		i = 0;
		while(token[i] && (token[i] != ' ' && token[i] != '\t'))
			i++;
		token.erase(i, token.size());
		if (token != "")
		{
			add_conf ++ ;
			if (add_conf == 1)
			{
				_parsingVector[server][left_token].clear();
				if (!pars_conf(left_token, token, line))
					throw std::exception();
			}
			_parsingVector[server][left_token].push_back(token);
			token = str.c_str() + i + skip + 1;
			skip += i;
			i = 0;
		}
	}
	if (add_conf == 0)
	{
		std::cerr << "Token " << RED << "\""<< left_token << "\"" << RESET << " need a right value line " << line;
		std::cerr << " [" << BOLDRED << "KO" << RESET << "]" << std::endl;
		throw std::exception();
	}
	tmp_it ++;
	line ++;
	return (tmp_it);
}

vector_iterator		&Parser::location_bloc(vector_iterator &tmp_it, int server, int &line, vector_iterator &end_it)
{
	std::string	tmp_str;
	std::string	location;
	int 		accolad;

	//if (_locationVector.size() == (unsigned long)server - 1)
	//	_locationVector.push_back(std::map<std::string, std::map<std::string, std::vector<std::string> > >());
	try
	{
		location = fill_location_path(*tmp_it, server,line);
		tmp_it ++;
		line ++;
	}
	catch (std::exception &e)
	{
		throw;
	}
	tmp_str = *tmp_it;
	int i = -1;
	while (tmp_str[++i] && (tmp_str[i] == ' ' || tmp_str[i] == '\t'));
	tmp_str.erase(tmp_str.begin(), tmp_str.end() - i);
	if (tmp_it == end_it || tmp_str != "{")
	{
		std::cerr << "\'{\' token expected after location token; line: " << line << ":";
		std::cerr << " [" << BOLDRED << "KO" << RESET << "]" << std::endl;
		throw std::exception();
	}
	accolad = line;
	tmp_it ++;
	line ++;
	tmp_str = *tmp_it;
	i = -1;
	while (tmp_str[++i] && (tmp_str[i] == ' ' || tmp_str[i] == '\t'));
	tmp_str.erase(tmp_str.begin(), tmp_str.end() - i);
	while (tmp_it != end_it && tmp_str != "}" )
	{
		try
		{
			new_conf_location(line, server, *tmp_it, location);
			tmp_it ++;
			line ++;
		}
		catch (std::exception &e)
		{
			throw;
		}
		tmp_str = *tmp_it;
		i = -1;
		while (tmp_str[++i] && (tmp_str[i] == ' ' || tmp_str[i] == '\t'));
		tmp_str.erase(tmp_str.begin(), tmp_str.end() - i);
	}
	if (tmp_it == end_it)
	{
		std::cerr << "\'}' token expected to match \'{\'; line: " << accolad << ":";
		std::cerr << " [" << BOLDRED << "KO" << RESET << "]" << std::endl;
		throw std::exception();
	}
	tmp_it ++;
	line ++;
	return (tmp_it);
}

std::string	Parser::fill_location_path(std::string str, int server, int line)
{
	int 		i = 0;
	int 		skip = 0;
	std::string	token;
	std::string	left_token;

	server --;
	while (str[i] == '\t' || str[i] == ' ')
		i ++;
	token = str.c_str() + i;
	skip += i;
	i = 0;
	while (token[i] && (token[i] != ' ' && token[i] != '\t'))
		i ++;
	if (!token[i])
	{
		std::cerr << "Token " << RED << "\""<< token << "\"" << RESET << " need a right value line " << line;
		std::cerr << " [" << BOLDRED << "KO" << RESET << "]" << std::endl;
		throw std::exception();
	}
	token.erase(token.begin()+i, token.end());
	left_token = token;
	token = str.c_str() + i + 1 + skip;
	skip += i;
	i = 0;
	while(token[i] && (token[i] == ' ' || token[i] == '\t'))
		i++;
	token.erase(0, i);
	skip += i;
	i = 0;
	if (!token[i])
	{
		std::cerr << "Token " << RED << "\""<< left_token << "\"" << RESET << " need a right value line " << line;
		std::cerr << " [" << BOLDRED << "KO" << RESET << "]" << std::endl;
		throw std::exception();
	}
	_locationVector[server][token];
	return token;
}

void	Parser::new_conf_location(int &line, int server, std::string str, std::string location)
{
	int 											i = 0;
	int												skip = 0;
	int												add_conf = 0;
	std::string										token;
	std::string										left_token;
	std::map<std::string, std::string>				check_token = init_map_token();
	std::map<std::string, std::string>::iterator	tmp_map_it;

	server --;
	if (str == "")
		return ;
	while (str[i] == '\t' || str[i] == ' ')
		i ++;
	if (str[i] == '#')
		return ;
	token = str.c_str() + i;
	skip += i;
	i = 0;
	while (token[i] && (token[i] != ' ' && token[i] != '\t'))
		i ++;
	if (!token[i])
	{
		std::cerr << "Token " << RED << "\""<< token << "\"" << RESET << " need a right value line " << line;
		std::cerr << " [" << BOLDRED << "KO" << RESET << "]" << std::endl;
		throw std::exception();
	}
	token.erase(token.begin()+i, token.end());
	tmp_map_it = check_token.find(token);
	if ( tmp_map_it == check_token.end())
	{
		std::cerr << "Token " << RED << "\"" << token << "\"" << RESET << " isn't a validated token line " << line;
		std::cerr << " [" << BOLDRED << "KO" << RESET << "]" << std::endl;
		throw std::exception();
	}
	left_token = token;
	token = str.c_str() + i + 1 + skip;
	skip += i;
	i = 0;
	while (token[i])
	{
		while(token[i] && (token[i] == ' ' || token[i] == '\t'))
			i++;
		token.erase(0, i);
		skip += i;
		i = 0;
		while(token[i] && (token[i] != ' ' && token[i] != '\t'))
			i++;
		token.erase(i, token.size());
		if (token != "")
		{
			add_conf ++ ;
			if (add_conf == 1)
			{
				_locationVector[server][location][left_token].clear();
				if (!pars_conf(left_token, token, line))
					throw std::exception();
			}
			_locationVector[server][location][left_token].push_back(token);
			token = str.c_str() + i + skip + 1;
			skip += i;
			i = 0;
		}
	}
	if (add_conf == 0)
	{
		std::cerr << "Token " << RED << "\""<< left_token << "\"" << RESET << " need a right value line " << line;
		std::cerr << " [" << BOLDRED << "KO" << RESET << "]" << std::endl;
		throw std::exception();
	}
}

std::map<std::string, std::string>	Parser::init_map_token()
{
	std::map<std::string, std::string>	map;
	map["method"];
	map["redirect"];
	map["root"];
	map["directory_listing"];
	map["default_file"];
	map["cgi"];
	map["upload_file"];
	return map;
}

bool	Parser::pars_conf(std::string token, std::string right_token, int line)
{
	if (token == "body_size")
	{
		if (!tcheck_size(right_token, token, line))
			return false;
	}
	else if (token == "root")
	{
		if (!tcheck_root(right_token, token, line))
			return false;
	}
	else if (token == "directory_listing" || token == "upload_file")
	{
		if (!tcheck_on_off(right_token, token, line))
			return false;
	}
	return true;
}

bool	Parser::tcheck_size(std::string token, std::string left_token, int line)
{
	for (int i = 0; token[i]; i ++)
	{
		if (!std::isdigit(token[i]))
		{
			std::cerr << "Token " << RED << "\""<< left_token << "\"" << RESET << " only accept digits characters, line:" << line;
			std::cerr << " [" << BOLDRED << "KO" << RESET << "]" << std::endl;
			return (false);
		}
	}
	if (token.size() > 5)
	{
		std::cerr << "Token " << RED << "\""<< left_token << "\"" << RESET << " can't be greater than 99 999, line:" << line;
		std::cerr << " [" << BOLDRED << "KO" << RESET << "]" << std::endl;
		return (false);
	}
	if (!std::atoi(token.c_str()))
	{
		std::cerr << "Token " << RED << "\""<< left_token << "\"" << RESET << " can't be null(0), line:" << line;
		std::cerr << " [" << BOLDRED << "KO" << RESET << "]" << std::endl;
		return (false);
	}
	return true;
}

bool	Parser::tcheck_root(std::string token, std::string left_token, int line)
{
	if (token[0] != '/')
	{
		std::cerr << "Token " << RED << "\""<< left_token << "\"" << RESET << " need begin with " << RED << "\"/\"" << RESET << " to match a destination, line:" << line;
		std::cerr << " [" << BOLDRED << "KO" << RESET << "]" << std::endl;
		return false;
	}
	return true;
}

bool	Parser::tcheck_on_off(std::string token, std::string left_token, int line)
{
	if (token == "off" || token == "on")
		return (true);
	std::cerr << "Token " << RED << "\""<< left_token << "\"" << RESET << " only accept";
	std::cerr << RED << " \"on\"" << RESET << " or " << RED << "\"off\"" << RESET << " token, line:" << line;
	std::cerr << " [" << BOLDRED << "KO" << RESET << "]" << std::endl;
	return false;
}

void	Parser::tcheck_listen(void)
{
	std::vector<std::string>	listen;
	std::vector<std::string>	tmp;
	std::string					port;


	for (big_vector::iterator it = _parsingVector.begin(); it != _parsingVector.end(); it ++)
	{
		port = (*it)["listen"][0];
		for (std::vector<std::string>::iterator i = listen.begin(); i != listen.end(); i ++)
		{
			if ((*i) == port)
			{
				std::cerr << "Can't handle same port ("<< RED << "\"" << port << "\"" << RESET << ") on two distinct server ";
				std::cerr << " [" << BOLDRED << "KO" << RESET << "]" << std::endl;
				throw std::exception();
			}
		}
		listen.push_back((*it)["listen"][0]);
	}
	std::cout << "Port parsing:";
	std::cout << " [" << BOLDGREEN << "OK" << RESET << "]" << std::endl;
}
