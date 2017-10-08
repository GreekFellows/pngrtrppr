//to do:
/*
make hpp file
 make vars respond to usual arithmetic operations
 make vars able to respond to do doubles
 make expressions for loops and functions.

need to find out how to concatinate strings rather than add what ever they add together
 delete variables that are no longer necessary


*/
#include <fstream>
#include <string>
#include <unordered_map>
#include <iostream>


using reg_index_type = unsigned int;

enum class var_type {
    STR, INT, BOOL, DOUBLE, FILE
};

struct file {
    std::fstream strm;
    std::string f_name; // this is done so that a file can be opened and reopened so
    // that we can read and write rather than read previous
    file(std::string &name);
    file(std::string &name, std::string &wts_tag);
    void close();
    void open();
    //~file();
    void write(std::string &s); // write to file
    void writeln(std::string &s);
    void where_to_start(std::string &tag); // determine where to start by given tag
};

struct var {
    //std::string name;
    reg_index_type id;
    var_type type;
    var(reg_index_type index, var_type t);
    var(reg_index_type index, var_type t, std::string &s);
    var(reg_index_type index, var_type t, int i);
    var(reg_index_type index, var_type t, bool b);
    var(reg_index_type index, var_type t, double d);
    ~var();
    union {
        std::string str_value;
        int int_value;
        bool bool_value;
        double dob_value;
        std::shared_ptr<file> f_value;
    };
};

std::unordered_map<std::string, reg_index_type> user_def_map;
std::unordered_map<reg_index_type, std::shared_ptr<var>> tmp_reg;