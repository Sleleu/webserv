#include "../../header/response/response.hpp"
#include "../../header/parser/parser.hpp"
#include "../../header/utils/color.hpp"

void acceptCgi()
{
}

void methodGET(HttpRequest const & request, HttpResponse & response)
{
    (void) request;
    std::cout << BOLDYELLOW << " GET" << RESET;

    std::string targetPath = response.getTargetPath();
    std::ifstream targetStream(targetPath.c_str());
    if (!targetStream.is_open())
    {
        response.setError("404", "Not Found");
        throw std::exception();
    }
    if (response.cgiUsed)
    {
	    std::cout << MAGENTA << " CGI used" << RESET;
        targetStream.close();
        acceptCgi();
        response.setBody(BODY_CGI); // FOR TEST
        return ;
    }
    std::string targetContent;
    std::string tmp;
    while (std::getline(targetStream, tmp))
        targetContent += tmp + "\n";
    targetStream.close();
    response.setBody("\n" + targetContent);
};

void methodPOST(HttpRequest const & request, HttpResponse & response)
{
    (void) request;
    (void) response;
    std::cout << "POST" << std::endl;
    // std::cout << BOLDYELLOW << " POST" << RESET;

    // std::string targetPath = response.getTargetPath();
    // const char* path = targetPath.c_str();
    // struct stat s;
    // if (!response.canUpload || stat(path, &s) == 0)
    // {
    //     response.setError("403", "Forbidden");
    //     throw std::exception();
    // }
    // std::ofstream newFile(path);
    // if (!newFile.is_open())
    // {
    //     response.setError("404", "Not Found");
    //     throw std::exception();
    // }
    // newFile << request.getBody();
    // newFile.close();
    // response.setError("201", "Created");
}

void methodDELETE(HttpRequest const & request, HttpResponse & response) //Pas de protection ? ACCESS ?
{
    (void) request;
    std::cout << BOLDYELLOW << " DELETE" << RESET;

    std::string targetPath = response.getTargetPath();
    const char* path = targetPath.c_str();
    struct stat s;
    if (stat(path, &s) != 0)
    {
        response.setError("404", "Not Found");
        throw std::exception();
    }
    if (std::remove(response.getTargetPath().c_str()) != 0)
    {
		response.setError("500", "Internal Server Error");
        throw std::exception();
    }
    // response.setError("204", "No Content");
    response.setError("200", "OK");
}

void acceptMethod(HttpRequest const & request, HttpResponse & response,\
std::map< std::string, std::vector< std::string > > locationInfo)
{
    (void)locationInfo;
	void (*useMethod[3])(HttpRequest const & request, HttpResponse & response) = {methodGET, methodPOST, methodDELETE};

    std::vector<std::string> acceptedMethods = locationInfo["method"];
    if (std::find(acceptedMethods.begin(), acceptedMethods.end(), request.getMethod()) == acceptedMethods.end())
    {
        response.setError("405", "Method Not Allowed");
        throw std::exception();
    }
    std::string possibleMethods[3] = {"GET", "POST", "DELETE"};
	for (int i = 0; i < 3; i++)
		if (possibleMethods[i] == request.getMethod())
		{
        	useMethod[i](request, response);
            return ;
        }
    response.setError("405", "Method Not Allowed");
    throw std::exception();
}
