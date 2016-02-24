#ifndef PASM_OPCODE_HPP
#define PASM_OPCODE_HPP

namespace pasm {

	enum class Opcode {
		None,
		mov,
		add,
		int_,
		jmp
	};

	static const char* const Opcode_str[] = { "None",
						  "mov",
				                  "add",
					          "int",
						  "jmp" };

}

#endif /*PASM_OPCODE_HPP*/
