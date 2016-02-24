#ifndef PASM_ARGUMENT_HPP
#define PASM_ARGUMENT_HPP

#include <string>
#include <iostream> 

namespace pasm {

	static const char* RegisterName_str[] = { "eax",
					      	  "ax",
					      	  "al",
					      	  "ah",
					      	  "ebx",
					      	  "bx",
					      	  "bl",
					      	  "bh",
					      	  "ecx",
					      	  "cx",
					      	  "cl",
					      	  "ch",
					      	  "edx",
					      	  "dx",
					      	  "dl",
					          "dh" };

	static const char* ArgumentType_str[] = { "None",
				      	  	  "Register",
				          	  "ConstantInt",
						  "Label",
						  "Symbol" };

	static const char* Access_str[] = { "Value",
					    "Reference" };


	enum class Access {
		Value,
		Reference
	};

	class Argument {
	public:
		enum class Type {
			None,
			Register,
			ConstantInt,
			Label
		} type;

		Argument(Type t) : type(t) { }

		virtual void print()
		{
			std::cout << "Argument(" << ArgumentType_str[(int)type] << ")";
		}

		virtual ~Argument() { }
	};

	class Register : public Argument {
	public:
		enum class Name {
			EAX, AX, AL, AH,
			EBX, BX, BL, BH,
			ECX, CX, CL, CH,
			EDX, DX, DL, DH
		} name;

		int size;

		Access access;

		Register(Name n, int s, Access a = Access::Value) : Argument(Argument::Type::Register), name(n), size(s), access(a) { }

		virtual void print()
		{
			std::cout << "Register(" << RegisterName_str[(int)name] << ", " << size << ", " << Access_str[(int)access] << ")"; 
		}
	};

	class ConstantInt : public Argument {
	public:
		long long value;

		ConstantInt(int v) : Argument(Argument::Type::ConstantInt), value(v) { }

		virtual void print()
		{
			std::cout << "ConstantInt(" << value << ")";
		}
	};

	class LabelArg : public Argument {
	public:
		std::string name;

		LabelArg(std::string n) : Argument(Argument::Type::Label), name(n) { }

		virtual void print()
		{
			std::cout << "Label(" << name << ")";
		}
	};

	class SymbolArg : public Argument {
	public:
		std::string name;

		Access access;

		SymbolArg(std::string n, Access a = Access::Value) : Argument(Argument::Type::Label), name(n), access(a) { }

		virtual void print()
		{
			std::cout << "Symbol(" << name << ", " << Access_str[(int)access] << ")";
		}
	};
}

#endif /*PASM_ARGUMENT_HPP*/
