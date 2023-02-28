#ifndef RESPONSE_H
#define RESPONSE_H

# include <iostream>
# include <map>
# include <string>
# include <vector>
# include <sstream>
# include <cstddef>
# include <algorithm>
# include <fstream>
# include "body.hpp"
# include "HttpRequest.hpp"
# include "HttpResponse.hpp"
# include "../../header/server/Server.hpp"

// handle_request.cpp
void simul_request(std::map< std::string, std::map< std::string, std::vector<std::string> > > & locationMap,\
std::map < std::string, std::vector<std::string> > defaultMap );
// std::string handle_request(std::string const & requestMsg);
std::string get_response( std::string const requestMsg,\
std::map< std::string, std::map< std::string, std::vector<std::string> > > & locationMap,\
std::map < std::string, std::vector<std::string> > defaultMap, bool verbose );

// methods.cpp
void methodGET(HttpRequest & request, HttpResponse  & response);
void methodPOST(HttpRequest & request, HttpResponse  & response);
void methodDELETE(HttpRequest & request, HttpResponse  & response);
void acceptMethod(HttpRequest & request, HttpResponse & response,\
std::map< std::string, std::vector< std::string > > locationInfo);

//parsing_request.cpp
bool fileExist(std::string & pathString);
std::map< std::string, std::vector< std::string > > getLocationInfo( HttpRequest & request,\
std::map< std::string, std::map< std::string, std::vector<std::string> > > & serverMap );
std::map< std::string, std::vector< std::string > > getServerMap (std::map< std::string, std::vector< std::string > > locationMap,\
std::map< std::string, std::vector< std::string > > defaultMap);
std::string parseBody(HttpRequest & request);
//response_utils.cpp
template <typename T>
std::string toString(T value)
{
	std::stringstream ss;
	ss << value;
	std::string str = ss.str();
	return str;
}

#endif
