#include "fileutils.h"

string getFileExtension(const string& filename)
{
	unsigned found = filename.find_last_of(".");
	return filename.substr(found+1);
}

string getPath( const string& filename )
{
	unsigned found = filename.find_last_of("/\\");
	return filename.substr(0, found);
}

string getFileName( const string& filename )
{
	unsigned found = filename.find_last_of("/\\");
	return filename.substr(found+1);
}