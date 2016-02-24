#ifndef PASM_PARSER_ERROR_HPP
#define PASM_PARSER_ERROR_HPP

#include <iostream>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <cstdarg>
#include <list>
#include <vector>
#include <cstdint>

#include "expression.hpp"
#include "parser_ir.hpp"

namespace pasm {

	static std::string filename;

	void init_error(std::string filename);

	void error(std::string fmt, ...);
	void error(int line_number, std::string line, std::string fmt, ...);

	void run_tests(std::vector<ParserIR> expressions);

	void assert(std::vector<ParserIR> expressions, 
		    bool(*opP)(Opcode),
		    bool(*arg1P)(Argument*),
		    bool(*arg2P)(Argument*),
		    bool(*test)(Argument*, Argument*),
		    std::string error_msg);


	template <Opcode Op>
	bool opcode_type_P(Opcode o);

	template <Argument::Type A>
	bool argument_type_P(Argument* a);

	bool two_register_P(Opcode o);

}

#endif /*PASM_PARSER_ERROR_HPP*/
