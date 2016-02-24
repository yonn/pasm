#include "code_parser.hpp"

namespace pasm {

	void clean_up(std::vector<ParserIR> tree)
	{
		for (auto e : tree) {
			delete e.expr.arg1;
			delete e.expr.arg2;
		}
	}

	CodeParser::CodeParser(std::list<LexerIR> list, std::list<DataIR> data) : tokens_(list), vars_(data), finished_(false)
	{
		init_opcodes();
		init_registers();

		generate_labels();
	}

	void CodeParser::init_opcodes()
	{
		opcodes_["mov"] = std::make_pair(Opcode::mov, 2);
		opcodes_["add"] = std::make_pair(Opcode::add, 2);
		opcodes_["int"] = std::make_pair(Opcode::int_, 1);
		opcodes_["jmp"] = std::make_pair(Opcode::jmp, 1);
	}

	void CodeParser::init_registers()
	{
		registers_["eax"] = std::make_pair(Register::Name::EAX, 32);
		registers_["ebx"] = std::make_pair(Register::Name::EBX, 32);
		registers_["ecx"] = std::make_pair(Register::Name::ECX, 32);
		registers_["edx"] = std::make_pair(Register::Name::EDX, 32);

		registers_["ax"] = std::make_pair(Register::Name::AX, 16);
		registers_["bx"] = std::make_pair(Register::Name::BX, 16);
		registers_["cx"] = std::make_pair(Register::Name::CX, 16);
		registers_["dx"] = std::make_pair(Register::Name::DX, 16);

		registers_["al"] = std::make_pair(Register::Name::AL, 8);
		registers_["bl"] = std::make_pair(Register::Name::BL, 8);
		registers_["cl"] = std::make_pair(Register::Name::CL, 8);
		registers_["dl"] = std::make_pair(Register::Name::DL, 8);

		registers_["ah"] = std::make_pair(Register::Name::AH, 8);
		registers_["bh"] = std::make_pair(Register::Name::BH, 8);
		registers_["ch"] = std::make_pair(Register::Name::CH, 8);
		registers_["dh"] = std::make_pair(Register::Name::DH, 8);
	}

	std::list<ParserIR> CodeParser::generate_expressions()
	{
		std::list<ParserIR> expressions;

		for (auto token : tokens_) {
			ParserIR e;
			e.index = token.index;
			e.line = token.line;
			curr_line_num = token.index;
			curr_line = token.line;
			e.expr = transform(token.tokens);
			if (e.expr.opcode != Opcode::None) {
				expressions.push_back(e);
			}
		}
		point_labels(expressions);
		return expressions;
	}

	Expression CodeParser::transform(TokenList tokens) {
		Expression e;
		while (not finished_) {
		start:
			auto str = fetch(tokens);
			if (is_opcode(str)) {
				auto op = parse_opcode(str);
				e.opcode = op.first;
				switch (op.second) {
				case 0:
					e.arg1 = new Argument(Argument::Type::None);
					e.arg2 = new Argument(Argument::Type::None);
					break;
				case 1:
					e.arg1 = get_next_arg(tokens);
					e.arg2 = new Argument(Argument::Type::None);
					break;
				case 2:
					e.arg1 = get_next_arg(tokens);
					e.arg2 = get_next_arg(tokens);
					break;
				default:
					break;
				}
			} else if (is_label(str)) {
				fetch(tokens); // get rid of ':'
				goto start;
			} else if (str == "") {
				e = Expression(Opcode::None, nullptr, nullptr);
			} else {
				error(curr_line_num, curr_line, "Unidentified token: \"%s\"", str.c_str());
			}
		}
		finished_ = false;
		return e;
	}

	void CodeParser::generate_labels()
	{
		std::string first;
		std::string second;
		bool flag = true;

		for (auto tt : tokens_) {
			for (auto t : tt.tokens) {
				if (flag) {
					first = t;
					flag = false;
				} else {
					if (t == ":") {
						labels_[first] = Label(tt.index);
					}
					flag = true;
				}
			}
		}
	}

	void CodeParser::point_labels(std::list<ParserIR> list)
	{
		for (auto& label : labels_) {
			int line = label.second.home_line;
			for (auto e : list) {
				if (e.index >= line) {
					label.second.ptr_line = e.index;
					break;
				}
			}
		}
	}

	std::string CodeParser::fetch(TokenList& t)
	{
		if (t.empty()) {
			finished_ = true;
			return "";
		} else {
			auto res = t.front();
			t.pop_front();
			return res;
		}
	}

	std::string CodeParser::peek(TokenList& t)
	{
		if (t.empty()) {
			finished_ = true;
			return "";
		} else {
			auto res = t.front();
			return res;
		}

	}

	Argument* CodeParser::get_next_arg(TokenList& t)
	{
		std::string arg = "";
		bool finished = false;

		while (!finished) {
			auto next = fetch(t);

			if (next == "," or finished_) {
				finished = true;
			} else {
				arg += next;
			}
		}

		return parse_arg(arg);
	}

	bool CodeParser::is_opcode(std::string token)
	{
		return opcodes_.count(token) == 1;
	}

	bool CodeParser::is_register(std::string token)
	{
		return registers_.count(token) == 1;
	}

	bool CodeParser::is_label(std::string token)
	{
		return labels_.count(token) == 1;
	}
	
	bool CodeParser::is_var(std::string token)
	{
		return std::find(vars_.begin(), vars_.end(), token) != vars_.end(); 
	}

	std::pair<Opcode, int> CodeParser::parse_opcode(std::string token)
	{
		return opcodes_[token];
	}

	Argument* CodeParser::parse_arg(std::string token)
	{
		int base = 10;

		if (token.size() > 2 and token[0] == '0' and token[1] == 'x') {
			base = 16;
		}

		try {
			int val = std::stoi(token, nullptr, base);
			return new ConstantInt(val);

		} catch (std::invalid_argument e) { }

		if (is_register(token)) {
			return new Register(registers_[token].first, registers_[token].second);
		} else if (is_label(token)) {
			return new LabelArg(token);
		} else if (token[0] == '[' and token[token.size() - 1] == ']') {
			std::string temp = "";
			for (size_t i = 1; i < token.size() - 1; i++) {
				temp += token[i];
			}
			if (is_register(temp)) {
				return new Register(registers_[temp].first, registers_[temp].second, Access::Reference);
			} else if (is_var(temp)) {
				return new SymbolArg(temp, Access::Reference);
			}
		} else if (is_var(token)) {
			return new SymbolArg(token);
		}

		error(curr_line_num, curr_line, "Unidentified token: \"%s\"", token.c_str());

		return nullptr;
	}


}
