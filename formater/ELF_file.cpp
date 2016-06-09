#include "ELF_file.hpp"

#include <elf.h>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
//#include <cstdint>
namespace pasm 
{
	
	namespace ELF
	{

		ELF_file::ELF_file(ELF::Types type, string tmp_file_name)
		{
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
				_elf_header.e_entry = 0x804a250; //randomly and manually choosen
			else
				_elf_header.e_entry = 0x0;
			
			/*_elf_header.e_phoff initilazation*/	
			_elf_header.e_phoff = -1; //will be altered later to the program header table offset
			
			/*_elf_header.e_shoff initilazation*/	
			_elf_header.e_shoff = -1; //will be altered later to the section header table offset
			
			/*_elf_header.e_flags initilazation*/	
			_elf_header.e_flags = 0x0;
			
			/*_elf_header.e_ehsize initilazation*/	
			_elf_header.e_ehsize = sizeof(Elf32_Ehdr);
			
			/*_elf_header.e_phentsize initilazation*/	
			_elf_header.e_phentsize = sizeof(Elf32_Phdr);
			
			/*_elf_header.e_phnum initilazation*/
			//will be altered later to the number of program header table entries
			_elf_header.e_phnum = -1; 
			
			/*_elf_header.e_shentsize initilazation*/	
			_elf_header.e_shentsize = sizeof(Elf32_Shdr);
			
			/*_elf_header.e_shnum initilazation*/
			//will be altered later to the number of section header table entries
			_elf_header.e_shnum = -1; 
			
			/*_elf_header.e_shstrndx initilazation*/
			//will be altered later to the index of the entry associated with section name string table in the section header table
			_elf_header.e_shstrndx = -1;
		}
   }
}
