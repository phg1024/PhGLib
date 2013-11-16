#include "FileMapper.h"


FileMapper::FileMapper(const string& filename):
	filename(filename)
{}

FileMapper::~FileMapper(void)
{
}

bool FileMapper::unmap() {

	// Close the file mapping object and the open file
	bFlag = UnmapViewOfFile(lpMapAddress);
	bFlag = CloseHandle(hMapFile); // close the file mapping object

	if(!bFlag)
	{
		cerr << "Error " << GetLastError() << " occurred closing the mapping object!" << endl;
		return false;
	}

	bFlag = CloseHandle(hFile);   // close the file itself

	if(!bFlag)
	{
		cerr << "Error " << GetLastError() << " occurred closing the mapping object!" << endl;
		return false;
	}

	return true;
}

bool FileMapper::map()
{
	std::wstring str(filename.begin(), filename.end());
	TCHAR * lpcTheFile = (TCHAR*)str.c_str();

	// open the file
	hFile = CreateFile(lpcTheFile,
		GENERIC_READ | GENERIC_WRITE,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	if (hFile == INVALID_HANDLE_VALUE)
	{
		cerr << "hFile is NULL" << endl;
		cerr << "Target file is " << lpcTheFile << endl;
		return false;
	}

	// Get the system allocation granularity.
	GetSystemInfo(&SysInfo);
	dwSysGran = SysInfo.dwAllocationGranularity;

	// Get the size of the file
	dwFileSize = GetFileSize(hFile,  NULL);	
	dwFileMapSize = dwFileSize;

	// Create a file mapping object for the file
	// Note that it is a good idea to ensure the file size is not zero
	hMapFile = CreateFileMapping( 
		hFile,          // current file handle
		NULL,           // default security
		PAGE_READWRITE, // read/write permission
		0,              // size of mapping object, high
		dwFileMapSize,  // size of mapping object, low
		NULL			// name of mapping object
		);

	if (hMapFile == NULL) {
		cerr << "hMapFile is NULL: last error "<< GetLastError() << endl;;
		return false;
	}

	dwMapViewSize = dwFileMapSize;

	lpMapAddress = MapViewOfFile(
		hMapFile,				// handle to mapping object
		FILE_MAP_ALL_ACCESS,	// read/write
		0,						// high-order 32 bits of file offset
		0,						// low-order 32 bits of file offset
		dwMapViewSize			// number of bytes to map
		);      

	if (lpMapAddress == NULL) {
		cerr << "lpMapAddress is NULL: last error " << GetLastError() << endl;
		return false;
	}

	pData = (char*)lpMapAddress;

	return true;
}