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

class Parser
{

	public:
	Parser(std::string conf_file);
	~Parser();
	void	fill_conf(std::string);
	bool	extension(std::string);
	void	fill_vector(void);

/*class File_error : public std::exception
	{
		public:
		virtual const char* what() const throw();
	};*/	

	private:
	std::vector< std::map < std::string, std::vector<std::string > > >	_parsingVector;
	std::vector<std::string>											_conf;
	Parser();

};

#endif
