#include "Mesh.h"
#include "GL/glut.h"
#include "MeshLoader.h"
#include "geometryutils.hpp"

namespace PhGUtils {
// ----------------------------------------------------------------------------
// Triangle mesh
// ----------------------------------------------------------------------------
void TriMesh::draw()
{
	glEnableClientState(GL_VERTEX_ARRAY);
	// cast the vertices to floats
	glVertexPointer(3, GL_FLOAT, 0, reinterpret_cast<float*>(&(v[0])));
	glDrawArrays( GL_TRIANGLES, 0,  nFaces*3);					// Draw All Of The Triangles At Once
	glDisableClientState( GL_VERTEX_ARRAY );					// Disable Vertex Arrays
}

void TriMesh::drawFrame() {

}

void TriMesh::drawFaceIndices() {

}

// ----------------------------------------------------------------------------
// Quad mesh
// ----------------------------------------------------------------------------
void QuadMesh::drawFrame() {
	for(int i=0;i<nFaces;i++) {
		face_t& f = face(i);
		vert_t& v1 = vertex(f.x);
		vert_t& v2 = vertex(f.y);
		vert_t& v3 = vertex(f.z);
		vert_t& v4 = vertex(f.w);

		glBegin(GL_LINE_LOOP);

		glVertex3f(v1.x, v1.y, v1.z);
		glVertex3f(v2.x, v2.y, v2.z);
		glVertex3f(v3.x, v3.y, v3.z);
		glVertex3f(v4.x, v4.y, v4.z);

		glEnd();
	}
}

void QuadMesh::drawFaceIndices()
{
	glBegin(GL_QUADS);

	for(int i=0;i<nFaces;i++) {
		face_t& f = face(i);
		//face_t& fn = faceNormal(i);
		vert_t& v1 = vertex(f.x);
		vert_t& v2 = vertex(f.y);
		vert_t& v3 = vertex(f.z);
		vert_t& v4 = vertex(f.w);
				
		float r, g, b;
		encodeIndex<float>(i, r, g, b);
		glColor4f(r, g, b, 1.0);

		glVertex3f(v1.x, v1.y, v1.z);
		glVertex3f(v2.x, v2.y, v2.z);
		glVertex3f(v3.x, v3.y, v3.z);
		glVertex3f(v4.x, v4.y, v4.z);
	}

	glEnd();
}

void QuadMesh::draw()
{
	for(int i=0;i<nFaces;i++) {
		face_t& f = face(i);
		//face_t& fn = faceNormal(i);
		vert_t& v1 = vertex(f.x);
		vert_t& v2 = vertex(f.y);
		vert_t& v3 = vertex(f.z);
		vert_t& v4 = vertex(f.w);

		//norm_t& n1 = normal(fn.x);
		//norm_t& n2 = normal(fn.y);
		//norm_t& n3 = normal(fn.z);
		//norm_t& n4 = normal(fn.w);

		// compute normal
		norm_t n = Vector3f(v1, v2).cross(Vector3f(v1, v4));
		n.normalize();

		glBegin(GL_QUADS);
		glNormal3f(n.x, n.y, n.z);
		//glNormal3f(n1.x, n1.y, n1.z);
		glVertex3f(v1.x, v1.y, v1.z);
		//glNormal3f(n2.x, n2.y, n2.z);
		glVertex3f(v2.x, v2.y, v2.z);
		//glNormal3f(n3.x, n3.y, n3.z);
		glVertex3f(v3.x, v3.y, v3.z);
		//glNormal3f(n4.x, n4.y, n4.z);
		glVertex3f(v4.x, v4.y, v4.z);

		glEnd();
	}
}

void QuadMesh::initWithLoader( const MeshLoader& loader )
{
	const vector<MeshLoader::face_t>& faces = loader.getFaces();
	const vector<MeshLoader::vert_t>& verts = loader.getVerts();
	const vector<MeshLoader::norm_t>& norms = loader.getNormals();
	const vector<MeshLoader::texcoord_t>& texcoords = loader.getTexcoords();

	this->init(verts.size(), faces.size());

	// fill in the vertices and faces
	v = verts;
	for(size_t i=0;i<faces.size();i++) {
		f[i] = face_t(faces[i].v[0], faces[i].v[1], faces[i].v[2], faces[i].v[3]);
		fn[i] = face_t(faces[i].n[0], faces[i].n[1], faces[i].n[2], faces[i].n[3]);
		fn[i] = face_t(faces[i].t[0], faces[i].t[1], faces[i].t[2], faces[i].t[3]);
	}
	n = norms;
	t = texcoords;
}

// ----------------------------------------------------------------------------
// General polygon mesh
// ----------------------------------------------------------------------------
void PolygonMesh::draw()
{

}

}