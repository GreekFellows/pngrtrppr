#pragma once
#ifndef PNGR2_BYTECODE_HPP
#define PNGR2_BYTECODE_HPP

#include "pngr2_lex.hpp"
#include "pngr2_var.hpp"
#include "pngr2_console.hpp"

#include <deque>
#include <string>
#include <memory>
#include <unordered_map>

//	abstraction is cool, avoid calling a single push_* or pop_* directly on a container

class bytecode {
public:
	// 2^8 objects at a time should be acceptable for now
	using ins_t = uint8_t;	// ins_t must not be greater than var_id_t
	using ins_queue_t = std::deque<ins_t>;

	enum ins_type : ins_t {
		//		instruction				# params
		acc,	//	acc [id]			1 * num
		lit,	//	lit [lit]			1 * num
		unop,	//	unop [op]			1
		binop,	//	binop [op]			1
	};

private:
	// parser stuff
	static std::deque<std::string> accs;
	static void add_acc(const std::string &new_acc);
	static std::string get_acc(const size_t &i);

	static std::deque<var_ptr> lits;
	static void add_lit(const var_ptr &new_lit);
	static var_ptr get_lit(const size_t &i);

	// runner stuff
	static std::unordered_map<std::string, std::deque<var_id_t>> syms;
	static void add_sym(const std::string &new_sym, const var_id_t &new_var_id);
	static void del_sym(const std::string &new_sym);
	static var_id_t get_sym(const std::string &str);

	static std::unordered_map<var_id_t, var_ptr> reg;
	static void add_var(const var_id_t &var_id, const var_ptr &p_var);
	static var_ptr get_var(const var_id_t &var_id);

	static std::deque<var_id_t> var_stack;
	static void var_stack_push(const var_id_t &var_id);
	static var_id_t var_stack_pop();

	static var_id_t new_var_id();

	static size_t get_ins_num_params(const ins_type &ins);

	// this function will be more complicated if var_id_t is actually larger than ins_t
	static ins_queue_t var_id_to_ins(const var_id_t &p_id);
	static var_id_t ins_to_var_id(ins_queue_t p_id);
	static const size_t num_ins_in_var_id = 2;

	// helper functions
	static void extend_back(ins_queue_t &to, ins_queue_t &what);

	// instruction fetching for running
	static ins_queue_t fetch_ins(ins_queue_t &ins_queue, ins_type &first_ins);

	// runtime error
	static console::runtime_error runtime_err;

public:
	static void parser_init();
	static void runner_init();

	// queue operations
	static ins_queue_t ins_queue_new();
	static void ins_queue_enq(ins_queue_t &ins_queue, const ins_t &ins);
	static ins_t ins_queue_deq(ins_queue_t &ins_queue);

	// instruction-adding functions
	static void add_ins_acc(ins_queue_t &ins_queue, const std::string &acc_id);

	static void add_ins_int_lit(ins_queue_t &ins_queue, const int &val);
	static void add_ins_dbl_lit(ins_queue_t &ins_queue, const double &val);
	static void add_ins_str_lit(ins_queue_t &ins_queue, const std::string &val);
	static void add_ins_bool_lit(ins_queue_t &ins_queue, bool val);

	static void add_ins_unop(ins_queue_t &ins_queue, const op_type &op);
	static void add_ins_binop(ins_queue_t &ins_queue, const op_type &op);

	static void run(ins_queue_t &ins_queue);
};

// we use the bytecode implementation; there may also be a struct implementation
using impl = bytecode;

#endif
