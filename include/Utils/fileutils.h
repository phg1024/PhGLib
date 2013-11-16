#pragma once

#include "../phgutils.h"

/* get the file extension */
string getFileExtension(const string& filename);

/* get the path to a specific file */
string getPath( const string& filename );

/* get the file name given a full path */
string getFileName( const string& filename );