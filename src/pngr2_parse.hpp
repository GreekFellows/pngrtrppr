#pragma once

#ifndef PNGR2_PARSE_HPP
#define PNGR2_PARSE_HPP

#include "pngr2_lex.hpp"
#include "pngr2_bytecode.hpp"
#include "pngr2_console.hpp"

#include <vector>

// this language better be LL(1)

/*

syntax

deffile = target "path" from "line";

write expr1 expr2 ... to f;
for x in (0, 10, 2) {}
while (i == 8) {}

x = 0;
x = 0.8;
x = "dtom";
x = target "me" from "top to bottom";

x = int(0.8);

// much operator precedence hierarchy

*/

class parser {
public:

	using token_ptr = std::shared_ptr<token>;

private:

	static bool get_int_from_token(const token_ptr &tok, int &i);
	static bool get_dbl_from_token(const token_ptr &tok, double &d);
	static bool get_str_from_token(const token_ptr &tok, std::string &s);

	static bool is_keyword_token(const token_ptr &tok, keyword_type = keyword_type::BAD);
	static bool is_word_token(const token_ptr &tok);
	static bool is_op_token(const token_ptr &tok, op_type = op_type::BAD);
	static bool is_num_token(const token_ptr &tok);
	static bool is_int_token(const token_ptr &tok);
	static bool is_dbl_token(const token_ptr &tok);
	static bool is_str_token(const token_ptr &tok);
	/*
	bool parse_stmt(std::vector<instr_ptr>&, std::istream&, token_ptr&);
	bool parse_add_expr(std::vector<instr_ptr>&, std::istream&, token_ptr&);
	bool parse_exp_expr(std::vector<instr_ptr>&, std::istream&, token_ptr&);
	bool parse_num(std::vector<instr_ptr>&, std::istream&, token_ptr&);*/

	/*
		the following functions parse_*() take the same arguments
		where tok is the pointer holding the current token.
	*/

	static bool parse_stmt(impl::ins_queue_t &ins_queue, std::istream &istrm, token_ptr &tok, console::syntax_error &err);

	static bool parse_expr_9(impl::ins_queue_t &ins_queue, std::istream &istrm, token_ptr &tok, console::syntax_error &err);
	static bool parse_expr_8(impl::ins_queue_t &ins_queue, std::istream &istrm, token_ptr &tok, console::syntax_error &err);
	static bool parse_expr_0(impl::ins_queue_t &ins_queue, std::istream &istrm, token_ptr &tok, console::syntax_error &err);

public:
	static impl::ins_queue_t parse(std::istream &istrm);

};

#endif
