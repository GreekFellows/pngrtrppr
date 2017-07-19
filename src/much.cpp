// http://en.cppreference.com/w/cpp/language/operator_precedence

/*

	stmt:
		expr_stmt

	expr_stmt:
		expr;

	expr:
		factor + expr
		factor - expr
		factor

	factor:
		term * factor
		term / factor
		term

	term:
		(expr)
		VALUE
		name

*/

// a = 3;
// a + 4;

using reg_index_type = unsigned int;
static reg_index_type next_id = 0;

std::unordered_map<std::string, reg_index_type> user_def_map;
std::unordered_map<reg_index_type, std::shared_ptr<var>> tmp_reg;

reg_index_type make_tmp(int i) {
	reg_index_type new_index;
	new_index.is_tmp = true;
	new_index.id = ++next_id;
	return new_index;
}

struct base {
};

struct expr_stmt : public base {
	std::shared_ptr<expr> rhs;

	void execute() {
		rhs->execute();
	}
};

struct expr : public base {
	enum class exec_type {
		ADD, MINUS, ITSELF
	} type;

	std::shared_ptr<factor> lhs;
	std::shared_ptr<expr> rhs;

	reg_index_type execute() {
		reg_index_type lhs_index = lhs->execute();

		if (type == exec_type::ITSELF) {
			// expr: factor
			return lhs_index;
		}
		else if (type == exec_type::ADD) {
			// expr: factor + expr
			reg_index_type rhs_index = rhs->execute();
			// a level of type checking
			return make_tmp(tmp_reg[lhs_index].field.i + tmp_reg[rhs_index].field.i);
		}
		else if (type == exec_type::MINUS) {
			// expr: factor - expr
			reg_index_type rhs_index = rhs->execute();
			// a level of type checking
			return make_tmp(tmp_reg[lhs_index].field.i - tmp_reg[rhs_index].field.i);
		}
	}
};

struct factor : public expr {
};

struct term : public factor {
};

struct name : public expr {
	std::string var_name;

	reg_index_type execute() {
		return user_def_map[var_name];
	}
};