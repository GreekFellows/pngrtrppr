

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

// need to make something for string, int, bool, file

#include "pngr2_instr.hpp"
#include <fstream>
#include <string>
#include <unordered_map>
#include <iostream>


// open and close so that what is recently writen can be read.
void file::close() {
    strm.close();
}
void file::open() {
    strm.open(f_name,std::fstream::out|std::fstream::in);
}

file::file(std::string &name) {
    strm.open(name,std::fstream::out|std::fstream::in); // if reading from file need this:
    f_name = name;                                    // std::ios_base::in|, but then
                                                     //can't open nonexistent
}
file::file(std::string &name, std::string &wts_tag) {
    strm.open(name,std::fstream::out|std::fstream::in);
    where_to_start(wts_tag);
    f_name = name;
}

/*file::~file() {
    strm.close();
}*/
void file::write(std::string &s) {
    const char* c = s.c_str();
    if(strm.is_open()) {
        std::cout << "open";
    }
    else {
        std::cout << "not open";
    }
    strm << s;
    // need to ask Satoru if we ever need to replace characters.
}
void file::writeln(std::string &s) {
    write(s);
    strm.put(10); // newline character
}
void file::where_to_start(std::string &tag) {
    close();// opens and closes so that what is written can be read.
    open();
    strm.seekp(0); // so that it searches whole file each time so can go in reverse
    int char_in_tag = 0;
    char c;
    int place_in_file = 0;
    while(!strm.eof() && char_in_tag != tag.length()) {
        // checks if current character matches the sequence
        c = strm.peek();
        if(c == tag.at(char_in_tag)) {
            char_in_tag++;
        }
        else {
            char_in_tag = 0;
        }
        // goes to next character in the file
        place_in_file++;
        strm.seekp(place_in_file);
    }
}


var::~var() {} // seems to fix some problem by having this, I guess string doesn't
               // delete otherwise


/*
stmts
    stmt stmts

stmt
    while expr_9 stmts
    def id = sutff;
    for (stmt; expr_9; stmt) {}
    if(expr_9) {stmts} elif(expr_9) {stmts} else(expr_9) {stmts}
 
 
 
*/

static reg_index_type next_id = 0;

var::var(reg_index_type index, var_type t) :
    id(index), type(t) {}

// for strings
var::var(reg_index_type index, var_type t, std::string &s) :
    id(index), type(t), str_value(s) {}

// for ints
var::var(reg_index_type index, var_type t, int i) :
    id(index), type(t), int_value(i) {}

// for bools
var::var(reg_index_type index, var_type t, bool b) :
    id(index), type(t), bool_value(b) {}

// for doubles
var::var(reg_index_type index, var_type t, double d) :
    id(index), type(t), dob_value(d) {}

unsigned int next_index() {
    // need to eventually look for open spaces
    next_id++;
    return next_id;
}

// should probably be renamed to make_var unless named vars will be treated differently
// used for making variable that is put in variable map
reg_index_type make_tmp(var_type t) {//std::shared_ptr<var> v) {
	reg_index_type new_index = next_index();
    std::shared_ptr<var> v  = std::make_shared<var>(new_index, t);
    tmp_reg[new_index] = v;
	return new_index;
}

reg_index_type make_named_var(var_type t, std::string name) {
    reg_index_type index = make_tmp(t);
    user_def_map[name] = index;
    return index;
}

// functions so that the way the variables are made does not matter
void set_var_value(reg_index_type index, std::string val, var_type t) {
    if(t == var_type::FILE) {
        tmp_reg[index]->f_value->close();
        tmp_reg[index]->f_value = std::make_shared<file>(val);
    }
    else {
        tmp_reg[index]->str_value = val;
    }
}

void set_var_value(reg_index_type index, int val) {
    tmp_reg[index]->int_value = val;
}

void set_var_value(reg_index_type index, bool val) {
    tmp_reg[index]->bool_value = val;
}
void set_var_value(reg_index_type index, double val) {
    tmp_reg[index]->dob_value = val;
}

// so that it is easier to just use the functions for file without knowing implementation of
// var
void write_to_var_file(reg_index_type index, std::string str) {
    tmp_reg[index]->f_value->write(str);
}
void writeln_to_var_file(reg_index_type index, std::string str) {
    tmp_reg[index]->f_value->writeln(str);
}
void where_to_start_var_file(reg_index_type index, std::string tag) {
    tmp_reg[index]->f_value->where_to_start(tag);
}

