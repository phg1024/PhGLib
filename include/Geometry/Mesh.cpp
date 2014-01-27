#include "Mesh.h"
#ifdef WIN32
#include "GL/freeglut.h"
#else
#include "GLUT/glut.h"
#endif
#include "MeshLoader.h"
#include "geometryutils.hpp"
#include "AABB.hpp"
#include "../Utils/utility.hpp"

namespace PhGUtils {
// ----------------------------------------------------------------------------
// Triangle mesh
// ----------------------------------------------------------------------------
void TriMesh::draw() const {
	
#if 0
	glEnableClientState(GL_VERTEX_ARRAY);
	// cast the vertices to floats
	glVertexPointer(3, GL_FLOAT, 0, reinterpret_cast<const float*>(&(v[0])));
	glDrawArrays( GL_TRIANGLES, 0,  nFaces*3);					// Draw All Of The Triangles At Once
	glDisableClientState( GL_VERTEX_ARRAY );					// Disable Vertex Arrays
#else
	for(int i=0;i<nFaces;i++) {
		const face_t& f = face(i);
		//face_t& fn = faceNormal(i);
		const vert_t& v1 = vertex(f.x);
		const vert_t& v2 = vertex(f.y);
		const vert_t& v3 = vertex(f.z);

		//norm_t& n1 = normal(fn.x);
		//norm_t& n2 = normal(fn.y);
		//norm_t& n3 = normal(fn.z);
		//norm_t& n4 = normal(fn.w);

		// compute normal
		norm_t n = Vector3f(v1, v2).cross(Vector3f(v1, v3));
		n.normalize();

		glBegin(GL_TRIANGLES);
		glNormal3f(n.x, n.y, n.z);
		//glNormal3f(n1.x, n1.y, n1.z);
		glVertex3f(v1.x, v1.y, v1.z);
		//glNormal3f(n2.x, n2.y, n2.z);
		glVertex3f(v2.x, v2.y, v2.z);
		//glNormal3f(n3.x, n3.y, n3.z);
		glVertex3f(v3.x, v3.y, v3.z);

		glEnd();
	}
#endif
}

void TriMesh::drawFrame() const {
	for(int i=0;i<nFaces;i++) {
		const face_t& f = face(i);
		const vert_t& v1 = vertex(f.x);
		const vert_t& v2 = vertex(f.y);
		const vert_t& v3 = vertex(f.z);

		glBegin(GL_LINE_LOOP);

		glVertex3f(v1.x, v1.y, v1.z);
		glVertex3f(v2.x, v2.y, v2.z);
		glVertex3f(v3.x, v3.y, v3.z);

		glEnd();
	}
}

void TriMesh::drawFaceIndices() const {
	glBegin(GL_TRIANGLES);

	for(int i=0;i<nFaces;i++) {
		const face_t& f = face(i);
		//face_t& fn = faceNormal(i);
		const vert_t& v1 = vertex(f.x);
		const vert_t& v2 = vertex(f.y);
		const vert_t& v3 = vertex(f.z);

		float r, g, b;
		encodeIndex<float>(i, r, g, b);
		glColor4f(r, g, b, 1.0);

		glVertex3f(v1.x, v1.y, v1.z);
		glVertex3f(v2.x, v2.y, v2.z);
		glVertex3f(v3.x, v3.y, v3.z);
	}

	glEnd();
}

void TriMesh::initWithLoader(const MeshLoader& loader)
{
	const vector<MeshLoader::face_t>& faces = loader.getFaces();
	const vector<MeshLoader::vert_t>& verts = loader.getVerts();
	const vector<MeshLoader::norm_t>& norms = loader.getNormals();
	const vector<MeshLoader::texcoord_t>& texcoords = loader.getTexcoords();

	this->init(verts.size(), faces.size());

	// fill in the vertices and faces
	v = verts;
	for(size_t i=0;i<faces.size();i++) {

		// must not be an empty face
		assert( !faces[i].v.empty() );
		f[i] = face_t(faces[i].v[0], faces[i].v[1], faces[i].v[2]);

		if( !faces[i].n.empty() )
			fn[i] = face_t(faces[i].n[0], faces[i].n[1], faces[i].n[2]);

		if( !faces[i].t.empty() )
			ft[i] = face_t(faces[i].t[0], faces[i].t[1], faces[i].t[2]);
	}
	n = norms;
	t = texcoords;

	buildVertexFaceMap();
	buildAABB();
}

void TriMesh::buildVertexFaceMap()
{
	helper.vfmap.resize(v.size());

	for(size_t i=0;i<f.size();i++) {
		face_t& f = face(i);

		helper.vfmap[f.x].insert(i);
		helper.vfmap[f.y].insert(i);
		helper.vfmap[f.z].insert(i);
	}
}

void TriMesh::buildAABB()
{
	helper.aabb = shared_ptr<AABBTree<float>>(new AABBTree<float>(*this));
}

float TriMesh::findClosestPoint_bruteforce(const Point3f& p, Point3i& vts, Point3f& bcoords)
{
#undef max
	// iterate through all faces and find a closest match
	float closestDist = numeric_limits<float>::max();
	std::for_each(f.begin(), f.end(), [&](const face_t& face) {
		Point3f hit;
		float dist = pointToTriangleDistance(p, v[face.x], v[face.y], v[face.z], hit);
		if( dist < closestDist ) {
			closestDist = dist;
			vts = face;
		}
	});

	computeBarycentricCoordinates(p, v[vts.x], v[vts.y], v[vts.z], bcoords);
	return closestDist;
}

float TriMesh::findClosestPoint(const Point3f& p, Point3i& vts, Point3f& bcoords, float distThreshold)
{
	throw lazy_exception();
}



// ----------------------------------------------------------------------------
// Quad mesh
// ----------------------------------------------------------------------------
void QuadMesh::drawFrame() const {
	for(int i=0;i<nFaces;i++) {
		const face_t& f = face(i);
		const vert_t& v1 = vertex(f.x);
		const vert_t& v2 = vertex(f.y);
		const vert_t& v3 = vertex(f.z);
		const vert_t& v4 = vertex(f.w);

		glBegin(GL_LINE_LOOP);

		glVertex3f(v1.x, v1.y, v1.z);
		glVertex3f(v2.x, v2.y, v2.z);
		glVertex3f(v3.x, v3.y, v3.z);
		glVertex3f(v4.x, v4.y, v4.z);

		glEnd();
	}
}

void QuadMesh::drawFaceIndices() const
{
	glBegin(GL_QUADS);

	for(int i=0;i<nFaces;i++) {
		const face_t& f = face(i);
		//face_t& fn = faceNormal(i);
		const vert_t& v1 = vertex(f.x);
		const vert_t& v2 = vertex(f.y);
		const vert_t& v3 = vertex(f.z);
		const vert_t& v4 = vertex(f.w);
				
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

void QuadMesh::draw() const
{
	for(int i=0;i<nFaces;i++) {
		const face_t& f = face(i);
		//face_t& fn = faceNormal(i);
		const vert_t& v1 = vertex(f.x);
		const vert_t& v2 = vertex(f.y);
		const vert_t& v3 = vertex(f.z);
		const vert_t& v4 = vertex(f.w);

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
		assert( !faces[i].v.empty() );
		f[i] = face_t(faces[i].v[0], faces[i].v[1], faces[i].v[2], faces[i].v[3]);
		if( !faces[i].n.empty() )
			fn[i] = face_t(faces[i].n[0], faces[i].n[1], faces[i].n[2], faces[i].n[3]);
		if( !faces[i].t.empty() )
			ft[i] = face_t(faces[i].t[0], faces[i].t[1], faces[i].t[2], faces[i].t[3]);
	}
	n = norms;
	t = texcoords;

	buildVertexFaceMap();
	buildAABB();
}

void QuadMesh::buildVertexFaceMap()
{
	helper.vfmap.resize(v.size());

	for(size_t i=0;i<f.size();i++) {
		face_t& f = face(i);

		helper.vfmap[f.x].insert(i);
		helper.vfmap[f.y].insert(i);
		helper.vfmap[f.z].insert(i);
		helper.vfmap[f.w].insert(i);
	}
}

void QuadMesh::buildAABB()
{
	helper.aabb = shared_ptr<AABBTree<float>>(new AABBTree<float>(*this));
}

float QuadMesh::findClosestPoint_bruteforce(const Point3f& p, Point3i& vts, Point3f& bcoords)
{
	// iterate through all faces and find a closest match
	float closestDist = numeric_limits<float>::max();
	std::for_each(f.begin(), f.end(), [&](const face_t& face) {
		Point3f hit1, hit2;
		float dist1 = pointToTriangleDistance(p, v[face.x], v[face.y], v[face.z], hit1);
		float dist2 = pointToTriangleDistance(p, v[face.y], v[face.z], v[face.w], hit2);
		if( dist1 < dist2 && dist1 < closestDist ) {
				closestDist = dist1;
				vts.x = face.x, vts.y = face.y, vts.z = face.z;
		}
		else if( dist2 < closestDist ) {
				closestDist = dist2;
				vts.x = face.y, vts.y = face.z, vts.z = face.w;
		}
	});

	computeBarycentricCoordinates(p, v[vts.x], v[vts.y], v[vts.z], bcoords);
	return closestDist;
}

float QuadMesh::findClosestPoint(const Point3f& p, Point3i& vts, Point3f& bcoords, float distThreshold)
{
	// construct an AABB around the query point
	AABB<float> aabb(
		p - Point3f(distThreshold, distThreshold, distThreshold),
		p + Point3f(distThreshold, distThreshold, distThreshold)
		);

	// finds closest point with AABB tree
	typedef const AABBNode<float>* node_cptr;
	// traverse the tree and examine all leaf nodes within distance threshold
	queue<node_cptr> Q;
	Q.push(helper.aabb->root());
	
	float closestDist = numeric_limits<float>::max();

	while( Q.size() ) {
		node_cptr n = Q.front();
		Q.pop();

		if( n->faceIdx == AABBNode<float>::INTERNAL_NODE ) {
			// check children
			node_cptr l = n->leftChild;
			// check left child intersection
			if( l->aabb.intersectsAABB(aabb) ) Q.push(l);

			node_cptr r = n->rightChild;
			// check right child intersection
			if( r->aabb.intersectsAABB(aabb) ) Q.push(r);
		}
		else if( n->faceIdx == AABBNode<float>::EMPTY_NODE ) {
			continue;
		}
		else if( n->faceIdx > 0 ) {			
			// leaf node, compute point-face distance
			face_t face = f[n->faceIdx];
			Point3f hit1, hit2;
			float dist1 = pointToTriangleDistance(p, v[face.x], v[face.y], v[face.z], hit1);
			float dist2 = pointToTriangleDistance(p, v[face.y], v[face.z], v[face.w], hit2);
			if( dist1 < dist2 && dist1 < closestDist ) {
				closestDist = dist1;
				vts.x = face.x, vts.y = face.y, vts.z = face.z;
			}
			else if( dist2 < closestDist ) {
				closestDist = dist2;
				vts.x = face.y, vts.y = face.z, vts.z = face.w;
			}
		}
	}

	if( closestDist < distThreshold ) {
		computeBarycentricCoordinates(p, v[vts.x], v[vts.y], v[vts.z], bcoords);
		return closestDist;
	}
	else {
		// not found
		return -1.0;
	}
}

// ----------------------------------------------------------------------------
// General polygon mesh
// ----------------------------------------------------------------------------
void PolygonMesh::draw()
{

}

}
