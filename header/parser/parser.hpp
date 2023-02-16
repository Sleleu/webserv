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

typedef std::vector<std::string>::iterator vector_iterator;
typedef	std::map<std::string, std::vector<std::string> > map_vector;

#define SERVER_NAME 0
#define LISTEN 1
#define ROOT 2
#define BODY_SIZE 3
#define LOCATION 4
#define ERROR 5
#define METHOD 6
#define DIRECTORY_LISTING 7
#define GCI 8
#define DEFAULT_FILE_IF_REQUEST_IS_DIRECTORY 9
#define UPLOAD_FILE 10

class Parser
{

	public:
	Parser(std::string conf_file);
	~Parser();
	void		fill_conf(std::string);
	bool		extension(std::string);
	void		fill_vector(void);
	void		server_block_parsing(vector_iterator &, vector_iterator &, int *);
	map_vector	initmap();
	void		initDefaultVector(void);

	private:
	std::vector< std::map < std::string, std::vector<std::string> > >	_parsingVector;
	std::vector<std::string>											_conf;
	std::vector<std::vector<std::string> >								_default_vec;
	Parser();

};

#endif
