#ifndef PHGUTILS_H
#define PHGUTILS_H

// OpenGL related
#include "GL/glew.h"
#ifdef WIN32
#include "GL/freeglut.h"
#else
#ifdef __APPLE__
#include "GLUT/glut.h"
#else
#include "GL/glut.h"
#endif
#endif

#ifdef __linux__
#define __forceinline
#endif

/// STL related
#include <algorithm>
#include <assert.h>
#include <functional>
#include <fstream>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <list>
#include <map>
#include <memory>
#include <numeric>
#include <queue>
#include <set>
#include <sstream>
#include <string>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <regex>
using namespace std;

typedef unsigned long idx_t;
typedef unsigned char byte_t;

namespace Constants
{
	const static int NOT_SET = -1;
}

#endif	// PHGUTILS_H
