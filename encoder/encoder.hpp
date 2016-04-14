#ifndef PASM_ENCODER_HPP
#define PASM_ENCODER_HPP

#include <vector>
#include <map>
#include <cstdint>

#include "../parser/parser.hpp"

namespace pasm {

	void init_maps();

	typedef std::vector<int8_t> bd;
	typedef std::vector<bd> binary_data;

	binary_data assemble(std::vector<ParserIR> instructions);

	bd transform(ParserIR& instruction);

	bd mov_register8(ParserIR& i);
	bd mov_register16(ParserIR& i);
	bd mov_register32(ParserIR& i);
}

#endif /*PASM_ENCODER_HPP*/
