#include "pngr.hpp"

#include <cctype>
#include <iostream>

token::token(const std::string &p_lexeme, token_type p_type) :
	lexeme(p_lexeme), type(p_type) {}
keyword_token::keyword_token(const std::string &p_lexeme, keyword_type p_kw) :
	token(p_lexeme, token_type::KEYWORD), kw(p_kw) {}
word_token::word_token(const std::string &p_lexeme) :
	token(p_lexeme, token_type::WORD) {}
op_token::op_token(const std::string &p_lexeme, op_type p_op) :
	token(p_lexeme, token_type::OP), op(p_op) {}
int_token::int_token(const std::string &p_lexeme, const int &p_val) :
	token(p_lexeme, token_type::INT), val(p_val) {}
dbl_token::dbl_token(const std::string &p_lexeme, const double &p_val) :
	token(p_lexeme, token_type::DBL), val(p_val) {}
unit_token::unit_token(const std::string &p_lexeme, unit_type p_unit) :
	token(p_lexeme, token_type::UNIT), unit(p_unit) {}
str_token::str_token(const std::string &p_lexeme, const std::string &p_val) :
	token(p_lexeme, token_type::STR), val(p_val) {}

void token::str(std::ostream &os) {
	os << "token, lexeme: " << lexeme << std::endl;
}

void keyword_token::str(std::ostream &os) {
	os << "kw_token, lexeme: " << lexeme << std::endl;
}

void word_token::str(std::ostream &os) {
	os << "word_token, lexeme: " << lexeme << std::endl;
}

void op_token::str(std::ostream &os) {
	os << "op_token, lexeme: " << lexeme << std::endl;
}

void int_token::str(std::ostream &os) {
	os << "int_token, lexeme: " << lexeme << ", val: " << val << std::endl;
}

void dbl_token::str(std::ostream &os) {
	os << "dbl_token, lexeme: " << lexeme << ", val: " << val << std::endl;
}

void unit_token::str(std::ostream &os) {
	os << "unit_token, lexeme: " << lexeme << std::endl;
}

void str_token::str(std::ostream &os) {
	os << "str_token, lexeme: " << lexeme << ", val: " << val << std::endl;
}

keyword_type get_keyword_type(const std::string &s) {
	if (s == "def") {
		return keyword_type::DEF;
	}
	else if (s == "for") {
		return keyword_type::FOR;
	}
	else if (s == "in") {
		return keyword_type::IN;
	}
	else if (s == "if") {
		return keyword_type::IF;
	}
	else if (s == "else") {
		return keyword_type::ELSE;
	}

	return keyword_type::BAD;
}

op_type get_op_type(const std::string &s) {
	if (s == "+") {
		return op_type::ADD;
	}
	else if (s == "=") {
		return op_type::ASSIGN;
	}
	else if (s == "{") {
		return op_type::LBRACE;
	}
	else if (s == "}") {
		return op_type::RBRACE;
	}
	else if (s == "(") {
		return op_type::LPAREN;
	}
	else if (s == ")") {
		return op_type::RPAREN;
	}
	else if (s == "`") {
		return op_type::EXPR;
	}
	else if (s == ",") {
		return op_type::COMMA;
	}
	else if (s == ";") {
		return op_type::SEMIC;
	}

	return op_type::BAD;
}

unit_type get_unit_type(const std::string &s) {
	if (s == "cm") {
		return unit_type::CM;
	}

	return unit_type::BAD;
}

inline bool is_word_initial(const char &ch) {
	return ch == '_' || std::isalpha(ch);
}

inline bool is_word_medial(const char &ch) {
	return is_word_initial(ch) || std::isdigit(ch);
}

inline bool is_num_initial(const char &ch) {
	return ch == '.' || std::isdigit(ch);
}

inline bool is_num_medial(const char &ch) {
	return is_num_initial(ch);
}

inline bool is_op_initial(const char &ch) {
	static const std::string op_chs = "+-*/%=><{}(),;`";
	return op_chs.find(ch) != std::string::npos;
}

inline bool is_op_medial(const char &ch) {
	return is_op_initial(ch);
}

inline bool is_str_initial(const char &ch) {
	return ch == '"';
}

