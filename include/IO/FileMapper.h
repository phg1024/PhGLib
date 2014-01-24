#pragma once

#ifdef WIN32
#include <Windows.h>
#include <tchar.h>
#else
#include <sys/file.h>
#include <sys/fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#endif

#include <stdio.h>
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

#ifdef WIN32
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
#else
    int outError;
    struct stat statInfo;
    int fileDescriptor;
    char* pData;
    size_t fileSize;

public:
    int error() const { return outError; }
#endif
};

}
