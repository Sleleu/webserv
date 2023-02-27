#ifndef PARSER_HPP
#define PARSER_HPP

#include <iostream>
#include <string>
#include <iostream>
#include <filesystem>
#include <fstream>
#include <cerrno>
#include <stdexcept>
#include <vector>
#include <map>
#include <algorithm>
#include <cstring>
#include <sys/stat.h>

typedef std::vector< std::map < std::string, std::vector<std::string> > >	big_vector;
typedef std::vector<std::string>::iterator vector_iterator;
typedef	std::map<std::string, std::vector<std::string> > map_vector;
typedef std::vector<std::map<std::string, std::map<std::string, std::vector<std::string> > > > big_location;
typedef std::map<std::string, std::map<std::string, std::vector<std::string> > >::iterator it_loca;

#define SERVER_NAME 0
#define LISTEN 1
#define ROOT 2
#define BODY_SIZE 3
#define REDIRECT 4
#define ERROR 5
#define METHOD 6
#define DIRECTORY_LISTING 7
#define CGI 8
#define DEFAULT_FILE_IF_REQUEST_IS_DIRECTORY 9
#define UPLOAD_FILE 10

class Parser
{

	public:
	Parser(std::string conf_file);
	~Parser();
	void								fill_conf(std::string);
	std::string							fill_location_path(std::string, int server, int line);
	void								fill_vector(void);
	void								fill_vector_with_name(std::vector<std::string> &);
	big_vector							&getBigVector(void);
	big_location						&getBigLocation(void);
	bool								extension(std::string);
	void								server_block_parsing(vector_iterator &, vector_iterator &, int *, int);
	bool								pars_conf(std::string, std::string, int);
	bool								tcheck_size(std::string, std::string, int);
	bool								tcheck_root(std::string, std::string, int);
	bool								tcheck_on_off(std::string, std::string, int);
	void								tcheck_listen();
	vector_iterator						&new_conf(std::string, int, int &line, vector_iterator &, vector_iterator &);
	void								new_conf_location(int &line, int server, std::string str, std::string);
	vector_iterator						&location_bloc(vector_iterator &, int, int &, vector_iterator &);
	std::map<std::string, std::string>	init_map_token();
	map_vector							initmap();
	void								initDefaultVector(void);

	private:
	std::vector< std::map < std::string, std::vector<std::string> > >						_parsingVector; //big_vector
	std::vector<std::string>																_conf;
	std::vector<std::vector<std::string> >													_default_vec;
	std::vector<std::map<std::string, std::map<std::string, std::vector<std::string> > > >	_locationVector;
	std::map<std::string, std::string>														_check_location_token;
	Parser();
};

std::ostream	&operator<<(std::ostream &o, Parser &src);

#endif
