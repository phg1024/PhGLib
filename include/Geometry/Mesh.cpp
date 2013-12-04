#include "Mesh.h"
#include "GL/glut.h"
#include "MeshLoader.h"

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
		glVertex3f(v3.x, v3.y, v3.z);
		glVertex3f(v4.x, v4.y, v4.z);

		glEnd();
	}
}

void QuadMesh::draw()
{
	glEnableClientState(GL_VERTEX_ARRAY);
	// cast the vertices to floats
	glVertexPointer(3, GL_FLOAT, 0, reinterpret_cast<float*>(&(v[0])));
	glDrawArrays( GL_QUADS, 0,  nFaces*4);					// Draw All Of The Quads
	glDisableClientState( GL_VERTEX_ARRAY );					// Disable Vertex Arrays
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