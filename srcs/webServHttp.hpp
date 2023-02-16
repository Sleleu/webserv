#ifndef WEBSERVHTTP_H
#define WEBSERVHTTP_H

# include <iostream>
# include <map>
# include <string>
# include <vector>
# include <sstream>
# include <cstddef>
# include <algorithm>
# include "httpRequest.hpp"
# include "httpResponse.hpp"
# include <fstream>

// methods.cpp
void methodGET(HttpRequest const & request, HttpResponse  & response);
void methodPOST(HttpRequest const & request, HttpResponse  & response);
void methodDELETE(HttpRequest const & request, HttpResponse  & response);

// handle_request.cpp
std::string handle_request(std::string const & requestMsg);

#endif