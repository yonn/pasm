#ifndef PASM_LEXER_HPP
#define PASM_LEXER_HPP

#include <list>
#include <string>
#include <utility>
#include <map>
#include <iostream>

#include "expression.hpp"

namespace pasm {

	typedef std::list<std::string> TokenList;

	struct LexerIR {
		int index;
		std::string line;
		TokenList tokens;
	};

	bool is_token(char c);

	bool is_whitespace(char c);

	TokenList split_tokens(std::string tokens);

	TokenList tokenize(std::string line);

	enum class Section { none,
			     text,
		             data,
			     LEN };

	Section get_section(std::string line, int line_num, int* text_section_line_number, int* data_section_line_number);
}

#endif /*PASM_LEXER_HPP*/
