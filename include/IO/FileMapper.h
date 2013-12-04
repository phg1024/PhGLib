#pragma once

#include <Windows.h>
#include <stdio.h>
#include <tchar.h>
#include <string>
#include <iostream>
#include <iterator>
using std::string;
using std::cout;
using std::cerr;
using std::endl;

namespace PhGUtils {
class FileMapper
{
public:
	FileMapper();
	/* default buffer size 32MB */
	FileMapper(const string& filename);
	~FileMapper();

	bool map();
	bool unmap();

	char* buffer() {
		return pData;
	}

private:
	string filename;

	HANDLE hFile;
	HANDLE hMapFile;
	BOOL bFlag;
	DWORD dBytesWritten;
	DWORD dwFileSize;
	DWORD dwFileMapSize;
	DWORD dwMapViewSize;
	DWORD dwSysGran;

	SYSTEM_INFO SysInfo;
	LPVOID lpMapAddress;

	char* pData;
	int i;
	int iData;
	int iViewDelta;
};

}