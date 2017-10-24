#include "pngr2_lex.hpp"
#include "pngr2_parse.hpp"
#include "pngr2_console.hpp"

#include <iostream>
#include <fstream>
#include <sstream>

int main(int argc, char** argv) {
	impl::runner_init();

	console::msg_welcome();

	for (int argi = 1; argi < argc; ++argi) {
		std::ifstream reader(argv[argi]);
		impl::run(parser::parse(reader));
	}

	std::string s;
	while (console::get_input(std::cin, s) && s != "quit") {
		std::istringstream iss(s);
		impl::run(parser::parse(iss));
	}

	return 0;
}
