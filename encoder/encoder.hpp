#ifndef PASM_ENCODER_HPP
#define PASM_ENCODER_HPP

#include <vector>
#include <utility>
#include <cstdint>
#include <array>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <locale>

#include "../parser/parser.hpp"

#define R(r) (registers[#r] = Register::Name::r)

namespace pasm {
	
	struct Registers {
		Register::Name r1;
		Register::Name r2;
		
		Registers(Register::Name o, Register::Name t) : r1(o), r2(t) { }
		Registers(Expression e) : r1(((Register*)(e.arg1))->name), r2(((Register*)(e.arg2))->name) { }
		Registers() { }
	};
	
	bool operator==(const Registers& r1, const Registers& r2);
	
	unsigned char rr_lookup(std::string inst, Registers r);
	unsigned char rc_lookup(std::string inst, Register::Name r);
	
	typedef std::vector<std::pair<Registers, unsigned char>> rr_table;
	typedef std::vector<std::pair<Register::Name, unsigned char>> rc_table;

	void init_maps();

	typedef std::vector<unsigned char> bd;
	typedef std::vector<bd> binary_data;

	binary_data assemble(std::vector<ParserIR> instructions);

	bd transform(ParserIR& instruction);
	
	bd encode_mov(ParserIR& instruction);
	bd encode_int(ParserIR& instruction);
	bd encode_ret(ParserIR& instruction);
	
	void print_hex(const binary_data& data);
}

#endif /*PASM_ENCODER_HPP*/
