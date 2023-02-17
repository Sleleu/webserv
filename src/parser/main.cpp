#include "../../header/parser/parser.hpp"
#include "../../header/utils/color.hpp"
#include "../../header/response/response.hpp"

int main(int argc, char **argv)
{
	(void)argc;
	(void)argv;
	Parser parser(argv[1]);
	simul_request();
}
