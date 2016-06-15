#include "ELF_file.hpp"
#include "../encoder/encoder.hpp"

#include <elf.h>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <utility>
//#include <cstdint>

using namespace std;

namespace pasm 
{
	
	int format(const binary_data& text_bin_dump, const string& output_file_name)
	{
		ELF::ELF_file output_file(ELF::Types::Executable, output_file_name);

		output_file._elf_header.e_phoff = sizeof(Elf32_Ehdr);

		output_file._elf_header.e_phnum = 1;
		

		output_file.write(text_bin_dump);
		
		return 0; //OK
	}
	
	namespace ELF
	{

		ELF_file::ELF_file(const ELF::Types& type, const string& output_file_name)
		{
			_output_file = fstream(output_file_name, fstream::out);
			
			/*_elf_header.e_ident initilazation - MAGIC(in readelf)*/	
			_elf_header.e_ident[EI_MAG0] = ELFMAG0; //0x7f - magic number
			_elf_header.e_ident[EI_MAG1] = ELFMAG1; //'E'
			_elf_header.e_ident[EI_MAG2] = ELFMAG2; //'L'
			_elf_header.e_ident[EI_MAG3] = ELFMAG3; //'F'
			_elf_header.e_ident[EI_CLASS] = ELFCLASS32; //32 bit
			_elf_header.e_ident[EI_DATA] = ELFDATA2LSB; //2's complement, little-endian
			_elf_header.e_ident[EI_VERSION] = EV_CURRENT; //current version of the ELF format
			_elf_header.e_ident[EI_OSABI] = ELFOSABI_SYSV; //UNIX System V (ABI)
			_elf_header.e_ident[EI_ABIVERSION] = 0x0; //version of ABI
			for(int i = EI_PAD; i < EI_NIDENT; ++i)//padding bytes
				_elf_header.e_ident[i] = 0x0;

			/*_elf_header.e_type initilazation*/	
			_elf_header.e_type = static_cast<uint16_t>(type);

			/*_elf_header.e_machine initilazation*/	
			_elf_header.e_machine = EM_386; //Intel Architecture 32-bit

			/*_elf_header.e_version initilazation*/	
			_elf_header.e_version = EV_CURRENT;

			
			/*_elf_header.e_entry initilazation*/	
			if(type == Types::Executable)
				_elf_header.e_entry = /*0x804a250*/ 0x8002BAD; //randomly and manually choosen
			else
				_elf_header.e_entry = 0x0;
			
			/*_elf_header.e_phoff initilazation*/	
			_elf_header.e_phoff = 0; //will be altered later to the program header table offset
			
			/*_elf_header.e_shoff initilazation*/	
			_elf_header.e_shoff = 0; //will be altered later to the section header table offset
			
			/*_elf_header.e_flags initilazation*/	
			_elf_header.e_flags = 0x0;
			
			/*_elf_header.e_ehsize initilazation*/	
			_elf_header.e_ehsize = sizeof(Elf32_Ehdr);
			
			/*_elf_header.e_phentsize initilazation*/	
			_elf_header.e_phentsize = sizeof(Elf32_Phdr);
			
			/*_elf_header.e_phnum initilazation*/
			//will be altered later to the number of program header table entries
			_elf_header.e_phnum = 0; 
			
			/*_elf_header.e_shentsize initilazation*/	
			_elf_header.e_shentsize = sizeof(Elf32_Shdr);
			
			/*_elf_header.e_shnum initilazation*/
			//will be altered later to the number of section header table entries
			_elf_header.e_shnum = 0; 
			
			/*_elf_header.e_shstrndx initilazation*/
			//will be altered later to the index of the entry associated with section name string table in the section header table
			_elf_header.e_shstrndx = 0;
		}
		
		ELF_file::~ELF_file()
		{
			_output_file.close();
		}
		
		void ELF_file::write(const binary_data& bin_dump)
		{
			char* elf_hdr = reinterpret_cast<char*>(&_elf_header);
			
			//elf_hdr[sizeof(Elf32_Ehdr) + 1] = NULL;
			
			_output_file.write(elf_hdr, sizeof(Elf32_Ehdr));

			Elf32_Phdr ph;

			ph.p_type = PT_LOAD;

			ph.p_offset = sizeof(Elf32_Ehdr) /* + 1*/;

			ph.p_vaddr = 0x200bad;

			ph.p_paddr = 0;

			ph.p_memsz = 0;

			ph.p_flags = PF_X;

			ph.p_align = 0;

			char* p_hdr = reinterpret_cast<char*>(&ph);

			_output_file.write(p_hdr, sizeof(Elf32_Phdr));

			size_t sum = 0;
			std::vector<char> text;

			for (const auto& d: bin_dump) {
				for (const auto c: d) {
					sum++;
					text.push_back(c);
				}
			}

			
			_output_file.write(&text[0], sum);
		}
		
		
		ELF_file& ELF_file::operator= (ELF_file&& other)
		{
			if(this != &other)
			{
				//free resources
				if(_output_file.is_open() == true)
					_output_file.close();
				if(_Shdr_table.empty() == false)
					_Shdr_table.clear();
				if(_Phdr_table.empty() == false)
					_Phdr_table.clear();
				

				_output_file = move(other.get_output_file());
				other.get_output_file() = fstream();
				
				_Shdr_table = move(other.get_Shdr_table());
				other.get_Shdr_table().clear();
				
				_Phdr_table = move(other.get_Phdr_table());
				other.get_Phdr_table().clear();
			}			
			
			return *this;
		}

   }
}
