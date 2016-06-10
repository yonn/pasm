#ifndef ELF_FILE_H_INCLUDED
#define ELF_FILE_H_INCLUDED

#include <elf.h>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
//#include <cstdint>

#include "../encoder/encoder.hpp"

using namespace std;

namespace pasm
{
	
	int format(const binary_data& text_bin_dump, const string& output_file_name);
	
	namespace ELF
	{

	/*typedef enum class Types : unsigned short
	{
	None = ET_NONE,
	Relocatable = ET_REL,
	Object = ET_REL,
	Executable = ET_EXEC,
	SharedObject = ET_DYN,
	Core = ET_CORE,
	} Types;*/

	//typedef uint16_t unsigned short;

	class Types
	{
	public:
		Types(uint16_t type) : _t(type){};
		Types(Types& other) : _t(static_cast<uint16_t>(other)){};	
		~Types(){};

		operator uint16_t() const {return static_cast<uint16_t>(this->_t);};	

		enum
		{
		None = ET_NONE,
		Relocatable = ET_REL,
		Object = ET_REL,
		Linkable = ET_REL,
		Executable = ET_EXEC,
		SharedObject = ET_DYN,
		Core = ET_CORE,
		};
		
	private:
		uint16_t _t;
	};

	class ELF_file
	{
		public:
		//ELF_file(ELF::Types type = ELF::Types::Object, string output_file_name = "obj.tmp");
		ELF_file(const ELF::Types& type, const string& output_file_name);
		~ELF_file();
		
		
		void write(const binary_data& bin_dump);
		
		
		
		
		
		ELF_file& operator= (ELF_file&& other);
		//friend void ELF_file::operator= (ELF_file& l, ELF_file&& r);
		
		Elf32_Ehdr& get_elf_header() { return _elf_header; };
		vector<Elf32_Phdr>& get_Phdr_table() { return _Phdr_table; };
		vector<Elf32_Shdr>& get_Shdr_table() { return _Shdr_table; };
		fstream& get_output_file() { return _output_file; };

		private:
		Elf32_Ehdr _elf_header;
		vector<Elf32_Phdr> _Phdr_table;
		vector<Elf32_Shdr> _Shdr_table;

		fstream _output_file; //which he's name is in tmp_file_name
	};

	}//end of namespace ELF
	
}

#endif // ELF_FILE_H_INCLUDED

