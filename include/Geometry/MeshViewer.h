#pragma once

#include "../OpenGL/gl3dcanvas.h"
#include "Mesh.h"
#include "AABB.hpp"

namespace PhGUtils {

class MeshViewer : public GL3DCanvas
{
public:
	MeshViewer(QWidget* parent = 0);
	~MeshViewer(void);

	void bindMesh(const shared_ptr<QuadMesh>& m) {
		quad = m;
		type = QUAD_MESH;
		aabb = shared_ptr<AABBTree<float>>(new AABBTree<float>(*quad));

		updateViewingParameters();
	}
	void bindMesh(const shared_ptr<TriMesh> m) {
		tri = m;
		type = TRIANGLE_MESH;
		aabb = shared_ptr<AABBTree<float>>(new AABBTree<float>(*tri));

		updateViewingParameters();
	}

protected:
	void initializeGL();
	void paintGL();
	void resizeGL(int w, int h);

	void updateViewingParameters();
	void enableLighting();
	void disableLighting();

private:
	enum MeshType {
		QUAD_MESH,
		TRIANGLE_MESH
	} type;

	shared_ptr<QuadMesh> quad;
	shared_ptr<TriMesh> tri;

	shared_ptr<AABBTree<float>> aabb;
	Point3f massCenter;
	float scale;
};

}