#include "pngr2_console.hpp"

#include <iostream>
#include "rang.hpp"

console::syntax_error::syntax_error() : syntax_error(false, "") {}
console::syntax_error::syntax_error(const bool &p_is_error, const std::string &p_msg) : is_error(p_is_error), msg(p_msg) {}

console::runtime_error::runtime_error() : runtime_error(false, "") {}
console::runtime_error::runtime_error(const bool &p_is_error, const std::string &p_msg) : is_error(p_is_error), msg(p_msg) {}

void console::msg_welcome() {
	std::cout
		<< rang::style::bold << rang::fg::cyan << "pngrtrppr"
		<< rang::style::reset << " | v0.0.0\n";
}

bool console::get_input(std::istream &istrm, std::string &s) {
	std::cout << " >> ";
	std::getline(istrm, s);
	return (bool)istrm;
}

void console::msg_syntax_error(const console::syntax_error &err) {
	std::cout
		<< rang::style::bold << rang::fg::red << "syntax error"
		<< rang::style::reset << " | " << err.msg << "\n";
}

void console::msg_parsing_done() {
	std::cout
		<< rang::style::bold << rang::fg::green << "parsing done.\n" << rang::style::reset;
}

void console::msg_runtime_error(const console::runtime_error &err) {
	std::cout
		<< rang::style::bold << rang::fg::red << "runtime error"
		<< rang::style::reset << " | " << err.msg << "\n";
}

void console::msg_execution_done() {
	std::cout
		<< rang::style::bold << rang::fg::green << "execution done.\n" << rang::style::reset;
}
