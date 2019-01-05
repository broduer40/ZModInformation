#pragma once
#include "ZMI_DLL.h"

/*
######################################
#
#
  ____________ _ _      
 |___  /  ____(_) |     
    / /| |__   _| | ___ 
   / / |  __| | | |/ _ \
  / /__| |    | | |  __/
 /_____|_|    |_|_|\___|
                        
                        
########################################
# Description: Defines ZFile class and stuff
# Author: ZDisket
# Copyright (C) 2019 YOUR MOM GAY LOLOLOL
#######################################
*/

#include <fstream>
#include <string>
#include <vector>


#include "ByteArr.h"
/*

__if_not_exists (INT64) {
#ifndef INT64
#define INT64 long long
#endif // !INT64

};

__if_not_exists (BYTE) {

#ifndef BYTE
#define BYTE unsigned char
#endif // !BYTE

};


*/





namespace EZFOpenMode {
	enum Enum {
		BinaryRead = 0,
		TextRead,
		BinaryWrite,
		TextWrite
	};
}
/*
Why is this here? Just use sizeof()!

*/
namespace DataSizes {
	enum Sz {
		FLOAT_SZ = 4,
		DOUBLE_SZ = 8,
		LONG_DOUBLE_SZ = 10,
		SIZE_T_SZ = 4,
		INT64_SZ = 8,
		LONG_SZ = 4,
		INT32_SZ = 4,
		INT16_SZ = 2,
		CHAR_SIZE = 1,
		WCHAR_SIZE = 2
	};
}

// ZFile: Class for (mostly binary) file handling.
// Cannot be copied
class ZMI_API ZFile
{
private:
	BYTE * m_pData;
	bool FileOpened;
	std::fstream Stream;

	EZFOpenMode::Enum OpenMode;

	int EZFOpenModeToIos(const EZFOpenMode::Enum& input);

public:
	ZFile();

	ZFile(const std::wstring& coFileName, const EZFOpenMode::Enum& Mode);
	ZFile(const std::string& coFName, const EZFOpenMode::Enum& coMode);

	bool Open(const std::string& in_sFileName,const EZFOpenMode::Enum& in_Mode);
	bool Open(const std::wstring& in_sFileName, const EZFOpenMode::Enum& in_Mode);

	void Seek(const INT64& in_Pos);
	INT64 GetPos();
	
	
	 INT64 GetFileLength();
	// Reads from the file
	// Please pass a pointer to this
	void Read(void* out, const INT64& count);
	// Writes to the file
	// Please pass a pointer
	void Write(void* in, const INT64& incount);

	
	// Read the entire file into a byte array
	ByteArr ReadEntireFile();
	
	// Write with template argument to not pass size.
	// Only works with regular datatypes
    template <typename Dat>
	void Write(const Dat& dta)
	{
		Stream.write((char*)&dta, sizeof(dta));
		
	
	}

	// Read with template argument to not pass size.
   // Only works with regular datatypes
	template <typename Dat>
	void Read(Dat& dta)
	{
		Stream.read((char*)&dta, sizeof(dta));


	}

	

	// Write a string
	template<typename chardat>
	void Write(const std::basic_string<chardat>& Str) {
		// Get total len in bytes.
		const size_t LenInBytes = Str.length() * sizeof(chardat);

		// Write the string length (NOT in bytes)
		Write(Str.length());
		Stream.write((char*)Str.data(),LenInBytes);
		
		
	
	}

	// Read a string
	template<typename chardat>
	void Read(std::basic_string<chardat>& Str) {

		size_t StrLen = 0;
		Read(StrLen);
		chardat* dpBuffer = new chardat[StrLen];


		Stream.read((char*)dpBuffer, sizeof(chardat) * StrLen);

		// For some reason (witchcraft?) our buffer has more chars in it than we actually allocated, which should be impossible.
		// Thankfully, std::string's assign function allows for cutting.
		Str.assign(dpBuffer,0,StrLen);


		delete[] dpBuffer;

	}

	// Write a vector
	template<typename vdat>
	void Write(const std::vector<vdat>& Vec) {
		// Write size in bytes then vector size.
	//	const size_t SzInBytes = Vec.size() * sizeof(vdat);
		Write(Vec.size());

		// Write vector size.

		auto It = Vec.begin();
		
		while (It != Vec.end()) {
			Write(*It);
			++It;
		}
		

		//Stream.write((char*)Vec.data(), SzInBytes);
	
	}

	// Read a vector
	template<typename vdat>
	void Read(std::vector<vdat>& Vec) {
		size_t vSz = 0;
		Read(vSz);
		
		Vec.resize(vSz);

		size_t i = 0;

		while (i != vSz) {
			Read(Vec[i]);
		
			++i;
		}


	}

	// Write some stuff
	template<typename Ty>
	void operator<<(const Ty& In) {
		Write(In);
	
	}

	// Write a Byte Array RAW into the file, without the size. Useful for exporting
	void Write(const ByteArr& BrDat);

	void operator<<(const ByteArr& BarDat) {
		if (BarDat.CoData() == NULL) {
			throw new std::invalid_argument("ZFile tried to write invalid byte array!!");
		}

		Write(BarDat.Size());
		Stream.write((const char*)BarDat.CoData(), BarDat.Size());
	
	}
	// Read to a byte array. Note: DELETES AND REPLACES THE ALREADY EXISTING CONTENTS THERE!!
	void operator>>(ByteArr& BarDat) {
		size_t BaSz = 0;
		Read(BaSz);
		BarDat.CAlloc(BaSz);
		Stream.read((char*)BarDat.GetData(), BaSz);

	}



	template<typename MTy>
	void operator>>(MTy& mIn) {
		Read(mIn);
	}
	
	void Close();


	~ZFile();
};

