#include "pngr2_var.hpp"

std::unordered_map<op_type, var::unary_func_t> var::unary_funcs = {
};

std::unordered_map<op_type, var::binary_func_t> var::binary_funcs = {
	{ op_type::ADD, var::binary_add }
};

var::unary_func_ptr_t var::get_unary_func_ptr(const op_type &op) {
	try {
		return &unary_funcs.at(op);
	}
	catch (std::out_of_range) {}

	return nullptr;
}

var::binary_func_ptr_t var::get_binary_func_ptr(const op_type &op) {
	try {
		return &binary_funcs.at(op);
	}
	catch (std::out_of_range) {}
	
	return nullptr;
}

var_ptr var::binary_add(const var_ptr &lhs, const var_ptr &rhs, bool sensible) {
	return nullptr;
	// stick dtom's adsds in here
}

var::var(const var_id_t &p_id, const var_t &p_type) : id(p_id), type(p_type) {}

var_ptr var::make_int(const var_id_t &p_id, const int &p_val) {
	var_ptr v = std::make_shared<var>(p_id, var_t::INT);
	v->field = p_val;
	return v;
}

var_ptr var::make_dbl(const var_id_t &p_id, const double &p_val) {
	var_ptr v = std::make_shared<var>(p_id, var_t::DBL);
	v->field = p_val;
	return v;
}

var_ptr var::make_bool(const var_id_t &p_id, const bool &p_val) {
	var_ptr v = std::make_shared<var>(p_id, var_t::BOOL);
	v->field = p_val;
	return v;
}

var_ptr var::make_str(const var_id_t &p_id, const std::string &p_val) {
	var_ptr v = std::make_shared<var>(p_id, var_t::STR);
	v->field = p_val;
	return v;
}

var_ptr var::make_file(const var_id_t &p_id, const std::shared_ptr<file> &p_val) {
	var_ptr v = std::make_shared<var>(p_id, var_t::FILE);
	v->field = p_val;
	return v;
}

void var::assign_id(const var_id_t &p_id) {
	id = p_id;
}

void var::assign_int(const int &p_val) {
	type = var_t::INT;
	field = p_val;
}

void var::assign_dbl(const double &p_val) {
	type = var_t::DBL;
	field = p_val;
}

void var::assign_bool(const bool &p_val) {
	type = var_t::BOOL;
	field = p_val;
}

void var::assign_str(const std::string &p_val) {
	type = var_t::STR;
	field = p_val;
}

void var::assign_file(const std::shared_ptr<file> &p_val) {
	type = var_t::FILE;
	field = p_val;
}