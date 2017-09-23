#include "pngr2_lex.hpp"
#include "pngr2_parse.hpp"

#include <iostream>
#include <fstream>
#include <sstream>

int main(int argc, char** argv) {
	for (int argi = 1; argi < argc; ++argi) {
		std::ifstream reader(argv[argi]);
		std::cout << "# instrs: " << parse(reader).size() << std::endl;
		/*for (std::shared_ptr<token> tok = get_token(reader); tok != nullptr; tok = get_token(reader)) {
			tok->str(std::cout);
		}*/
	}

	std::string s;
	while (std::getline(std::cin, s) && s != "quit") {
		std::istringstream iss(s);
		std::cout << "# instrs: " << parse(iss).size() << std::endl;
	}

	return 0;
}