#pragma once

#include "../phgutils.h"
#include "point.hpp"
#include "vector.hpp"

namespace PhGUtils {
	class QuadMesh;
	class TriMesh;

	class MeshWriter {
	public:
		virtual bool save(const QuadMesh& mesh, const string& filename) = 0;
		virtual bool save(const TriMesh& mesh, const string& filename) = 0;
	};

	class OBJWriter : public MeshWriter {
	public:
		virtual bool save(const QuadMesh& mesh, const string& filename);
		virtual bool save(const TriMesh& mesh, const string& filename);
    bool save(const QuadMesh& mesh, const vector<int> &selectedFaces, const string& filename);    
	};

	class PLYWriter : public MeshWriter {
	public:
		virtual bool save(const QuadMesh& mesh, const string& filename);
		virtual bool save(const TriMesh& mesh, const string& filename);
	};
}
