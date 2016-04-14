#ifndef PASM_VARIABLE_HPP
#define PASM_VARIABLE_HPP

#include <cstdint>
#include <string>
#include <iostream>

namespace pasm {

	class Variable {
	public:
		enum class Type {
			Scalar,
			String,
			Array
		} type;

		std::string name;

		Variable(Type t, std::string n) : type(t), name(n) { }
		
		virtual ~Variable() { }

		virtual void print() = 0;
	};

	class Scalar : public Variable {
	public:
		int size; // bits

		union {
			int8_t  byte;
			int16_t word;
			int32_t dword;
		} value;
		
		Scalar(std::string name, int s, int32_t init) : Variable(Variable::Type::Scalar, name), size(s)
		{
			value.dword = init;
		}

		virtual void print()
		{
			std::cout << "Variable \"" << name << "\" #|Scalar: size_in_bits(" << size << "), init_value(" << value.dword << ") |#";
		}
	};

	class String : public Variable {
	public:
		std::string value;

		String(std::string name, std::string str) : Variable(Variable::Type::String, name)
		{
			for (size_t i = 1; i < str.length() - 1; i++) {
				value += str[i];
			}
		}

		virtual void print()
		{
			std::cout << "Variable \"" << name << "\" #|String: \"" << value << "\" |#";
		}
	};

}

#endif /*PASM_VARIABLE_HPP*/
