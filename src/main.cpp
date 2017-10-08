//#include "pngr2_lex.hpp"
//#include "pngr2_parse.hpp"
#include "pngr2_instr.cpp"

#include <iostream>
#include <fstream>
#include <sstream>

int main(int argc, char** argv) {
	bytecode::runner_init();

	for (int argi = 1; argi < argc; ++argi) {
		std::ifstream reader(argv[argi]);
		bytecode::run(parser::parse(reader));
	}

	std::string s;
	while (std::getline(std::cin, s) && s != "quit") {
		std::istringstream iss(s);
		bytecode::run(parser::parse(iss));
	}

	return 0;
}
