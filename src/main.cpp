
#include "pngr.hpp"

#include <string>
#include <fstream>
#include <streambuf>
#include <iostream>
#include <sstream>


//so that it is easier to switch between console and reading from text file
#define inputThroughConsole false

int main() {
#if !inputThroughConsole
    std::ifstream t("file.txt");
    std::string line((std::istreambuf_iterator<char>(t)),
                    std::istreambuf_iterator<char>());
#else
    std::string line;
#endif
    std::stringstream input;
    input << t.rdbuf();
    std::stringstream output;
#if inputThroughConsole
	while (std::getline(std::cin, line) && line != "quit") {
#endif
		auto tokens = lex(line);

		for (auto token : tokens) {
			token->str(std::cout);
            token->str(output);
			delete token;
		}
#if inputThroughConsole
    }
#endif
    // output to file
    std::ofstream o("output.txt");
    o << output.rdbuf();
}


// note for compilation: gcc -std=c++11