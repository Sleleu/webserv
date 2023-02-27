#include <iostream>

#ifndef SERVER_INCLUDES_HPP
#define SERVER_INCLUDES_HPP

//------------ DISPLAY -----------------
int		display_error(const std::string error_message);
void	display_ok(const std::string ok_message);
void	print_vector(std::vector<int> &vector);
//--------------------------------------

//------------ UTILS -------------------
std::string	convert_char_to_string(char *str);
//--------------------------------------

#endif