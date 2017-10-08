#pragma once

#ifndef PNGR2_PARSE_HPP
#define PNGR2_PARSE_HPP

#include "pngr2_lex.hpp"
#include "pngr2_bytecode.hpp"

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

// we use the bytecode implementation; there may also be a struct implementation
using impl = bytecode;

class parser {
public:
	using token_ptr = std::shared_ptr<token>;

private:
	struct parser_error {
		enum error_type {
			no, eof, err
		} type;
		std::string msg;

		parser_error();
		parser_error(const error_type &p_is_error, const std::string &p_msg);
	};

	static int get_int_from_token(const token_ptr &tok);
	static double get_dbl_from_token(const token_ptr &tok);

	static bool is_keyword_token(const token_ptr &tok, keyword_type = keyword_type::BAD);
	static bool is_word_token(const token_ptr &tok);
	static bool is_op_token(const token_ptr &tok, op_type = op_type::BAD);
	static bool is_num_token(const token_ptr &tok);
	static bool is_int_token(const token_ptr &tok);
	static bool is_dbl_token(const token_ptr &tok);
	/*
	bool parse_stmt(std::vector<instr_ptr>&, std::istream&, token_ptr&);
	bool parse_add_expr(std::vector<instr_ptr>&, std::istream&, token_ptr&);
	bool parse_exp_expr(std::vector<instr_ptr>&, std::istream&, token_ptr&);
	bool parse_num(std::vector<instr_ptr>&, std::istream&, token_ptr&);*/

	/*
		the following functions parse_*() take the same arguments
		where tok is the pointer holding the current token.
	*/

	static void parse_stmt(impl::ins_queue_t &ins_queue, std::istream &istrm, token_ptr &tok, parser_error &err);

	static void parse_expr_9(impl::ins_queue_t &ins_queue, std::istream &istrm, token_ptr &tok, parser_error &err);
	static void parse_expr_0(impl::ins_queue_t &ins_queue, std::istream &istrm, token_ptr &tok, parser_error &err);

public:
	static impl::ins_queue_t parse(std::istream &istrm);

};

#endif
