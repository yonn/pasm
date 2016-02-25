#include <fstream>
#include <iostream>
#include <list>
#include <string>

#include "parser/parser.hpp"

int main(int argc, char* argv[])
{
	auto stuff = pasm::parse_file(argv[1]); 
	auto exprs = std::get<0>(stuff);
	auto vars = std::get<1>(stuff);
	auto labels = std::get<2>(stuff);

	run_tests(exprs);

	pasm::print_expressions(exprs);
	pasm::print_labels(labels);
	pasm::print_variables(vars);

	pasm::clean_up(exprs);
	pasm::clean_up(vars);

	return 0;
}
