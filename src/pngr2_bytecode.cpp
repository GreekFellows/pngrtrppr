#include "pngr2_bytecode.hpp"

#include <iostream>

std::deque<std::string> bytecode::accs;
std::deque<var_ptr> bytecode::lits;
std::unordered_map<std::string, std::deque<var_id_t>> bytecode::syms;
std::unordered_map<var_id_t, var_ptr> bytecode::reg;
std::deque<var_id_t> bytecode::var_stack;
console::runtime_error bytecode::runtime_err;

void bytecode::add_acc(const std::string &new_acc) {
	accs.push_back(new_acc);
}

std::string bytecode::get_acc(const size_t &i) {
	if (i < accs.size()) {
		return accs[i];
	}
	else {
		if (!runtime_err.is_error) {
			runtime_err = console::runtime_error(true, "get_acc: no id token with index " + std::to_string(i) + " does not exist");
		}
		return "";
	}
}

void bytecode::add_lit(const var_ptr &new_lit) {
	lits.push_back(new_lit);
}

var_ptr bytecode::get_lit(const size_t &i) {
	if (i < lits.size()) {
		return lits[i];
	}
	else {
		if (!runtime_err.is_error) {
			runtime_err = console::runtime_error(true, "get_lit: no literal token with index " + std::to_string(i) + " does not exist");
		}
		return nullptr;
	}
}

void bytecode::add_sym(const std::string &new_sym, const var_id_t &new_var_id) {
	try {
		syms.at(new_sym);
	}
	catch (std::out_of_range) {
		syms.insert({
			new_sym,
			std::deque<var_id_t>()
		});
	}
	syms.at(new_sym).push_back(new_var_id);
}

void bytecode::del_sym(const std::string &new_sym) {
	try {
		syms.at(new_sym).pop_back();
	}
	catch (std::out_of_range) {
		if (!runtime_err.is_error) {
			runtime_err = console::runtime_error(true, "del_sym: symbol " + new_sym + " does not exist");
		}
	}
}

var_id_t bytecode::get_sym(const std::string &str) {
	try {
		return syms.at(str).back();
	}
	catch (std::out_of_range) {
		if (!runtime_err.is_error) {
			runtime_err = console::runtime_error(true, "get_sym: symbol " + str + " does not exist");
		}
	}
	return var::var_id_zero;
}

void bytecode::add_var(const var_id_t &var_id, const var_ptr &p_var) {
	reg.insert_or_assign(var_id, p_var);
}

var_ptr bytecode::get_var(const var_id_t &var_id) {
	try {
		return reg.at(var_id);
	}
	catch (std::out_of_range) {
		if (!runtime_err.is_error) {
			runtime_err = console::runtime_error(true, "get_var: variable with id " + std::to_string(var_id) + " does not exist");
		}
	}
	return nullptr;
}

void bytecode::var_stack_push(const var_id_t &var_id) {
	var_stack.push_back(var_id);
}

var_id_t bytecode::var_stack_pop() {
	if (var_stack.empty()) {
		if (!runtime_err.is_error) {
			runtime_err = console::runtime_error(true, "var_stack_pop: variable stack is empty");
		}
		return var::var_id_zero;
	}

	var_id_t ret = var_stack.back();
	var_stack.pop_back();
	return ret;
}

var_id_t bytecode::new_var_id() {
	static var_id_t counter = 0, squared = 0;
	counter += (var_id_t)std::pow(squared++, 2);
	return counter;
}

size_t bytecode::get_ins_num_params(const ins_type &ins) {
	size_t i = 0;

	switch (ins) {
	case ins_type::acc:
	case ins_type::lit:
		i = num_ins_in_var_id;
		break;
	case ins_type::unop:
	case ins_type::binop:
		i = 1;
		break;
	}

	return i;
}

bytecode::ins_queue_t bytecode::var_id_to_ins(const var_id_t &p_id) {
	ins_queue_t queue = ins_queue_new();
	queue.push_back((p_id >> 8) & 0x000000ff);	// most significant 8 bits first in
	queue.push_back(p_id & 0x000000ff);			// least significant 8 bits next in
	return queue;
}

var_id_t bytecode::ins_to_var_id(ins_queue_t p_id) {
	ins_t most_i = p_id.front();
	p_id.pop_front();
	ins_t least_i = p_id.front();
	p_id.pop_front();
	return (most_i << 8) & least_i;
}

void bytecode::extend_back(ins_queue_t &to, ins_queue_t &what) {
	while (!what.empty()) {
		to.push_back(what.front());
		what.pop_front();
	}
}

void bytecode::parser_init() {
	accs = std::deque<std::string>();
	lits = std::deque<var_ptr>();
}

bytecode::ins_queue_t bytecode::ins_queue_new() {
	return ins_queue_t();
}

void bytecode::ins_queue_enq(ins_queue_t &ins_queue, const ins_t &ins) {
	ins_queue.push_back(ins);
}

bytecode::ins_t bytecode::ins_queue_deq(ins_queue_t &ins_queue) {
	ins_t ret = ins_queue.front();
	ins_queue.pop_front();
	return ret;
}

