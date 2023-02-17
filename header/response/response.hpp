#ifndef RESPONSE_H
#define RESPONSE_H

# include <iostream>
# include <map>
# include <string>
# include <vector>
# include <sstream>
# include <cstddef>
# include <algorithm>
# include "HttpRequest.hpp"
# include "HttpResponse.hpp"
# include <fstream>

// methods.cpp
void methodGET(HttpRequest const & request, HttpResponse  & response);
void methodPOST(HttpRequest const & request, HttpResponse  & response);
void methodDELETE(HttpRequest const & request, HttpResponse  & response);

// handle_request.cpp
void simul_request();
std::string handle_request(std::string const & requestMsg);

#endif
