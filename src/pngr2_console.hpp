#pragma once
#ifndef PNGR2_CONSOLE_HPP
#define PNGR2_CONSOLE_HPP

#include <string>

/*
	interpreter options

	-w		wild guesswork
	-b		parse by bytecode
	-c		parse correctly
	-u		stupid undefined behavior
	-r		remind logical error possibilities
*/

class console {
public:

	struct syntax_error {
		bool is_error;
		std::string msg;

		syntax_error();
		syntax_error(const bool &p_is_error, const std::string &p_msg);
	};

	struct runtime_error {
		bool is_error;
		std::string msg;

		runtime_error();
		runtime_error(const bool &p_is_error, const std::string &p_msg);
	};

	static void msg_welcome();
	static bool get_input(std::istream &istrm, std::string &s);

	static void msg_parsing_done();
	static void msg_execution_done();

	static void msg_syntax_error(const syntax_error &err);
	static void msg_runtime_error(const runtime_error &err);
};

#endif
