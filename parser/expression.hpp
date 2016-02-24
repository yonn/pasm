#ifndef PASM_EXPRESSION_HPP
#define PASM_EXPRESSION_HPP

#include "opcode.hpp"
#include "argument.hpp"
#include "label.hpp"

namespace pasm {
	
	struct Expression {
		Opcode opcode;

		Argument* arg1;
		Argument* arg2;

		Expression(Opcode o, Argument* a1, Argument* a2) : opcode(o), arg1(a1), arg2(a2) { }
		Expression() { }
	};

}

#endif /*PASM_EXPRESSION_HPP*/
