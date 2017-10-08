#include "pngr2_bytecode.hpp"

#include <iostream>

std::deque<std::string> bytecode::accs;
std::deque<var_ptr> bytecode::lits;
std::unordered_map<std::string, std::deque<var_id_t>> bytecode::syms;
std::unordered_map<var_id_t, var_ptr> bytecode::reg;
std::deque<var_id_t> bytecode::var_stack;

void bytecode::add_acc(const std::string &new_acc) {
	accs.push_back(new_acc);
}

std::string bytecode::get_acc(const size_t &i) {
	if (i < accs.size()) {
		return accs[i];
	}
	else {
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
		syms.at(new_sym);
	}
	catch (std::out_of_range) {
		return;
	}
	syms.at(new_sym).pop_back();
}

var_id_t bytecode::get_sym(const std::string &str) {
	return syms[str].back();
}

void bytecode::add_var(const var_id_t &var_id, const var_ptr &p_var) {
	reg.insert_or_assign(var_id, p_var);
}

var_ptr bytecode::get_var(const var_id_t &var_id) {
	try {
		return reg.at(var_id);
	}
	catch (std::out_of_range) {
		return nullptr;
	}
}

void bytecode::var_stack_push(const var_id_t &var_id) {
	var_stack.push_back(var_id);
}

var_id_t bytecode::var_stack_pop() {
	var_id_t ret = var_stack.back();
	var_stack.pop_back();
	return ret;
}

var_id_t bytecode::new_var_id() {
	static var_id_t counter = 0, squared = 0;
	counter += std::pow(squared++, 2);
	return counter;
}

size_t bytecode::get_ins_num_params(const ins_type &ins) {
	size_t i = 0;
	switch (ins) {
	case ins_type::acc:
	case ins_type::lit:
		i = 1;
		break;
	}
	return i;
}

bytecode::ins_queue_t bytecode::var_id_to_ins(const var_id_t &p_id) {
	ins_queue_t queue = ins_queue_new();
	queue.push_back(p_id);
	return queue;
}

var_id_t bytecode::ins_to_var_id(ins_queue_t p_id) {
	return p_id.front();
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

void bytecode::runner_init() {
	syms = std::unordered_map<std::string, std::deque<var_id_t>>();
	reg = std::unordered_map<var_id_t, var_ptr>();
	var_stack = std::deque<var_id_t>();
}

void bytecode::run(ins_queue_t &ins_queue) {
	while (!ins_queue.empty()) {
		ins_type current_ins;
		ins_queue_t current_queue = fetch_ins(ins_queue, current_ins);

		switch (current_ins) {
		case ins_type::acc:
			var_stack_push(get_sym(get_acc(ins_to_var_id(current_queue))));
			std::cout << "acc\n";
			break;

		case ins_type::lit:
			auto v = get_lit(ins_to_var_id(current_queue));
			var_id_t new_id = new_var_id();
			add_var(new_id, v);
			var_stack_push(new_id);
			std::cout << "lit\n";
			break;
		}
	}
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
