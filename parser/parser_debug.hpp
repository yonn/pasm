#ifndef PASM_PARSER_DEBUG_HPP
#define PASM_PARSER_DEBUG_HPP

#include <iostream>
#include <string>

#include "lexer.hpp"
#include "expression.hpp"
#include "code_parser.hpp"
#include "data_parser.hpp"

namespace pasm {
	
	void print_list(TokenList list);

	void print_expression(Expression e);
	
	void print_tokens(std::list<LexerIR> list);

	void print_expressions(std::vector<ParserIR> list);

	void print_labels(std::map<std::string, Label> labels);

	void print_variables(std::list<DataIR> vars);
}

#endif /*PASM_PARSER_DEBUG_HPP*/