reg_index_type add(reg_index_type index1, reg_index_type index2)
{
    if(tmp_reg[index1]->type == var_type::INT && tmp_reg[index2]->type == var_type::INT) {
        reg_index_type index = make_tmp(var_type::INT);
        tmp_reg[index]->int_value = tmp_reg[index1]->int_value + tmp_reg[index2]->int_value;
        return index;
    }
    else if (tmp_reg[index1]->type == var_type::INT && tmp_reg[index2]->type == var_type::STR) {
        reg_index_type index = make_tmp(var_type::STR);
        tmp_reg[index]->str_value = std::to_string(tmp_reg[index1]->int_value) + tmp_reg[index2]->str_value;
        return index;
    }
    else if (tmp_reg[index1]->type == var_type::INT && tmp_reg[index2]->type == var_type::BOOL) {
        reg_index_type index = make_tmp(var_type::INT);
        tmp_reg[index]->int_value = tmp_reg[index1]->int_value + tmp_reg[index2]->bool_value;
        return index;
    }
    else if(tmp_reg[index1]->type == var_type::STR && tmp_reg[index2]->type == var_type::INT) {
        reg_index_type index = make_tmp(var_type::STR);
        tmp_reg[index]->str_value = tmp_reg[index1]->str_value + std::to_string(tmp_reg[index2]->int_value);
        return index;
    }
    else if (tmp_reg[index1]->type == var_type::STR && tmp_reg[index2]->type == var_type::STR) {
        reg_index_type index = make_tmp(var_type::STR);
        tmp_reg[index]->str_value = tmp_reg[index1]->str_value + tmp_reg[index2]->str_value;
        return index;
    }
    else if (tmp_reg[index1]->type == var_type::STR && tmp_reg[index2]->type == var_type::BOOL) {
        reg_index_type index = make_tmp(var_type::STR);
        if(tmp_reg[index2]->bool_value) {
            tmp_reg[index]->str_value = tmp_reg[index1]->str_value + "true";
        }
        else {
            tmp_reg[index]->str_value = tmp_reg[index1]->str_value + "false";
        }
        return index;
    }
    else if(tmp_reg[index1]->type == var_type::BOOL && tmp_reg[index2]->type == var_type::INT) {
        reg_index_type index = make_tmp(var_type::INT);
        tmp_reg[index]->int_value = tmp_reg[index1]->bool_value + tmp_reg[index2]->int_value;
        return index;
    }
    else if (tmp_reg[index1]->type == var_type::BOOL && tmp_reg[index2]->type == var_type::STR) {
        reg_index_type index = make_tmp(var_type::STR);
        if(tmp_reg[index2]->bool_value) {
            tmp_reg[index]->str_value = "true" + tmp_reg[index2]->str_value;
        }
        else {
            tmp_reg[index]->str_value = "false" + tmp_reg[index2]->str_value;
        }
        return index;
    }
    else if (tmp_reg[index1]->type == var_type::BOOL && tmp_reg[index2]->type == var_type::BOOL) {
        reg_index_type index = make_tmp(var_type::BOOL);
        tmp_reg[index]->bool_value = tmp_reg[index1]->bool_value + tmp_reg[index2]->bool_value;
        return index;
    }
    else {
        // perhaps in the future could have stuff about if this is file or other that causes prob
        return -1; // error
    }
}

reg_index_type assign(reg_index_type index1, reg_index_type index2) {
    if(tmp_reg[index2]->type == var_type::STR) {
        if()
        tmp_reg[]
    }
}

struct instr {
    unsigned int id; // this may not be needed, but in future maybe for funtions that
                     // are executed many times
};

struct expr_4 : public instr {
    // should note that lower level corresponds to decreasing order of opperation
    // that is that lower is done first.
	enum class exec_type {
		ADD, MINUS
	} type;
    // should probably take out itself, because does not make any sense since it never
    //needs to happen
	std::shared_ptr<expr_4> lhs;
    std::shared_ptr<expr_4> rhs;

	virtual reg_index_type execute() {
		reg_index_type lhs_index = lhs->execute();

		if (type == exec_type::ADD) {
			// expr: factor + expr
			reg_index_type rhs_index = rhs->execute();
            //can delete the rhs side after executing it
			// a level of type checking
            reg_index_type new_var = add(lhs_index,rhs_index);
            // should add option to delete it here because technically used, but unknown
            // if not always temp
            return new_var;
		}
		else if (type == exec_type::MINUS) {
			// expr: factor - expr
			reg_index_type rhs_index = rhs->execute();
			// a level of type checking
            //std::shared_ptr<var> new_var = subtract(tmp_reg[lhs_index],
            //                                        tmp_reg[rhs_index]);
            return 0;//make_tmp(new_var);
		}
        return 0;// this will make an error happen
	}
};

struct expr_3 : public expr_4 {
    enum class factor_type {
        MULT, DIVID, MOD
    } type;
    
    virtual reg_index_type execute() {
        reg_index_type lhs_index = lhs->execute();
        if (type == factor_type::MULT) {
            reg_index_type rhs_index = rhs->execute();
            //std::shared_ptr<var> new_var = multiply(tmp_reg[lhs_index],
            //                                        tmp_reg[rhs_index]);
            return 0;//make_tmp(new_var);
        }
        else if (type == factor_type::DIVID) {
            reg_index_type rhs_index = rhs->execute();
            //std::shared_ptr<var> new_var = divide(tmp_reg[lhs_index],tmp_reg[rhs_index])
            return 0;//make_tmp(new_var);
        }
        return 0;
    }
};

struct expr_0 : public expr_3 {
    // need to make sure that the index to value writen and not actual value.
    reg_index_type value;
    
    reg_index_type execute() {
        return value;
    }
};