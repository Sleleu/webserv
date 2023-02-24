#ifndef DIRECTORY_INDEX_HPP
#define DIRECTORY_INDEX_HPP

# include <unistd.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <iostream>
# include <map>
# include <string>
# include <map>
# include <vector>
# include <sstream>
# include <fstream>
# include "HttpResponse.hpp"
# include "body.hpp"
# include "../utils/colors.hpp"
# include <cstring>
# include <dirent.h>

#define _FILE 0
#define _DIR 1

std::string	dir_list(char * relativePath, char * completePath);
std::string	html_error(std::string error);
std::string	creat_html(std::map<std::string, int>, std::string);

#endif
