//	yes, this file contains a redefinition of struct var and file originally defined in pngr2_instr.cpp
//	but we will eventually pull out these definitions from pngr2_instr.cpp and use this file
//	since both the struct and bytecode implementation depend on the same definition of var and file

//	NO, STOP USING TEMPLATE AND GO BACK TO OVERLOADING

#pragma once

#ifndef PNGR2_VAR_HPP
#define PNGR2_VAR_HPP

#include <cstdint>
#include <string>
#include <memory>
#include <variant>
#include <unordered_map>

#include "pngr2_lex.hpp"

using var_id_t = uint16_t;

enum class var_t {
	STR, INT, BOOL, DBL, FILE
};

struct file {};

class var;
using var_ptr = std::shared_ptr<var>;

class var {
public:
	using unary_func_t = var_ptr(*)(const var_ptr &lhs, bool sensible);
	using binary_func_t = var_ptr(*)(const var_ptr &lhs, const var_ptr &rhs, bool sensible);

	using unary_func_ptr_t = unary_func_t*;
	using binary_func_ptr_t = binary_func_t*;

private:
	static std::unordered_map<op_type, unary_func_t> unary_funcs;

	// unary functions
	// static var_ptr sample_unary_func(const var_ptr &lhs, bool sensible);

	static std::unordered_map<op_type, binary_func_t> binary_funcs;

	// binary functions
	// static var_ptr sample_binary_func(const var_ptr &lhs, const var_ptr &rhs, bool sensible);
	static var_ptr binary_add(const var_ptr &lhs, const var_ptr &rhs, bool sensible);

public:

	static const var_id_t var_id_zero = 0;

	static unary_func_ptr_t get_unary_func_ptr(const op_type &op);
	static binary_func_ptr_t get_binary_func_ptr(const op_type &op);

	var_id_t id;
	var_t type;
	std::variant<int, bool, double, std::string, std::shared_ptr<file>> field;

	static var_ptr make_int(const var_id_t &p_id, const int &p_val);
	static var_ptr make_dbl(const var_id_t &p_id, const double &p_val);
	static var_ptr make_bool(const var_id_t &p_id, const bool &p_val);
	static var_ptr make_str(const var_id_t &p_id, const std::string &p_val);
	static var_ptr make_file(const var_id_t &p_id, const std::shared_ptr<file> &p_val);

	void assign_id(const var_id_t &p_id);
	void assign_int(const int &p_val);
	void assign_dbl(const double &p_val);
	void assign_bool(const bool &p_val);
	void assign_str(const std::string &p_val);
	void assign_file(const std::shared_ptr<file> &p_val);

	var(const var_id_t &p_id, const var_t &p_type);
};

#endif
