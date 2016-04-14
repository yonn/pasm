#include "encoder.hpp"

namespace pasm {

	binary_data assemble(std::vector<ParserIR> instructions)
	{
		binary_data data;
		for (auto inst: instructions) {
			data.push_back(transform(inst));
		}
		return data;
	}

	bd transform(ParserIR& inst)
	{
		bd data;
		return data;
	}
}
