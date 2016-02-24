#include "parser_debug.hpp"

namespace pasm {
	
	void print_list(TokenList list)
	{
		for (auto token : list) {
			std::cout << ">\t" << token << std::endl;
		}
	}

	void print_expression(Expression e)
	{
		std::cout << ">>\tExpression: #| ";
		std::cout << Opcode_str[(int)e.opcode] << ", ";
		e.arg1->print();
		std::cout << ", ";
		e.arg2->print();
		std::cout << " |#" << std::endl;
	}

	void print_tokens(std::list<LexerIR> list)
	{
		for (auto l : list) {
			std::cout << l.index << ") " << '"' << l.line << '"' << ": " << std::endl;
			print_list(l.tokens);
			std::cout << std::endl << std::endl;
		}
	}

	void print_expressions(std::vector<ParserIR> list)
	{
		for (auto p : list) {
			std::cout << p.index << ") " << '"' << p.line << '"' << ": " << std::endl;
			print_expression(p.expr);
			std::cout << std::endl << std::endl;
		}
	}

	void print_labels(std::map<std::string, Label> labels)
	{
		for (auto label : labels) {
			std::cout << label.second.home_line << ") " << std::endl
				  << "label> " << label.first << ", "<< label.second.home_line << " -> " << label.second.ptr_line << std::endl << std::endl;
		}
	}

	void print_variables(std::list<DataIR> vars)
	{
		for (auto v : vars) {
			std::cout << v.index << ") " << '"' << v.line << '"' << ": " << std::endl;
			v.var->print();
			std::cout << std::endl << std::endl;
		}
	}

}
