#include "pngr2_var.hpp"

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