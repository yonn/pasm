#include "lexer.hpp"

namespace pasm {

	bool is_token(char c)
	{
		return c == ',' or
		       c == ':';
	}

		bool is_whitespace(char c)
	{
		return c == ' ' or
		       c == '\t' or
		       c == '\n';
	}

	TokenList split_tokens(std::string tokens)
	{
		TokenList new_tokens;
		std::string buffer = "";

		for (auto c : tokens) {
			if (is_token(c)) {
				new_tokens.push_back(buffer);
				new_tokens.push_back(std::string(1, c));
				buffer = "";
			} else {
				buffer += c;
			}
		}

		new_tokens.push_back(buffer);
		return new_tokens;
	}

	/*TokenList tokenize(std::string line)
	{
		TokenList tokens;
		TokenList new_tokens;
		std::string buffer = "";
		bool comment = false;

		for (auto c : line) {
			if (c == ';') {
				comment = true;
			} else if (c == '\n' and comment) {
				comment = false;
				tokens.push_back(buffer);
				buffer = "";
			} else if (is_whitespace(c) and !comment) {
				tokens.push_back(buffer);
				buffer = "";
			} else if (!comment) {
				buffer += c;
			}
		}

		tokens.push_back(buffer);

		for (auto token : tokens) {
			auto new_ = split_tokens(token);
			new_tokens.splice(new_tokens.end(), new_);
		}

		new_tokens.remove_if([](const std::string& s) -> bool { return s == ""; });

		return new_tokens;
	}*/

	TokenList tokenize(std::string program)
	{
		TokenList tokens;
		TokenList new_tokens;
		std::string buffer = "";
		bool comment = false;
		bool quote = false;

		for (auto c : program) {
			if (c == ';' and !quote) {
				comment = true;
			} else if (c == '"' and !comment) {
				buffer += '"';
				quote = !quote;
			} else if (c == '\n' and comment) {
				comment = false;
			} else if (is_whitespace(c) and !comment and !quote) {
				tokens.push_back(buffer);
				buffer = "";
			} else if (!comment) {
				buffer += c;
			}
		}
	
		tokens.push_back(buffer);
	
		for (auto token : tokens) {
			auto new_ = split_tokens(token);
			new_tokens.splice(new_tokens.end(), new_);
		}
		
		tokens.clear();
		buffer = "";
		quote = false;
		for (auto token : new_tokens) {
			if (token[0] == '"') {
				buffer += token;
				quote = true;
			} else if (token[token.size() - 1] == '"') {
				buffer += token;
				tokens.push_back(buffer);
				quote = false;
				buffer = "";
			} else if (quote) {
				buffer += token;
			} else {
				tokens.push_back(token);
			}
		}
		
		tokens.remove_if([](const std::string& s) -> bool { return s == ""; });
	
		return tokens;
	}


	Section get_section(std::string line, int line_num, int* text_section_line_number, int* data_section_line_number)
	{
		static auto section = Section::none;
		if (line.find("section .text") != std::string::npos) {
			section = Section::text;
			*text_section_line_number = line_num;
		} else if (line.find("section .data") != std::string::npos) {
			section = Section::data;
			*data_section_line_number = line_num;
		}
		return section;
	}
}
