#include "../../header/response/response.hpp"
#include "../../header/parser/parser.hpp"
#include "../../header/utils/color.hpp"

void methodGET(HttpRequest const & request, HttpResponse & response)
{
    std::cout << BOLDYELLOW << " GET" << RESET;
    (void) request;
    (void) response;
    std::string targetPath = response.getTargetPath();
    std::ifstream ifs(targetPath.c_str());
    if (!ifs.is_open())
        return ; // <---- Erreur ??
    std::string targetContent;
    std::string tmp;
    while (std::getline(ifs, tmp))
        targetContent += tmp + "\n";
    ifs.close();
    response.setBody("\n" + targetContent);
};

void methodPOST(HttpRequest const & request, HttpResponse & response)
{
    std::cout << BOLDYELLOW << " POST" << RESET;
    (void) request; (void) response;
    // std::string targetPath = response.getTargetPath();
    // std::ifstream ifs(targetPath.c_str());
    // if (ifs.is_open())
    // {
    //     ifs.close();
    //     return ; // <---- Erreur ??
    // }

    // std::ofstream ofs(targetPath.c_str());

}
void methodDELETE(HttpRequest const & request, HttpResponse & response)
{
    std::cout << BOLDYELLOW << " DELETE" << RESET;
    (void) request; (void) response;
}

void acceptMethod(HttpRequest const & request, HttpResponse & response,\
std::map< std::string, std::vector< std::string > > locationInfo)
{
    (void)locationInfo;
	void (*useMethod[3])(HttpRequest const & request, HttpResponse & response) = {methodGET, methodPOST, methodDELETE};

    std::vector<std::string> acceptedMethods = locationInfo["method"];
    if (std::find(acceptedMethods.begin(), acceptedMethods.end(), request.getMethod()) == acceptedMethods.end())
        throw std::exception();
    std::string possibleMethods[3] = {"GET", "POST", "DELETE"};
	for (int i = 0; i < 3; i++)
		if (possibleMethods[i] == request.getMethod())
		{
        	useMethod[i](request, response);
            return ;
        }
    throw std::exception();
}
