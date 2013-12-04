#pragma once

#include "../phgutils.h"

namespace PhGUtils {
/* get the file extension */
string getFileExtension(const string& filename);

/* get the path to a specific file */
string getPath( const string& filename );

/* get the file name given a full path */
string getFileName( const string& filename );

/* write a iteratable item to file */
template <typename T>
bool write2file(const T& v, const string& filename)
{
	try{
		ofstream fout(filename, ios::out);
		for(auto it=v.begin();it!=v.end();it++) {
			fout << (*it);
		}
		fout.close();
		return true;
	}
	catch(...) {
		cerr << "Failed to write to file " << filename << endl;
		return false;
	}
}
}