void skip_whitespace(std::string::const_iterator &iter, const std::string::const_iterator &end) {
	while (iter != end && isspace(*iter)) {
		++iter;
	}
}

enum class phase_type {
	DECIDE, WORD, NUM, OP, STR
};

std::list<token*> lex(const std::string &line) {
	auto iter = line.cbegin(), end = line.cend();

	// skip whitespace prior to code
	skip_whitespace(iter, end);

	phase_type phase = phase_type::DECIDE;

	std::list<token*> tokens;
	std::string buffer = "";
	bool int_not_dbl, escape;
	char quote_count;

	while (iter != end) {
		bool incr = false;
		char ch = *iter;

		switch (phase) {
		case phase_type::DECIDE:
			skip_whitespace(iter, end);
			ch = *iter;
			buffer = "";
			if (is_word_initial(ch)) {
				phase = phase_type::WORD;
			}
			else if (is_num_initial(ch)) {
				int_not_dbl = true;
				phase = phase_type::NUM;
			}
			else if (is_str_initial(ch)) {
				quote_count = 0;
				escape = false;
				phase = phase_type::STR;
			}
			else if (is_op_initial(ch)) {
				phase = phase_type::OP;
			}
			else {
				std::cerr << "bad character" << std::endl;
				abort();
			}
			break;

		case phase_type::WORD:
			if (is_word_medial(ch)) {
				incr = true;
				buffer += ch;
			}
			else {
				if (get_keyword_type(buffer) != keyword_type::BAD) {
					tokens.push_back(new keyword_token(buffer, get_keyword_type(buffer)));
				}
				else if (get_unit_type(buffer) != unit_type::BAD) {
					tokens.push_back(new unit_token(buffer, get_unit_type(buffer)));
				}
				else {
					tokens.push_back(new word_token(buffer));
				}
				phase = phase_type::DECIDE;
			}
			break;

		case phase_type::NUM:
			if (is_num_medial(ch)) {
				incr = true;
				if (ch == '.') {
					int_not_dbl = false;
				}
				buffer += ch;
			}
			else {
				if (int_not_dbl) {
					try {
						tokens.push_back(new int_token(buffer, std::stoi(buffer)));
					}
					catch (...) {
						std::cerr << "bad integer" << std::endl;
						abort();
					}
				}
				else {
					try {
						tokens.push_back(new dbl_token(buffer, std::stod(buffer)));
					}
					catch (...) {
						std::cerr << "bad double" << std::endl;
						abort();
					}
				}
				phase = phase_type::DECIDE;
			}
			break;

		case phase_type::STR:
			incr = true;
			// add escaping rules
			if (ch == '"') {
				if (++quote_count >= 2) {
					tokens.push_back(new str_token("\"" + buffer + "\"", buffer));
					phase = phase_type::DECIDE;
				}
			}
			else {
				buffer += ch;
			}
			break;

		case phase_type::OP:
			if (is_op_medial(ch)) {
				incr = true;
				buffer += ch;
			}
			else {
				tokens.push_back(new op_token(buffer, get_op_type(buffer)));
				phase = phase_type::DECIDE;
			}
			break;
		}

		if (incr) {
			++iter;
		}
	}

	if (!buffer.empty()) {
		switch (phase) {
		case phase_type::WORD:
			if (get_keyword_type(buffer) != keyword_type::BAD) {
				tokens.push_back(new keyword_token(buffer, get_keyword_type(buffer)));
			}
			else if (get_unit_type(buffer) != unit_type::BAD) {
				tokens.push_back(new unit_token(buffer, get_unit_type(buffer)));
			}
			else {
				tokens.push_back(new word_token(buffer));
			}
			break;

		case phase_type::NUM:
			if (int_not_dbl) {
				try {
					tokens.push_back(new int_token(buffer, std::stoi(buffer)));
				}
				catch (...) {
					std::cerr << "bad integer" << std::endl;
					abort();
				}
			}
			else {
				try {
					tokens.push_back(new dbl_token(buffer, std::stod(buffer)));
				}
				catch (...) {
					std::cerr << "bad double" << std::endl;
					abort();
				}
			}
			break;

		case phase_type::OP:
			tokens.push_back(new op_token(buffer, get_op_type(buffer)));
			break;
		}
	}

	return tokens;
}
