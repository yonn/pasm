#include "parser_error.hpp"

namespace pasm {

	void init_error(std::string f)
	{
		filename = f;
	}

	void error(std::string fmt, ...)
	{
		fmt = fmt + '\n';
		std::va_list args;
		va_start(args, fmt.c_str());

		std::fprintf(stderr, "Error: ");
		std::vfprintf(stderr, fmt.c_str(), args);

		va_end(args);

		std::exit(1);
	}
	
	void error(int line_number, std::string line, std::string fmt, ...)
	{
		fmt = fmt;
		std::va_list args;
		va_start(args, fmt.c_str());
		
		std::fprintf(stderr, "In file: \"%s\"::%d: error: ", filename.c_str(), line_number);
		std::vfprintf(stderr, fmt.c_str(), args);
		std::fprintf(stderr, "\n\t%s\n\n", line.c_str());

		va_end(args);
		std::exit(1);
	}

	void run_tests(std::vector<ParserIR> e)
	{
		assert(e, &two_register_P, &argument_type_P<Argument::Type::Register>, &argument_type_P<Argument::Type::Register>, [](auto a1, auto a2) {
					return (((Register*)a1)->size) == (((Register*)a2)->size);
				}, "Incompatible register sizes. Registers must be the same size.");

		assert(e, &opcode_type_P<Opcode::mov>, &argument_type_P<Argument::Type::Register>, &argument_type_P<Argument::Type::ConstantInt>,
		       [](auto a1, auto a2) {
				int size = ((Register*)a1)->size;
				long long value = ((ConstantInt*)a2)->value;
				switch (size) {
				case 8:
					return (value >= INT8_MIN and value <= INT8_MAX);
				case 16:
					return (value >= INT16_MIN and value <= INT16_MAX);
				case 32:
					return (value >= INT32_MIN and value <= INT32_MAX);
				}
		       }, "Integer literal is too big to fit in the specified register.");
	}
	
	void assert(std::vector<ParserIR> expressions, 
			    bool(*opP)(Opcode),
			    bool(*arg1P)(Argument*),
			    bool(*arg2P)(Argument*),
			    bool(*test)(Argument*, Argument*),
			    std::string error_msg)
	{
		for (auto e : expressions) {
			if (opP(e.expr.opcode) and arg1P(e.expr.arg1) and arg2P(e.expr.arg2)) {
				if (not test(e.expr.arg1, e.expr.arg2)) {
					error(e.index, e.line, error_msg.c_str());
				}
			}
		}
	}

	template <Opcode Op>
	bool opcode_type_P(Opcode o)
	{
		return o == Op;
	}

	template <Argument::Type A>
	bool argument_type_P(Argument* a)
	{
		return a->type == A;
	}

	bool two_register_P(Opcode o)
	{
		return o == Opcode::mov or
		       o == Opcode::add;

	}
}
