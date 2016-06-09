#ifndef ELF_FILE_H_INCLUDED
#define ELF_FILE_H_INCLUDED

#include <elf.h>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
//#include <cstdint>

using namespace std;

namespace pasm
{
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
		Types(Types& other) : _t(other._t){};	
		~Types(){};

		operator uint16_t() const {return static_cast<uint16_t>(this->_t);};	

		enum
		{
		None = ET_NONE,
		Relocatable = ET_REL,
		Object = ET_REL,
		Executable = ET_EXEC,
		SharedObject = ET_DYN,
		Core = ET_CORE,
		};

		uint16_t _t;
	};

	class ELF_file
	{
		public:
		//ELF_file(ELF::Types type = ELF::Types::Object, string tmp_file_name = "obj.tmp");
		ELF_file(ELF::Types type, string tmp_file_name);
		~ELF_file(){};

		private:
		Elf32_Ehdr _elf_header;
		vector<Elf32_Phdr> _Phdr_table;
		vector<Elf32_Shdr> _Shdr_table;

		fstream _text_binary_dump; //which he's name is in tmp_file_name
	};

	}//end of namespace ELF
	
}

#endif // ELF_FILE_H_INCLUDED

