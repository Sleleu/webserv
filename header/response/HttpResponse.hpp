#ifndef HTTRESPONSE_H
#define HTTRESPONSE_H

# include <iostream>
# include <map>
# include <string>
# include <vector>
# include <sstream>
# include <cstddef>
# include <ctime>
# include <fstream>
# include <sys/stat.h>
# include <algorithm>
# include "HttpRequest.hpp"

class HttpResponse
{
	public:
		typedef std::map<std::string, std::vector<std::string> > 	map_server;

		HttpResponse();

		void	setResponseInfo(HttpRequest & request, std::map< std::string, std::vector< std::string > > & serverMap);
		void	redirectTargetPath(std::string url);
		void	setCgi(HttpRequest const & request, std::map< std::string, std::vector< std::string > > & serverMap);
		void	setUpload(std::map< std::string, std::vector< std::string > > & serverMap);
		bool	findInCgiBin();

		std::string getResponseString();
		void		setHeader();
		std::vector<std::string> getPackets(map_server serverMap, std::string responseString, bool verbose);
		void		errorReturn();

		std::string getErrorPath() const;
		std::string getCode();
		std::string getStatus();
		std::string getTargetPath() const;
		std::string getCgiPath() const;
		std::string getUploadPath() const;
		std::string getIsUpload() const;
		std::string getExtension() const;
		std::string getRoot() const;

		void		setError(std::string code, std::string status);
		void		setCode(std::string content);
		void		setStatus(std::string content);
		void		setBody(std::string src);

		bool		isDirectory();
		bool		canUpload;
		bool		cgiUsed;
		bool		directoryListing;
		bool		errorPage;

		class BodySizeException : public std::exception {};
		class RedirectException : public std::exception {};

	private :

		std::string _targetPath;
		std::string _errorPath;
		std::string _cgiPath;
		std::string _uploadPath;
		std::string _isUpload;
		std::string _extension;
		std::string _root;
		std::string _accept;

		std::vector<std::string> _errorConf;

		std::map<std::string, std::string> 	_controlData;
		std::map<std::string, std::string> 	_headers;
		std::string							_body;
		std::string							_bodyError;

};


#endif
