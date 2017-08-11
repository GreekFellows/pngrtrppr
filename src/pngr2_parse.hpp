#pragma once

#ifndef PNGR2_PARSE_HPP
#define PNGR2_PARSE_HPP

#include "pngr2_lex.hpp"

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

using instr_ptr = int; // dummy; instr must have a null value

using token_ptr = std::shared_ptr<token>;

bool is_keyword_token(const token_ptr&, keyword_type = keyword_type::BAD);
bool is_op_token(const token_ptr&, op_type = op_type::BAD);
bool is_num_token(const token_ptr&);
bool is_int_token(const token_ptr&);
bool is_dbl_token(const token_ptr&);

instr_ptr parse_stmt(std::vector<instr_ptr>&, std::istream&, token_ptr&);
instr_ptr parse_add_expr(std::vector<instr_ptr>&, std::istream&, token_ptr&);
instr_ptr parse_exp_expr(std::vector<instr_ptr>&, std::istream&, token_ptr&);
instr_ptr parse_num(std::vector<instr_ptr>&, std::istream&, token_ptr&);

std::vector<instr_ptr> parse(std::istream&);

#endif
