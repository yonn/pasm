#include "data_parser.hpp"

namespace pasm {

	bool operator==(const DataIR& d, const std::string& s)
	{
		return d.var->name == s;
	}

	void clean_up(std::list<DataIR> vars)
	{
		for (auto v : vars) {
			delete v.var;
		}
	}
	
	DataParser::DataParser(std::list<LexerIR> tokens) : tokens_(tokens)
	{
		init_scalars();
	}

	void DataParser::init_scalars()
	{
		scalar_types_["db"] = 8;
		scalar_types_["dw"] = 16;
		scalar_types_["dd"] = 32;
	}

	std::list<DataIR> DataParser::generate_variables()
	{
		std::list<DataIR> vars;
		for (auto token : tokens_) {
			DataIR d;
			d.index = token.index;
			d.line = token.line;
			using_list(token);
			d.var = transform(token.tokens);
			vars.push_back(d);
		}
		return vars;
	}


	Variable* DataParser::transform(TokenList list)
	{
		while (not finished_) {
			std::string var_name = fetch();
			auto colon = fetch();
			if (colon != ":") {
				error(curr_index_, curr_line_, "Unexpected \"%s\" in place of ':'.", colon.c_str());
			}
			auto type = parse_var_type(fetch());
			switch (type.first) {
			case Variable::Type::Scalar:
				return new Scalar(var_name, type.second, string_to_int(fetch()));
				break;
			case Variable::Type::String:
				return new String(var_name, fetch());
				break;
			default:
				error(curr_index_, curr_line_, "Somebody really messed up and I don't know what's wrong.");
				break;
			}
		}
		return nullptr;
	}

	void DataParser::using_list(LexerIR list)
	{
		curr_list_ = list.tokens;
		curr_index_ = list.index;
		curr_line_ = list.line;
		finished_ = false;
	}

	std::string DataParser::fetch()
	{
		if (curr_list_.empty()) {
			finished_ = true;
			return "";
		}
		auto ans = curr_list_.front();
		curr_list_.pop_front();
		return ans;
	}

	std::pair<Variable::Type, int> DataParser::parse_var_type(std::string str)
	{
		if (is_scalar(str)) {
			return std::make_pair(Variable::Type::Scalar, scalar_size(str));
		} else if (str == "str") {
			return std::make_pair(Variable::Type::String, 0);
		} else {
			error(curr_index_, curr_line_, "Unknown type identifier: \"%s\".", str.c_str());
		}
	}

	bool DataParser::is_scalar(std::string str)
	{
		return scalar_types_.count(str) == 1;
	}

	int DataParser::scalar_size(std::string str)
	{
		return scalar_types_[str];
	}

	int32_t DataParser::string_to_int(std::string str)
	{
		int base = 10;

		if (str.size() > 2 and str[0] == '0' and str[1] == 'x') {
			base = 16;
		}

		try {
			int val = std::stoi(str, nullptr, base);
			return val; 

		} catch (std::invalid_argument e) { 
			error(curr_index_, curr_line_, "Could not parse \"%s\" as a number.", str.c_str());
			return 0;
		}
	}
}
