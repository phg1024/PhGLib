#pragma once

#include "../phgutils.h"
#include "point.hpp"
#include "vector.hpp"

namespace PhGUtils {
class MeshLoader;

template <typename T> class AABBTree;

template <typename VT, typename FT>
class MeshBase
{
public:
	typedef typename VT vert_t;
	typedef typename FT face_t;
	typedef Vector3f norm_t;
	typedef Point2f texcoord_t;

	MeshBase(void){}
	MeshBase(size_t numVerts, size_t numFaces)
	{
		init(numVerts, numFaces);
	}
	~MeshBase(void){}

	virtual void init(size_t numVerts, size_t numFaces) {
		nVerts = numVerts;
		nFaces = numFaces;

		f.resize(nFaces);
		fn.resize(nFaces);
		ft.resize(nFaces);
		v.resize(nVerts);
		n.resize(nVerts);
	}

	virtual void initWithLoader(const MeshLoader& loader) = 0;

	size_t faceCount() const {
		return f.size();
	}

	size_t vertCount() const {
		return v.size();
	}

	virtual face_t& face(size_t idx) {
		return f[idx];
	}
	virtual const face_t& face(size_t idx) const {
		return f[idx];
	}
	virtual vert_t& vertex(size_t idx) {
		return v[idx];
	}
	virtual const vert_t& vertex(size_t idx) const {
		return v[idx];
	}
	virtual const face_t& faceNormal(size_t idx) const {
		return fn[idx];
	}
	virtual face_t& faceNormal(size_t idx) {
		return fn[idx];
	}
	virtual const norm_t& normal(size_t idx) const {
		return n[idx];
	}
	virtual norm_t& normal(size_t idx) {
		return n[idx];
	}

	const set<int>& incidentFaces(size_t idx) const {
		return helper.vfmap.at(idx);
	}

	virtual void draw() const = 0;
	virtual void drawFrame() const = 0;
	virtual void drawFaceIndices() const = 0;

	// helper functions
	virtual float findClosestPoint_bruteforce(const Point3f& p, Point3i& vts, Point3f& bcoords) = 0;
	virtual float findClosestPoint(const Point3f& p, Point3i& vts, Point3f& bcoords, float distThreshold) = 0;

protected:
	template <typename T> friend class AABBTree;
	friend class MeshViewer;
	friend class OBJLoader;
	friend class PLYLoader;
	friend class OBJWriter;
	friend class PLYWriter;

	virtual void buildVertexFaceMap() = 0;
	virtual void buildAABB() = 0;

protected:
	size_t nVerts, nFaces;
	vector<face_t> f;
	vector<face_t> fn;		// normals indices for vertices on a face
	vector<face_t> ft;		// texture coordinates indices for vertices on a face
	vector<vert_t> v;
	vector<norm_t> n;
	vector<texcoord_t> t;

	struct MeshHelper {
		vector<set<int>> vfmap;		// vertex to face map
		shared_ptr<AABBTree<float>> aabb;
	} helper;
};
}