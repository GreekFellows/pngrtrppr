#include "pngr2.hpp"

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
	else if (s == "-") {
		return op_type::SUB;
	}
	else if (s == "*") {
		return op_type::MUL;
	}
	else if (s == "/") {
		return op_type::DIV;
	}
	else if (s == "%") {
		return op_type::MOD;
	}
	else if (s == "=") {
		return op_type::ASSIGN;
	}
	else if (s == "+=") {
		return op_type::ADDASSIGN;
	}
	else if (s == "-=") {
		return op_type::SUBASSIGN;
	}
	else if (s == "*=") {
		return op_type::MULASSIGN;
	}
	else if (s == "/=") {
		return op_type::DIVASSIGN;
	}
	else if (s == "%=") {
		return op_type::MODASSIGN;
	}
	else if (s == "==") {
		return op_type::EQ;
	}
	else if (s == "<") {
		return op_type::LT;
	}
	else if (s == ">") {
		return op_type::GT;
	}
	else if (s == "<=") {
		return op_type::LE;
	}
	else if (s == ">=") {
		return op_type::GE;
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

bool may_be_op(const std::string &s) {
	static const std::vector<std::string> all_ops = {
		"+", "-", "*", "/", "%", "=",
		"+=", "-=", "*=", "/=", "%=",
		"==", "<", ">", "<=", ">=",
		"{", "}", "(", ")", ",", ";"
	};

	std::string::size_type len = s.size();
	for (const auto &op : all_ops) {
		if (op.substr(0u, len) == s) {
			return true;
		}
	}

	return false;
}

inline bool is_str_initial(const char &ch) {
	return ch == '"';
}

void skip_whitespace(std::istream &iss) {
	while (iss && isspace(iss.peek())) {
		iss.get();
	}
}

enum class get_token_phase_type {
	PEEK, WORD, NUM, OP, STR
};

std::shared_ptr<token> get_token(std::istream &iss) {
	// skip whitespace prior to code
	skip_whitespace(iss);

	get_token_phase_type phase = get_token_phase_type::PEEK;

	if (!iss) {
		return nullptr;
	}

	// peek to determine what type the token would be
	char ch = iss.peek();
	if (is_word_initial(ch)) {
		phase = get_token_phase_type::WORD;
	}
	else if (is_num_initial(ch)) {
		phase = get_token_phase_type::NUM;
	}
	else if (is_str_initial(ch)) {
		phase = get_token_phase_type::STR;
	}
	else if (is_op_initial(ch)) {
		phase = get_token_phase_type::OP;
	}
	else {
		// we need a better error handling mechanism
		return nullptr;
	}

	// read into token
	std::string buffer = "";
	int dq_count = 0; // doublequote counts, for str_token
	bool escaped = false; // whether the previous char was \ or not, for str_token
	bool int_not_dbl = true; // whether there is a decimal pt while lexing into an int_token / dbl_token

	// this loop will run at least once, since (iss) is guaranteed to be true on the first iteration
	while (iss) {
		ch = iss.peek();

		switch (phase) {
		case get_token_phase_type::WORD:
			if (is_word_medial(ch)) {
				buffer += ch;
			}
			else {
				if (get_keyword_type(buffer) != keyword_type::BAD) {
					return std::make_shared<keyword_token>(buffer, get_keyword_type(buffer));
				}
				else if (get_unit_type(buffer) != unit_type::BAD) {
					return std::make_shared<unit_token>(buffer, get_unit_type(buffer));
				}
				else {
					return std::make_shared<word_token>(buffer);
				}
			}
			break;

		// in the lexeme there is no distinction between enclosing "s and escaped "s
		// the only escapable characters are \ and "
		case get_token_phase_type::STR:
			if (ch == '\"') {
				if (escaped) {
					buffer += ch;
					escaped = false;
				}
				else {
					if (++dq_count >= 2) {
						iss.get(); // the final doublequote must be read from the input stringstream
						return std::make_shared<str_token>("\"" + buffer + "\"", buffer);
					}
				}
			}
			else if (ch == '\\') {
				if (escaped) {
					buffer += ch;
				}
				escaped = !escaped;
			}
			else {
				escaped = false; // maybe this should be an error
				buffer += ch;
			}
			break;

		case get_token_phase_type::NUM:
			if (is_num_medial(ch)) {
				if (ch == '.') {
					int_not_dbl = false;
				}
				buffer += ch;
			}
			else {
				if (int_not_dbl) {
					try {
						return std::make_shared<int_token>(buffer, std::stoi(buffer));
					}
					catch (...) {
						std::cerr << "bad integer" << std::endl;
						abort();
					}
				}
				else {
					try {
						return std::make_shared<dbl_token>(buffer, std::stod(buffer));
					}
					catch (...) {
						std::cerr << "bad double" << std::endl;
						abort();
					}
				}
			}
			break;

		case get_token_phase_type::OP:
			if (may_be_op(buffer + ch)) {
				buffer += ch;
			}
			else {
				return std::make_shared<op_token>(buffer, get_op_type(buffer));
			}
			break;
		}

		iss.get();
	}

	return nullptr;
}