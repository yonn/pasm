#ifndef PASM_PARSER_IR_HPP
#define PASM_PARSER_IR_HPP

namespace pasm {

	struct ParserIR {
		int index;
		std::string line;
		Expression expr;
	};

	void clean_up(std::vector<ParserIR> tree);

}

#endif /*PASM_PARSER_IR_HPP*/
