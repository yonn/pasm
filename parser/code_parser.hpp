#ifndef PASM_CODE_PARSER_HPP
#define PASM_CODE_PARSER_HPP

#include <list>
#include <string>
#include <utility>
#include <map>
#include <algorithm>

#include "expression.hpp"
#include "data_parser.hpp"
#include "lexer.hpp"
#include "parser_error.hpp"
#include "parser_ir.hpp"

namespace pasm {

	class CodeParser {
	public:
		CodeParser(std::list<LexerIR> list, std::list<DataIR> data);

		std::list<ParserIR> generate_expressions();

		std::map<std::string, Label> labels_;

	private:
		std::list<LexerIR> tokens_;
		bool finished_;
		std::string curr_line;
		int curr_line_num;
		std::map<std::string, std::pair<Opcode, int>> opcodes_;
		std::map<std::string, std::pair<Register::Name, int>> registers_;
		std::list<DataIR> vars_;
		
		Expression transform(TokenList tokens);
		void generate_labels();
		void point_labels(std::list<ParserIR> list);

		std::string fetch(TokenList& t);
		std::string peek(TokenList& t);
		Argument* get_next_arg(TokenList& t);

		void init_opcodes();
		void init_registers();

		bool is_opcode(std::string token);
		bool is_register(std::string token);
		bool is_label(std::string token);
		bool is_var(std::string token);

		std::pair<Opcode, int> parse_opcode(std::string token);
		Argument* parse_arg(std::string token);

	};
}

#endif /*PASM_CODE_PARSER_HPP*/
