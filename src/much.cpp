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
#include <fstream>
#include <string>
#include <unordered_map>
#include <iostream>

enum class var_type {
    STR, INT, BOOL, DOUBLE, FILE
};

struct file {
    std::fstream strm;
    file(std::string &name);
    //~file();
    void write(std::string &s); // write to file
    void writeln(std::string &s);
    void where_to_start(std::string &tag); // determine where to start by given tag
    
};
file::file(std::string &name) {
    strm.open(name,std::fstream::out|std::fstream::trunc); // if reading from file need this:
                                        // std::ios_base::in|, but then can't open
                                        //nonexistent
}
/*file::~file() {
    strm.close();
}*/
void file::write(std::string &s) {
    const char* c = s.c_str();
    //std::cout << c;
    //strm.write(c, s.length());
    //std::cout << s;
    //std::cout << s.length();
    if(strm.is_open()) {
        std::cout << "open";
    }
    else {
        std::cout << "not open";
    }
    strm << s;
    //delete[] c;
}
void file::writeln(std::string &s) {
    write(s);
    strm.put(10); // newline character
}
void file::where_to_start(std::string &tag) {
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

struct var {
    std::string &name;
    var_type type;
    var(std::string &v_name, var_type t, std::string &str);
    var(std::string &v_name, var_type t, int i);
    var(std::string &v_name, var_type t, bool b);
    var(std::string &v_name, var_type t, double doub);
    var(std::string &v_name, var_type t, std::shared_ptr<file> f);
    union {
        std::string str_value;
        int int_value;
        bool bool_value;
        double dob_value;
        std::shared_ptr<file> f_value;
    };
};

var::var(std::string &v_name, var_type t, std::string &str) :
            name(v_name), type(t) {
                str_value = str;
            }

var::var(std::string &v_name, var_type t, int i) :
            name(v_name), type(t), int_value(i) {}

var::var(std::string &v_name, var_type t, bool b) :
            name(v_name), type(t), bool_value(b) {}

var::var(std::string &v_name, var_type t, double doub) :
            name(v_name), type(t), dob_value(doub) {}

var::var(std::string &v_name, var_type t, std::shared_ptr<file> f) :
            name(v_name), type(t), f_value(f) {}


std::shared_ptr<var> add(std::shared_ptr<var> var1, std::shared_ptr<var> var2)
{
    if(var1->type == var_type::INT && var2->type == var_type::INT) {
        std::shared_ptr<var> new_int;
        new_int->type = var_type::INT;
        new_int->int_value = var1->int_value + var2->int_value;
        return new_int;
    }
    else if (var1->type == var_type::INT && var2->type == var_type::STR) {
        std::shared_ptr<var> new_string;
        new_string->type = var_type::STR;
        new_string->str_value = std::to_string(var1->int_value) + var2->str_value;
        return new_string;
    }
    else if (var1->type == var_type::INT && var2->type == var_type::BOOL) {
        std::shared_ptr<var> new_int;
        new_int->type = var_type::INT;
        new_int->int_value = var1->int_value + var2->bool_value;
        return new_int;
    }
    else if(var1->type == var_type::STR && var2->type == var_type::INT) {
        std::shared_ptr<var> new_string;
        new_string->type = var_type::STR;
        new_string->str_value = var1->str_value + std::to_string(var2->int_value);
        return new_string;
    }
    else if (var1->type == var_type::STR && var2->type == var_type::STR) {
        std::shared_ptr<var> new_string;
        new_string->type = var_type::STR;
        new_string->str_value = var1->str_value + var2->str_value;
        return new_string;
    }
    else if (var1->type == var_type::STR && var2->type == var_type::BOOL) {
        std::shared_ptr<var> new_string;
        new_string->type = var_type::STR;
        if(var2->bool_value) {
            new_string->str_value = var1->str_value + "true";
        }
        else {
            new_string->str_value = var1->str_value + "false";
        }
        return new_string;
    }
    else if(var1->type == var_type::BOOL && var2->type == var_type::INT) {
        std::shared_ptr<var> new_int;
        new_int->type = var_type::INT;
        new_int->int_value = var1->bool_value + var2->int_value;
        return new_int;
    }
    else if (var1->type == var_type::BOOL && var2->type == var_type::STR) {
        std::shared_ptr<var> new_string;
        new_string->type = var_type::STR;
        if(var2->bool_value) {
            new_string->str_value = "true" + var2->str_value;
        }
        else {
            new_string->str_value = "false" + var2->str_value;
        }
        return new_string;
    }
    else if (var1->type == var_type::BOOL && var2->type == var_type::BOOL) {
        std::shared_ptr<var> new_bool;
        new_bool->type = var_type::BOOL;
        new_bool->bool_value = var1->bool_value + var2->bool_value;
        return new_bool;
    }
    else {
        return nullptr;
    }
}

using reg_index_type = unsigned int;
static reg_index_type next_id = 0;

std::unordered_map<std::string, reg_index_type> user_def_map;
std::unordered_map<reg_index_type, std::shared_ptr<var>> tmp_reg;

unsigned int next_index() {
    next_id++;
    return next_id;
}

reg_index_type make_tmp(std::shared_ptr<var> v) {
	reg_index_type new_index = next_index();
    tmp_reg[new_index] = v;
	return new_index;
}

struct instr {
    unsigned int id; // this may not be needed, but in future maybe for funtions that
                     // are executed many times
};

struct expr_4 : public instr {
    // should note that lower level corresponds to decreasing order of opperation
    // that is that lower is done first.
	enum class exec_type {
		ADD, MINUS, ITSELF
	} type;
    // should probably take out itself, because does not make any sense since it never
    //needs to happen
	std::shared_ptr<expr_4> lhs;
    std::shared_ptr<expr_4> rhs;

	reg_index_type execute() {
		reg_index_type lhs_index = lhs->execute();

		if (type == exec_type::ITSELF) {
			// expr: factor
			return lhs_index;
		}
		else if (type == exec_type::ADD) {
			// expr: factor + expr
			reg_index_type rhs_index = rhs->execute();
            //can delete the rhs side after executing it
			// a level of type checking
            std::shared_ptr<var> new_var = add(tmp_reg[lhs_index],tmp_reg[rhs_index]);
            // should add option to delete it here because technically used, but unknown
            // if not always temp
			return make_tmp(new_var);
		}
		else if (type == exec_type::MINUS) {
			// expr: factor - expr
			reg_index_type rhs_index = rhs->execute();
			// a level of type checking
            //std::shared_ptr<var> new_var = subtract(tmp_reg[lhs_index],
            //                                        tmp_reg[rhs_index]);
            return 0;//make_tmp(new_var);
		}
        return 0;
	}
};

struct expr_3 : public expr_4 {
    enum class factor_type {
        MULT, DIVID, ITSELF
    } type;
    
    reg_index_type execute() {
        reg_index_type lhs_index = lhs->execute();
        if(type == factor_type::ITSELF){
            return lhs_index;
        }
        else if (type == factor_type::MULT) {
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
    std::shared_ptr<var> value;
    
    reg_index_type execute() {
        return make_tmp(value);
    }
};