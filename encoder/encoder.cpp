#include "encoder.hpp"

namespace pasm {
	
	bool operator==(const Registers& r1, const Registers& r2)
	{
		return ((r1.r1 == r2.r1) and (r1.r2 == r2.r2));
	}
	
	unsigned char rr_lookup(Registers r)
	{
		for (auto rr: rr_table) {
			if (rr.first == r) {
				return rr.second; 
			}
		}
		return 0;
	}
	
	unsigned char rc_lookup(Register::Name r)
	{
		for (auto rc: rc_table) { 
			if (rc.first == r) {
				return rc.second;
			}
		}
		return 0;
	}
	
	void init_maps()
	{
		/*
		* Sorry for this.
		* Too lazy to write it out by hand.
		* #define R(r) (registers[#r] = Register::Name::r)
		* So:
		* R(AX); == registers["AX"] = Register::Name::AX;
		*/
		R(EAX);
		R(EBX);
		R(ECX);
		R(EDX);
		R(AX);
		R(BX);
		R(CX);
		R(DX);
		R(AL);
		R(BL);
		R(CL);
		R(DL);
		R(AH);
		R(BH);
		R(CH);
		R(DH);
		
		std::ifstream f("encoder/rr_table.txt");
		std::string r1, r2;
		unsigned int byte;
		while (f >> r1 >> r2 >> std::hex >> byte) {
			for (auto& c: r1) c = std::toupper(c);
			for (auto& c: r2) c = std::toupper(c);
			Registers r(registers[r1], registers[r2]);
			rr_table.push_back(std::make_pair(r, byte));
		}
		f.close();
		
		f = std::ifstream("encoder/rc_table.txt");
		while (f >> r1 >> std::hex >> byte) {
			for (auto& c: r1) c = std::toupper(c);
			rc_table.push_back(std::make_pair(registers[r1], byte));
		}
	}

	binary_data assemble(std::vector<ParserIR> instructions)
	{
		init_maps();
		binary_data data;
		for (auto inst: instructions) {
			data.push_back(transform(inst));
		}
		return data;
	}

	bd transform(ParserIR& inst)
	{
		switch (inst.expr.opcode) {
		case Opcode::mov: {
			return encode_mov(inst);
		}
		default: {
			error(inst.index, inst.line, "Unsupported opcode for assembly.");
			return bd();
		}
		}
	}
	
	bd encode_mov(ParserIR& i)
	{
		bd data;
		if (i.expr.arg1->type != Argument::Type::Register) {
			error(i.index, i.line, "In 'mov' first arg must be register.");
		}
		if (i.expr.arg2->type == Argument::Type::Register) {
			switch (((Register*)i.expr.arg1)->size) {
			case 8: {
				data.push_back(0x88);
				break;
			}
			case 16: {
				data.push_back(0x66);
				data.push_back(0x89);
				break;
			}
			case 32: {
				data.push_back(0x89);
				break;
			}
			}
			data.push_back((int)rr_lookup(Registers(i.expr)));
		} else if (i.expr.arg2->type == Argument::Type::ConstantInt) {
			if (((Register*)i.expr.arg1)->size != 16) {
				data.push_back((int)rc_lookup(((Register*)i.expr.arg1)->name));
				std::array<unsigned char, 3> bytes;
				auto n = ((ConstantInt*)i.expr.arg2)->value;
				bytes[0] = ((n >> 24) & 0xFF);
				bytes[1] = ((n >> 16) & 0xFF);
				bytes[2] = ((n >> 8) & 0xFF);
				bytes[3] = (n & 0xFF);
				for (auto b: bytes) {
					data.push_back(b);
				}
				data.push_back((int)((ConstantInt*)i.expr.arg2)->value);
			} else {
				data.push_back(0x66);
				data.push_back((int)rc_lookup(((Register*)i.expr.arg1)->name));
				data.push_back((unsigned char)(((ConstantInt*)i.expr.arg2)->value & 0xFF));
				data.push_back((unsigned char)(((ConstantInt*)i.expr.arg2)->value >> 8));
			}
		}
		return data;
	}
	
	void print_hex(const binary_data& data)
	{
		for (auto d: data) {
			for (auto dd: d) {
				std::cout << std::hex << "0x" << (int)dd << " ";
			}
			std::cout << std::endl;
		}
	}
}