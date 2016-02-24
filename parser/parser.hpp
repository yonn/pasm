#ifndef PASM_PARSER_HPP
#define PASM_PARSER_HPP

#include <fstream>
#include <iostream>
#include <list>
#include <string>
#include <tuple>

#include "lexer.hpp"
#include "code_parser.hpp"
#include "data_parser.hpp"
#include "parser_debug.hpp"
#include "parser_error.hpp"

namespace pasm {
	
	std::tuple<std::vector<ParserIR>, std::list<DataIR>, std::map<std::string, Label>> parse_file(const char* filename);

	template <typename T>
	std::vector<T> list_as_vector(std::list<T> l);

}

#endif /*PASM_PARSER_HPP*/
