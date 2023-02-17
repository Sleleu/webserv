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
void acceptMethod(HttpRequest const & request, HttpResponse & response,\
std::map< std::string, std::vector< std::string > > locationInfo);

// handle_request.cpp
void simul_request();
std::string handle_request(std::string const & requestMsg);

//parsing_request.cpp
std::map< std::string, std::vector< std::string > > getLocationInfo( HttpRequest & request,\
std::map< std::string, std::map< std::string, std::vector<std::string> > > & serverMap );

#endif
