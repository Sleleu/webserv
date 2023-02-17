#ifndef PARSER_HPP
#define PARSER_HPP

/*INTERROGATION ? 
- le fichier s'ouvre si c'est enfaite une directory
- parser apres les valerus dans mon vector de conf ? exemple: server_name plusieurs string || server "test ok" (with white space)
- meme port sur deux server diff ?
- ajouter commentaire hors bloc server
*/

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

typedef std::vector< std::map < std::string, std::vector<std::string> > >	big_vector;
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
	void		server_block_parsing(vector_iterator &, vector_iterator &, int *, int);
	map_vector	initmap();
	void		initDefaultVector(void);
	void		fill_vector_with_name(std::vector<std::string> &);
	big_vector	&getBigVector(void);
	void		new_conf(std::string, int, int line);

	private:
	std::vector< std::map < std::string, std::vector<std::string> > >	_parsingVector; //big_vector
	std::vector<std::string>											_conf;
	std::vector<std::vector<std::string> >								_default_vec;
	Parser();

};

std::ostream	&operator<<(std::ostream &o, Parser &src);

#endif
