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

using var_id_t = uint8_t;

enum class var_t {
	STR, INT, BOOL, DBL, FILE
};

struct file {};

struct var;
using var_ptr = std::shared_ptr<var>;

struct var {
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
