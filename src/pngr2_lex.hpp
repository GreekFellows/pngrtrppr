#pragma once

#ifndef PNGR2_LEX_HPP
#define PNGR2_LEX_HPP

#include <string>
#include <memory>
#include <istream>
#include <vector>

// tokenizer

enum class token_type {
	BAD, KEYWORD, WORD, OP, INT, DBL, UNIT, STR
};

enum class keyword_type {
	BAD, DEF, FOR, IN, IF, ELSE, WHILE, TARGET, FROM, TO,
	WRITE, WRITELN
};

enum class op_type : uint8_t {
	BAD, ADD, SUB, MUL, DIV, MOD, ASSIGN,
	ADDASSIGN, SUBASSIGN, MULASSIGN, DIVASSIGN, MODASSIGN,
	EQ, INEQ, LT, GT, LE, GE,
	AND, OR, XOR, NOT,
	LBRACE, RBRACE, LPAREN, RPAREN, COMMA, SEMIC
};

// are there more?
enum class unit_type {
	BAD, PT, MM, CM, IN, EX, EM, BP, PC, DD, CC, ND, NC, SP
};

struct token {
	std::string lexeme;
	token_type type;

	token(const std::string&, token_type);
	virtual void str(std::ostream&);
};

struct keyword_token : public token {
	keyword_type kw;
	keyword_token(const std::string&, keyword_type);
	void str(std::ostream&);
};

struct word_token : public token {
	word_token(const std::string&);
	void str(std::ostream&);
};

struct op_token : public token {
	op_type op;
	op_token(const std::string&, op_type);
	void str(std::ostream&);
};

struct int_token : public token {
	int val;
	int_token(const std::string&, const int&);
	void str(std::ostream&);
};

struct dbl_token : public token {
	double val;
	dbl_token(const std::string&, const double&);
	void str(std::ostream&);
};

struct unit_token : public token {
	unit_type unit;
	unit_token(const std::string&, unit_type);
	void str(std::ostream&);
};

struct str_token : public token {
	std::string val;
	str_token(const std::string&, const std::string&);
	void str(std::ostream&);
};

using token_ptr = std::shared_ptr<token>;

keyword_type get_keyword_type(const std::string&);
op_type get_op_type(const std::string&);
unit_type get_unit_type(const std::string&);

bool is_word_initial(const char&);
bool is_word_medial(const char&);
bool is_num_initial(const char&);
bool is_num_medial(const char&);
bool is_op_initial(const char&);
bool may_be_op(const std::string&);
bool is_str_initial(const char&);
void skip_whitespace(std::istream&);

token_ptr get_token(std::istream&);

#endif
