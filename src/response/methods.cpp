#include "../../header/response/response.hpp"
#include "../../header/parser/parser.hpp"

void methodGET(HttpRequest const & request, HttpResponse & response)
{
    (void) request;
    (void) response;
    std::string targetPath = response.getTargetPath();
    std::ifstream ifs(targetPath.c_str());
    std::string targetContent;
    std::string tmp;
    while (std::getline(ifs, tmp))
        targetContent += tmp + "\n";
    ifs.close();
    response.setBody("\n" + targetContent);
};

void methodPOST(HttpRequest const & request, HttpResponse & response) {(void) request; (void) response;};
void methodDELETE(HttpRequest const & request, HttpResponse & response) {(void) request; (void) response;};
