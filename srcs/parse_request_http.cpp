# include <iostream>
# include <map>

class HttpRequest
{
	public:
		HttpRequest(std::string request_method, std::string request_target, std::string protocol_version)
		: _request_method(request_method), _request_target(request_target), _protocol_version(protocol_version){}

		void	setHeaderField(std::string field_name, std::string field_value){
			_header_fields.insert(std::pair< std::string, std::string >(field_name, field_value));
		}
		void	setBody(std::string body){ _body = body; }

		std::string const getRequestMethod() const { return _request_method;}
		std::string const getRequestTarget() const { return _request_target;}
		std::string const getProtocolVersion() const { return _protocol_version;}

	private:
		std::string const						_request_method;
		std::string const						_request_target;
		std::string const						_protocol_version;
		std::map< std::string, std::string >	_header_fields;
		std::string 							_body;
};

std::string parse_request_http(std::string request_msg)
{
	std::string request_method(request_msg, 0, request_msg.find(" "));
	std::cout << request_method << std::endl;

	// HttpRequest();
	return (request_msg); //FAUX
}

int main ()
{
	std::string request_msg = "\
GET /index.html HTTP/1.1\n\
Host: localhost:8080\n\
Connection: keep-alive\n\
Cache-Control: max-age=0\n\
Upgrade-Insecure-Requests: 1\n\
User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_13_3) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/65.0.3325.162 Safari/537.36\n\
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8\n\
DNT: 1\n\
Accept-Encoding: gzip, deflate, br\n\
Accept-Language: en-US,en;q=0.9\n";

	parse_request_http(request_msg);
}
