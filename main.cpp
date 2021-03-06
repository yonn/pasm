#include <fstream>
#include <iostream>
#include <list>
#include <string>

#include "parser/parser.hpp"
#include "encoder/encoder.hpp"
#include "formater/elf_file.hpp"

int _main(int argc, char* argv[])
{
	const char* out;
	auto stuff = pasm::parse_file(argv[1]); 
	auto exprs = std::get<0>(stuff);
	auto vars = std::get<1>(stuff);
	auto labels = std::get<2>(stuff);

	pasm::run_tests(exprs);

	pasm::print_expressions(exprs);
	pasm::print_labels(labels);
	pasm::print_variables(vars);
	
	auto data = pasm::assemble(exprs);

	if (argc < 3) {
		out = "a.out";
	} else {
		out = argv[2];
	}
	
	int rc = pasm::format(data, out);
	
	pasm::print_hex(data);

	pasm::clean_up(exprs);
	pasm::clean_up(vars);

	return 0;
}

extern "C"
{
	int main(int argc, char *argv[])
	{
		return _main(argc, argv);
	}
}
