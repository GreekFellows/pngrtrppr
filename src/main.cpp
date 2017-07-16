#include "pngr2.hpp"

#include <iostream>
#include <fstream>
#include <sstream>

int main(int argc, char** argv) {
	for (int argi = 1; argi < argc; ++argi) {
		std::ifstream reader(argv[argi]);
		for (std::shared_ptr<token> tok = get_token(reader); tok != nullptr; tok = get_token(reader)) {
			tok->str(std::cout);
		}
	}

	for (std::shared_ptr<token> tok = get_token(std::cin); tok != nullptr; tok = get_token(std::cin)) {
		tok->str(std::cout);
	}

	return 0;
}