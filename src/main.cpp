#include "pngr.hpp"

#include <iostream>

int main() {
	std::string line;
	while (std::getline(std::cin, line) && line != "quit") {
		auto tokens = lex(line);

		for (auto token : tokens) {
			token->str(std::cout);
			delete token;
		}
	}
}