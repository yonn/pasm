#ifndef PASM_DATA_PARSER_HPP
#define PASM_DATA_PARSER_HPP

#include <string>
#include <list>

#include "lexer.hpp"
#include "variable.hpp"
#include "parser_error.hpp"

namespace pasm {

	struct DataIR {
		int index;
		std::string line;
		Variable* var;
	};

	bool operator==(const DataIR& d, const std::string& s);

	void clean_up(std::list<DataIR> vars);

	class DataParser {
	public:
		DataParser(std::list<LexerIR> tokens);

		std::list<DataIR> generate_variables();

	private:
		std::list<LexerIR> tokens_;
		TokenList curr_list_;
		int curr_index_;
		std::string curr_line_;
		bool finished_;

		std::map<std::string, int> scalar_types_;

		void init_scalars();

		Variable* transform(TokenList list);

		std::pair<Variable::Type, int> parse_var_type(std::string str);
		bool is_scalar(std::string str);
		int scalar_size(std::string str);
		int32_t string_to_int(std::string str);

		void using_list(LexerIR list);
		std::string fetch();
	};

}

#endif /*PASM_DATA_PARSER_HPP*/
