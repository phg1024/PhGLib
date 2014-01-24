#include "FileMapper.h"

namespace PhGUtils {
FileMapper::FileMapper(const string& filename):
	filename(filename)
{}

FileMapper::~FileMapper(void)
{
}

#ifdef WIN32
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
#else

// char * inPathName, void ** outDataPtr, size_t * outDataLength
bool FileMapper::map() {
    // Return safe values on error.
    pData = nullptr;
    fileSize = 0;

    // Open the file.
    fileDescriptor = open( filename.c_str(), O_RDONLY, 0 );
    if( fileDescriptor < 0 )
    {
       outError = errno;
       cout << "failed to open file " << filename << endl;
       return false;
    }
    else
    {
        // We now know the file exists. Retrieve the file size.
        if( fstat( fileDescriptor, &statInfo ) != 0 )
        {
            cerr << "file does not exists!" << endl;
            outError = errno;
            return false;
        }
        else
        {
            cout << "file size = " << statInfo.st_size << endl;
            // Map the file into a read-only memory region.
            void *paddr = mmap(NULL,
                               statInfo.st_size,
                               PROT_READ,
                               MAP_PRIVATE,
                               fileDescriptor,
                               0);
            pData = reinterpret_cast<char*>(paddr);
            if( paddr == MAP_FAILED )
            {
                outError = errno;
                cerr << "mmap failed." << endl;
                return false;
            }
            else
            {
                // On success, return the size of the mapped file.
                fileSize = statInfo.st_size;
            }
        }

        // Now close the file. The kernel doesn’t use our file descriptor.
        close( fileDescriptor );
    }

    return true;
}

bool FileMapper::unmap() {
    int status = munmap( pData, fileSize );
    return (status == 0);
}

#endif

}
