#include "pngr2_parse.hpp"

#include <iostream>

parser::parser_error::parser_error(): parser_error(no, "") {

}

parser::parser_error::parser_error(const error_type &p_type, const std::string &p_msg):
	type(p_type), msg(p_msg) {
}

int parser::get_int_from_token(const token_ptr &tok) {
	return std::dynamic_pointer_cast<int_token>(tok)->val;
}

double parser::get_dbl_from_token(const token_ptr &tok) {
	return std::dynamic_pointer_cast<dbl_token>(tok)->val;
}

bool parser::is_keyword_token(const token_ptr &tok, keyword_type kw) {
	return tok->type == token_type::KEYWORD &&
		(kw == keyword_type::BAD || std::dynamic_pointer_cast<keyword_token>(tok)->kw == kw);
}

bool parser::is_word_token(const token_ptr &tok) {
	return tok->type == token_type::WORD;
}

bool parser::is_op_token(const token_ptr &tok, op_type op) {
	return tok->type == token_type::OP &&
		(op == op_type::BAD || std::dynamic_pointer_cast<op_token>(tok)->op == op);
}

bool parser::is_num_token(const token_ptr &tok) {
	return is_int_token(tok) || is_dbl_token(tok);
}

bool parser::is_int_token(const token_ptr &tok) {
	return tok->type == token_type::INT;
}

bool parser::is_dbl_token(const token_ptr &tok) {
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
/*
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
*/

void parser::parse_stmt(impl::ins_queue_t &ins_queue, std::istream &istrm, token_ptr &tok, parser_error &err) {
	if (tok == nullptr) {	//	end-of-file
		err.type = parser::parser_error::eof;
		err.msg = "end of code";
		return;
	}
	else if (is_op_token(tok, op_type::SEMIC)) {	//	empty statement
		tok = get_token(istrm);
		return;
	}
	else {	//	expr;
		// parse_add_expr(instrs, istrm, tok);
		parse_expr_9(ins_queue, istrm, tok, err);
		if (is_op_token(tok, op_type::SEMIC)) {
			tok = get_token(istrm);
			return;
		}
		else {
			err.type = parser::parser_error::eof;
			err.msg = "expected semicolon";
		}
	}
}

void parser::parse_expr_9(impl::ins_queue_t &ins_queue, std::istream &istrm, token_ptr &tok, parser_error &err) {
	parse_expr_0(ins_queue, istrm, tok, err);
}

void parser::parse_expr_0(impl::ins_queue_t &ins_queue, std::istream &istrm, token_ptr &tok, parser_error &err) {
	if (is_int_token(tok)) {
		impl::add_ins_int_lit(ins_queue, get_int_from_token(tok));
		token_ptr tok = get_token(istrm);
	}
	else if (is_word_token(tok)) {
		impl::add_ins_acc(ins_queue, tok->lexeme);
		token_ptr tok = get_token(istrm);
	}
	else {
		err.type = parser::parser_error::err;
		err.msg = "expected literal or alphanumeric string";
	}
}

impl::ins_queue_t parser::parse(std::istream &istrm) {
	impl::parser_init();

	impl::ins_queue_t ins_queue = impl::ins_queue_new();
	token_ptr tok = get_token(istrm);
	parser_error err;

	while (err.type == parser_error::no) {
		parse_stmt(ins_queue, istrm, tok, err);
	}

	if (err.type != parser_error::no) {
		// give error
	}

	return ins_queue;
}
