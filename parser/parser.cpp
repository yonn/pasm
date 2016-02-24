#include "parser.hpp"

namespace pasm {
	

	template <typename T>
	std::vector<T> list_as_vector(std::list<T> l)
	{
		std::vector<T> v;
		v.reserve(l.size());
		for (auto e : l) {
			v.push_back(e);
		}
		return v;
	}

	std::tuple<std::vector<ParserIR>, std::list<DataIR>, std::map<std::string, Label>> parse_file(const char* filename)
	{
		if (!filename) {
			error("No input files.\nCompilation terminated.");
		}

		init_error(filename);

		std::ifstream file(filename);
		std::string line;
		std::list<std::string> sections[(int)Section::LEN];
		int line_num = 1;
		int data_section_line_number = 0;
		int text_section_line_number = 0;
		bool data = true;

		while (std::getline(file, line)) {
			sections[(int)get_section(line, line_num, &text_section_line_number, &data_section_line_number)].push_back(line);
			line_num++;
		}
	
		if (sections[(int)Section::text].empty()) {
			error("Could not find text section.");
		}

		if (sections[(int)Section::data].empty()) {
			data = false;
		}
		
		if (data) sections[(int)Section::data].pop_front();
		sections[(int)Section::text].pop_front();

		std::list<LexerIR> text_section_ir;
		int i = text_section_line_number + 1;
		for (auto line : sections[(int)Section::text]) {
			if (line != "") {
				LexerIR l;
				l.index = i;
				l.line = line;
				l.tokens = tokenize(line);
				text_section_ir.push_back(l);
			}
			i++;
		}
		
		
		std::list<LexerIR> data_section_ir;
		if (data) {
			int i = data_section_line_number + 1;
			for (auto line : sections[(int)Section::data]) {
				if (line != "") {
					LexerIR l;
					l.index = i;
					l.line = line;
					l.tokens = tokenize(line);
					data_section_ir.push_back(l);
				}
				i++;
			}
		}
		
		text_section_ir.remove_if([](const LexerIR& l) -> bool { return  l.tokens.empty(); });
		if (data) data_section_ir.remove_if([](const LexerIR& l) -> bool { return  l.tokens.empty(); });

		DataParser d_parser(data_section_ir);
		auto vars = d_parser.generate_variables();

		CodeParser c_parser(text_section_ir, vars);
		auto exprs = c_parser.generate_expressions();

		return std::make_tuple(list_as_vector(exprs), vars, c_parser.labels_);
	}

}