void bytecode::add_ins_acc(ins_queue_t &ins_queue, const std::string &acc_id) {
	size_t count = accs.size();
	add_acc(acc_id);

	ins_queue.push_back(ins_type::acc);

	ins_queue_t acc_id_queue = var_id_to_ins(count);
	extend_back(ins_queue, acc_id_queue);
}

void bytecode::add_ins_int_lit(ins_queue_t &ins_queue, const int &val) {
	size_t count = lits.size();
	add_lit(var::make_int(0, val));

	ins_queue.push_back(ins_type::lit);

	ins_queue_t lit_id_queue = var_id_to_ins(count);
	extend_back(ins_queue, lit_id_queue);
}

void bytecode::add_ins_dbl_lit(ins_queue_t &ins_queue, const double &val) {
	size_t count = lits.size();
	add_lit(var::make_dbl(0, val));

	ins_queue.push_back(ins_type::lit);

	ins_queue_t lit_id_queue = var_id_to_ins(count);
	extend_back(ins_queue, lit_id_queue);
}

void bytecode::add_ins_str_lit(ins_queue_t &ins_queue, const std::string &val) {
	size_t count = lits.size();
	add_lit(var::make_str(0, val));

	ins_queue.push_back(ins_type::lit);

	ins_queue_t lit_id_queue = var_id_to_ins(count);
	extend_back(ins_queue, lit_id_queue);
}

void bytecode::add_ins_bool_lit(ins_queue_t &ins_queue, bool val) {
	size_t count = lits.size();
	add_lit(var::make_bool(0, val));

	ins_queue.push_back(ins_type::lit);

	ins_queue_t lit_id_queue = var_id_to_ins(count);
	extend_back(ins_queue, lit_id_queue);
}

void bytecode::add_ins_unop(ins_queue_t &ins_queue, const op_type &op) {
	ins_queue.push_back(ins_type::unop);
	ins_queue.push_back((ins_t)op);
}

void bytecode::add_ins_binop(ins_queue_t &ins_queue, const op_type &op) {
	ins_queue.push_back(ins_type::binop);
	ins_queue.push_back((ins_t)op);
}

void bytecode::runner_init() {
	syms = std::unordered_map<std::string, std::deque<var_id_t>>();
	reg = std::unordered_map<var_id_t, var_ptr>();
	var_stack = std::deque<var_id_t>();
	runtime_err = console::runtime_error();
}

void bytecode::run(ins_queue_t &ins_queue) {
	runtime_err = console::runtime_error();

	ins_type current_ins;
	ins_queue_t current_queue;

	while (!ins_queue.empty() && !runtime_err.is_error) {
		current_queue = fetch_ins(ins_queue, current_ins);

		if (current_ins == ins_type::acc) {
			var_stack_push(get_sym(get_acc(ins_to_var_id(current_queue))));
		}
		else if (current_ins == ins_type::lit) {
			auto v = get_lit(ins_to_var_id(current_queue));
			var_id_t new_id = new_var_id();
			add_var(new_id, v);
			var_stack_push(new_id);
		}
		else if (current_ins == ins_type::unop) {
			op_type op = (op_type)current_queue.front();
			var::unary_func_ptr_t unary_func_ptr = var::get_unary_func_ptr(op);
			if (unary_func_ptr) {
				var_id_t lhs = var_stack_pop();
				var_id_t new_id = new_var_id();
				add_var(new_id, (*unary_func_ptr)(get_var(lhs), true));
				var_stack_push(new_id);
			}
			else {
				if (!runtime_err.is_error) {
					runtime_err = console::runtime_error(true, "get_unary_func: unary operator is undefined");
				}
			}
		}
		else if (current_ins == ins_type::binop) {
			op_type op = (op_type)current_queue.front();
			var::binary_func_ptr_t binary_func_ptr = var::get_binary_func_ptr(op);
			if (binary_func_ptr) {
				var_id_t lhs = var_stack_pop();
				var_id_t rhs = var_stack_pop();
				var_id_t new_id = new_var_id();

				add_var(new_id, (*binary_func_ptr)(get_var(lhs), get_var(rhs), true));
				var_stack_push(new_id);
			}
			else {
				if (!runtime_err.is_error) {
					runtime_err = console::runtime_error(true, "get_binary_func: binary operator is undefined");
				}
			}
		}
	}

	if (runtime_err.is_error) {
		console::msg_runtime_error(runtime_err);
	}
	
	console::msg_execution_done();
}

bytecode::ins_queue_t bytecode::fetch_ins(ins_queue_t &ins_queue, ins_type &first_ins) {
	ins_queue_t first_queue = bytecode::ins_queue_new();

	first_ins = (ins_type)ins_queue.front();
	ins_queue.pop_front();

	for (size_t i = 0, s = get_ins_num_params(first_ins); i != s; ++i) {
		first_queue.push_back(ins_queue.front());
		ins_queue.pop_front();
	}

	return first_queue;
}
