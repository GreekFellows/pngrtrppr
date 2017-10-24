#include "pngr2_parse.hpp"

#include <iostream>

bool parser::get_int_from_token(const token_ptr &tok, int &i) {
	if (tok) {
		i = std::dynamic_pointer_cast<int_token>(tok)->val;
		return true;
	}
	return false;
}

bool parser::get_dbl_from_token(const token_ptr &tok, double &d) {
	if (tok) {
		d = std::dynamic_pointer_cast<dbl_token>(tok)->val;
		return true;
	}
	return false;
}

bool parser::get_str_from_token(const token_ptr &tok, std::string &s) {
	if (tok) {
		s = std::dynamic_pointer_cast<str_token>(tok)->val;
		return true;
	}
	return false;
}

bool parser::is_keyword_token(const token_ptr &tok, keyword_type kw) {
	return tok && tok->type == token_type::KEYWORD &&
		(kw == keyword_type::BAD || std::dynamic_pointer_cast<keyword_token>(tok)->kw == kw);
}

bool parser::is_word_token(const token_ptr &tok) {
	return tok && tok->type == token_type::WORD;
}

bool parser::is_op_token(const token_ptr &tok, op_type op) {
	return tok && tok->type == token_type::OP &&
		(op == op_type::BAD || std::dynamic_pointer_cast<op_token>(tok)->op == op);
}

bool parser::is_num_token(const token_ptr &tok) {
	return is_int_token(tok) || is_dbl_token(tok);
}

bool parser::is_int_token(const token_ptr &tok) {
	return tok && tok->type == token_type::INT;
}

bool parser::is_dbl_token(const token_ptr &tok) {
	return tok && tok->type == token_type::DBL;
}

bool parser::is_str_token(const token_ptr &tok) {
	return tok && tok->type == token_type::STR;
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

bool parser::parse_stmt(impl::ins_queue_t &ins_queue, std::istream &istrm, token_ptr &tok, console::syntax_error &err) {
	if (tok == nullptr) {	//	end-of-file
		err = console::syntax_error(true, "");	// empty error msg = eof
		return true;
	}
	else if (is_op_token(tok, op_type::SEMIC)) {	//	empty statement
		tok = get_token(istrm);
		return true;
	}
	else {	//	expr;
		if (parse_expr_9(ins_queue, istrm, tok, err)) {
			if (is_op_token(tok, op_type::SEMIC)) {
				tok = get_token(istrm);
				return true;
			}
			else {
				err = console::syntax_error(true, "expected a semicolon");
				return false;
			}
		}
		else {
			return false;
		}
	}
}

bool parser::parse_expr_9(impl::ins_queue_t &ins_queue, std::istream &istrm, token_ptr &tok, console::syntax_error &err) {
	if (parse_expr_8(ins_queue, istrm, tok, err)) {
		if (is_op_token(tok, op_type::ASSIGN) ||
			is_op_token(tok, op_type::ADDASSIGN) ||
			is_op_token(tok, op_type::SUBASSIGN) ||
			is_op_token(tok, op_type::MULASSIGN) ||
			is_op_token(tok, op_type::DIVASSIGN) ||
			is_op_token(tok, op_type::MODASSIGN)
			) {
			tok = get_token(istrm);
			if (parse_expr_9(ins_queue, istrm, tok, err)) {
				impl::add_ins_binop(ins_queue, get_op_type(tok->lexeme));
				return true;
			}

			err = console::syntax_error(true, "expected an expression");
			return false;
		}

		return true;
	}

	err = console::syntax_error(true, "expected an expression");
	return false;
}

bool parser::parse_expr_8(impl::ins_queue_t &ins_queue, std::istream &istrm, token_ptr &tok, console::syntax_error &err) {
	return parse_expr_0(ins_queue, istrm, tok, err);
}

bool parser::parse_expr_0(impl::ins_queue_t &ins_queue, std::istream &istrm, token_ptr &tok, console::syntax_error &err) {
	if (is_int_token(tok)) {
		int i;
		if (get_int_from_token(tok, i)) {
			impl::add_ins_int_lit(ins_queue, i);
			tok = get_token(istrm);
			return true;
		}

		err = console::syntax_error(true, "could not get value of int_token");
		return false;
	}
	else if (is_dbl_token(tok)) {
		double d;
		if (get_dbl_from_token(tok, d)) {
			impl::add_ins_dbl_lit(ins_queue, d);
			tok = get_token(istrm);
			return true;
		}

		err = console::syntax_error(true, "could not get value of dbl_token");
		return false;
	}
	else if (is_str_token(tok)) {
		std::string s;
		if (get_str_from_token(tok, s)) {
			impl::add_ins_str_lit(ins_queue, s);
			tok = get_token(istrm);
			return true;
		}

		err = console::syntax_error(true, "could not get value of str_token");
		return false;
	}
	else if (is_word_token(tok)) {
		if (tok->lexeme == "true" || tok->lexeme == "false") {
			impl::add_ins_bool_lit(ins_queue, tok->lexeme == "true");
			tok = get_token(istrm);
			return true;
		}

		impl::add_ins_acc(ins_queue, tok->lexeme);
		tok = get_token(istrm);
		return true;
	}
	else if (is_op_token(tok, op_type::LPAREN)) {
		tok = get_token(istrm);
		if (parse_expr_9(ins_queue, istrm, tok, err)) {
			if (is_op_token(tok, op_type::RPAREN)) {
				tok = get_token(istrm);
				return true;
			}

			err = console::syntax_error(true, "expected a rightparen");
			return false;
		}

		err = console::syntax_error(true, "expected an expression");
		return false;
	}

	err = console::syntax_error(true, "expected a literal or alphanumeric string");
	return false;
}

impl::ins_queue_t parser::parse(std::istream &istrm) {
	impl::parser_init();

	impl::ins_queue_t ins_queue = impl::ins_queue_new();
	token_ptr tok = get_token(istrm);
	console::syntax_error err;

	while (!err.is_error) {
		parse_stmt(ins_queue, istrm, tok, err);
	}

	if (err.msg != "") {
		console::msg_syntax_error(err);
	}
	else {
		console::msg_parsing_done();
	}

	return ins_queue;
}
