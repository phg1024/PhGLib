#pragma once

#include "MeshBase.hpp"

namespace PhGUtils {
class MeshLoader;

class TriMesh : MeshBase<Point3f, Point3i>
{
public:
	typedef MeshBase<Point3f, Point3i> base_t;
	typedef base_t::face_t face_t;
	typedef base_t::vert_t vert_t;
	typedef base_t::norm_t norm_t;
	typedef base_t::texcoord_t texcoord_t;

	TriMesh():MeshBase<Point3f, Point3i>(0, 0){}
	TriMesh(size_t numVerts, size_t numFaces):
		MeshBase<Point3f, Point3i>(numVerts, numFaces)
	{}
	~TriMesh(){}

	virtual void initWithLoader(const MeshLoader& loader);

	virtual void draw();
	virtual void drawFrame();
	virtual void drawFaceIndices();

	friend class OBJLoader;
private:
};

class QuadMesh : public MeshBase<Point3f, Point4i>
{
public:
	typedef MeshBase<Point3f, Point4i> base_t;
	typedef base_t::face_t face_t;
	typedef base_t::vert_t vert_t;
	typedef base_t::norm_t norm_t;
	typedef base_t::texcoord_t texcoord_t;

	QuadMesh():MeshBase<Point3f, Point4i>(0, 0){}
	QuadMesh(size_t numVerts, size_t numFaces):
		MeshBase<Point3f, Point4i>(numVerts, numFaces)
	{}
	~QuadMesh(){}

	virtual void initWithLoader(const MeshLoader& loader);

	virtual void draw();
	virtual void drawFrame();
	virtual void drawFaceIndices();

	friend class OBJLoader;
private:
};

class PolygonMesh : public MeshBase<Point3f, vector<int>>
{
public:
	typedef MeshBase<Point3f, vector<int>> base_t;
	typedef base_t::face_t face_t;
	typedef base_t::vert_t vert_t;
	typedef base_t::norm_t norm_t;
	typedef base_t::texcoord_t texcoord_t;

	PolygonMesh():MeshBase<Point3f, vector<int>>(0, 0){}
	~PolygonMesh(){}

	virtual void initWithLoader(const MeshLoader& loader);

	virtual void draw();
	virtual void drawFrame();
	virtual void drawFaceIndices();

private:
};

}