//#include "pngr2_lex.hpp"
//#include "pngr2_parse.hpp"
#include "pngr2_instr.cpp"

#include <iostream>
#include <fstream>
#include <sstream>

int main(int argc, char** argv) {
	/*for (int argi = 1; argi < argc; ++argi) {
		std::ifstream reader(argv[argi]);
		std::cout << "# instrs: " << parse(reader).size() << std::endl;
		for (std::shared_ptr<token> tok = get_token(reader); tok != nullptr; tok = get_token(reader)) {
			tok->str(std::cout);
		}
	}*/
    std::string name("file.txt");
    std::fstream strm1;
    strm1.open(name, std::fstream::out|std::fstream::in);
    std::shared_ptr<file> f = std::make_shared<file>(name);
    std::string s3("hello");
    f->write(s3);
    f->writeln(s3);
    f->strm.close(); // need to close the stream to allow where to start
    std::shared_ptr<file> f1 = std::make_shared<file>(name);
    f1->where_to_start(s3);
    std::string s2("gg");
    f1->write(s2);
    reg_index_type v1 = make_tmp(var_type::INT);
    tmp_reg[v1]->str_value = s2;
    reg_index_type v2 = make_tmp(var_type::INT);
    tmp_reg[v2]->str_value = s3;
    /*
     std::shared_ptr<var> v1 = std::make_shared<var>(next_index(), var_type::INT);
     v1->int_value = 25;
     std::shared_ptr<var> v2 = std::make_shared<var>(next_index(), var_type::INT);
     v2->int_value = 15;*/
    reg_index_type v3 = add(v1,v2);
    std::shared_ptr<expr_0> e1= std::make_shared<expr_0>();
    std::shared_ptr<expr_0> e2= std::make_shared<expr_0>();
    e1->value = v1; //make the variable get an index to the value in the table.
    e2->value = v2;
    std::shared_ptr<expr_4> e3= std::make_shared<expr_4>();
    e3->type = expr_4::exec_type::ADD;
    e3->lhs = e1;
    e3->rhs = e2;
    unsigned int i = e3->execute();
    std::cout << std::endl << tmp_reg[i]->int_value << std::endl ;
	//std::string s;
	/*while (std::getline(std::cin, s) && s != "fuck") {
		std::istringstream iss(s);
		std::cout << "# instrs: " << parse(iss).size() << std::endl;
	}*/

	return 0;
}