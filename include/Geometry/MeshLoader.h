#pragma once

#include "../phgutils.h"
#include "point.hpp"
#include "vector.hpp"

class MeshLoader
{
public:
	virtual bool load(const string& filename) = 0;

protected:
	typedef Point3f vert_t;
	struct face_t {
		face_t() {
			// at least 3 vertices
			v.reserve(8);
			n.reserve(8);
			t.reserve(8);
		}
		vector<int> v, n, t;
		Vector3f normal;
	};
	typedef Point2f texcoord_t;
	typedef Vector3f norm_t;

	bool triangulated;
	bool hasVertexTexCoord;
	bool hasVertexNormal;
	vector<vert_t> verts;
	vector<face_t> faces;
	vector<texcoord_t> texcoords;
	vector<norm_t> normals;

protected:
	void clear();
	void triangulate();
	void estimateNormals();
};

class PLYLoader : public MeshLoader
{
public:
	virtual bool load(const string& filename);

protected:
	struct vert_t {
		float x, y, z;
		unsigned char r, g, b;
	};

	struct face_t {
		unsigned char nVerts;
		vector<int> verts;
	};
};

class OBJLoader : public MeshLoader
{
public:
	virtual bool load(const string& filename);
};