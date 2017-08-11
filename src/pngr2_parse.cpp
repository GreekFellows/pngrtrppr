#include "pngr2_parse.hpp"

#include <iostream>

bool is_keyword_token(const std::shared_ptr<token> &tok, keyword_type kw) {
	return tok->type == token_type::KEYWORD &&
		(kw == keyword_type::BAD || std::dynamic_pointer_cast<keyword_token>(tok)->kw == kw);
}

bool is_op_token(const std::shared_ptr<token> &tok, op_type op) {
	return tok->type == token_type::OP &&
		(op == op_type::BAD || std::dynamic_pointer_cast<op_token>(tok)->op == op);
}

bool is_num_token(const std::shared_ptr<token> &tok) {
	return is_int_token(tok) || is_dbl_token(tok);
}

bool is_int_token(const std::shared_ptr<token> &tok) {
	return tok->type == token_type::INT;
}

bool is_dbl_token(const std::shared_ptr<token> &tok) {
	return tok->type == token_type::DBL;
}

/*
	stmt:
		add_expr ;
		;

	add_expr:
		add_expr + exp_expr
		exp_expr

	exp_expr:
		num ^ exp_expr
		num
*/

instr_ptr parse_stmt(std::vector<instr_ptr> &instrs, std::istream &istrm, token_ptr &tok) {
	if (tok == nullptr) {
		// end of code
		return NULL;
	}
	else if (is_op_token(tok, op_type::SEMIC)) {
		tok = get_token(istrm);
		// empty statement
		return NULL;
	}
	else {
		parse_add_expr(instrs, istrm, tok);
		if (is_op_token(tok, op_type::SEMIC)) {
			tok = get_token(istrm);
			// empty statement
			return NULL;
		}
		else {
			std::cerr << "stmt: expected ;" << std::endl;
			abort();
		}
	}
}

instr_ptr parse_add_expr(std::vector<instr_ptr> &instrs, std::istream &istrm, token_ptr &tok) {
	instr_ptr exp_expr_instr = parse_exp_expr(instrs, istrm, tok);

	while (is_op_token(tok, op_type::ADD)) {
		tok = get_token(istrm);
		instr_ptr exp_expr_2_instr = parse_exp_expr(instrs, istrm, tok);
		exp_expr_instr = (instr_ptr)nullptr; // exp_expr_instr + exp_expr_2_instr
		instrs.push_back(exp_expr_instr);
	}

	return exp_expr_instr;
}

instr_ptr parse_exp_expr(std::vector<instr_ptr> &instrs, std::istream &istrm, token_ptr &tok) {
	instr_ptr num_instr = parse_num(instrs, istrm, tok);

	if (is_op_token(tok, op_type::XOR)) {
		tok = get_token(istrm);
		instr_ptr exp_expr_instr = parse_exp_expr(instrs, istrm, tok);
		num_instr = (instr_ptr)nullptr; // num_instr ^ exp_expr_instr
		instrs.push_back(num_instr);
	}

	return num_instr;
}

instr_ptr parse_num(std::vector<instr_ptr> &instrs, std::istream &istrm, token_ptr &tok) {
	if (is_int_token(tok)) {
		tok = get_token(istrm);
		instr_ptr instr = (instr_ptr)nullptr;
		instrs.push_back(instr);
		return instr;
	}
	else if (is_dbl_token(tok)) {
		tok = get_token(istrm);
		instr_ptr instr = (instr_ptr)nullptr;
		instrs.push_back(instr);
		return instr;
	}
	else {
		std::cerr << "num: not a number" << std::endl;
		abort();
	}

	return NULL;
}

std::vector<instr_ptr> parse(std::istream &istrm) {
	std::vector<instr_ptr> instrs;
	token_ptr tok = get_token(istrm);
	parse_stmt(instrs, istrm, tok);
	return instrs;
}
