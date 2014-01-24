#pragma once

/// @brief Axis-Aligned Bounding Box class.

#include "../phgutils.h"
#include "geometryutils.hpp"
#include "../Core/Tree.hpp"
#include "Mesh.h"
#include "../OpenGL/glutilities.h"
#include "../Utils/utility.hpp"

namespace PhGUtils {
/* Axis-Aligned Bounding Box class */
template<typename T>
struct AABB {
public:
#ifdef WIN32
	typedef typename Point3<T> point_t;
	typedef typename Vector3<T> vector_t;
#else
    typedef Point3<T> point_t;
    typedef Vector3<T> vector_t;
#endif
    enum Axis {
        X = 0,
        Y = 1,
        Z = 2
    };

    AABB() {
        for(int i=0;i<3;i++) {
			minCoord[i] = numeric_limits<T>::max();
            maxCoord[i] = -numeric_limits<T>::max();
        }
    }

	AABB(const point_t& minPt, const point_t& maxPt) {
		for(int i=0;i<3;i++) {
			minCoord[i] = minPt[i];
			maxCoord[i] = maxPt[i];
		}
	}

	AABB(const AABB& other) {
		for(int i=0;i<3;i++) {
			minCoord[i] = other.minCoord[i];
			maxCoord[i] = other.maxCoord[i];
		}
	}

	AABB& operator=(const AABB& other) {
		if( this != &other ) {
			for(int i=0;i<3;i++) {
				minCoord[i] = other.minCoord[i];
				maxCoord[i] = other.maxCoord[i];
			}
		}
		return (*this);
	}

	bool isInside(const point_t& p);

    /// @brief intersection test with a infinite length ray
    bool intersectTest(const point_t& origin, const point_t& direction) const;
    /// @brief intersection test with a finite length ray
    bool intersectTestInRange(const point_t& orig, const point_t& dest,
                              const vector_t& dir, const vector_t& invDir) const;

	/// @brief intersection test with another AABB
	bool intersectsAABB(const AABB& other) const;

    T range(Axis axis) const
    {
        return maxCoord[axis] - minCoord[axis];
    }

	T XRange() const { return maxCoord[0] - minCoord[0]; }
	T YRange() const { return maxCoord[1] - minCoord[1]; }
	T ZRange() const { return maxCoord[2] - minCoord[2]; }

	T minX() const {return minCoord[0]; }
	T& minX() { return minCoord[0]; }

	T minY() const {return minCoord[1]; }
	T& minY() { return minCoord[1]; }

	T minZ() const {return minCoord[2]; }
	T& minZ() { return minCoord[2]; }

	T maxX() const {return maxCoord[0]; }
	T& maxX() { return maxCoord[0]; }

	T maxY() const {return maxCoord[1]; }
	T& maxY() { return maxCoord[1]; }

	T maxZ() const {return maxCoord[2]; }
	T& maxZ() { return maxCoord[2]; }

private:
    T minCoord[3];
    T maxCoord[3];
};

template<typename T>
bool PhGUtils::AABB<T>::intersectsAABB(const AABB& other) const
{
	if (maxX() < other.minX() || 
		maxY() < other.minY() || 
		maxZ() < other.minZ() ||
		minX() > other.maxX() || 
		minY() > other.maxY() ||
		minZ() > other.maxZ() ) 
	{
		return false;
	}
	return true;
}


template<typename T>
bool PhGUtils::AABB<T>::isInside(const typename AABB<T>::point_t& p)
{
	return p.x < maxCoord[0] && p.x > minCoord[0]
	    && p.y < maxCoord[1] && p.y > minCoord[1]
		&& p.z < maxCoord[2] && p.y > minCoord[2];
}


template <typename T>
bool AABB<T>::intersectTest(const typename AABB<T>::point_t &origin, const typename AABB<T>::point_t &direction) const
{
	throw lazy_exception();
}

template <typename T>
bool AABB<T>::intersectTestInRange(const typename AABB<T>::point_t &orig, const typename AABB<T>::point_t &dest,
								const typename AABB<T>::vector_t &dir, const typename AABB<T>::vector_t &invDir) const
{
	T tmin, tmax, tymin, tymax, tzmin, tzmax;

	if( dir.x() >= 0 )
	{
		tmin = (minCoord[0] - orig.x()) * invDir.x();
		tmax = (maxCoord[0] - orig.x()) * invDir.x();
	}
	else
	{
		tmin = (maxCoord[0] - orig.x()) * invDir.x();
		tmax = (minCoord[0] - orig.x()) * invDir.x();
	}

	if( dir.y() >= 0 )
	{
		tymin = (minCoord[1] - orig.y()) * invDir.y();
		tymax = (maxCoord[1] - orig.y()) * invDir.y();
	}
	else
	{
		tymin = (maxCoord[1] - orig.y()) * invDir.y();
		tymax = (minCoord[1] - orig.y()) * invDir.y();
	}

	if( tmin > tymax || tymin > tmax )
		return false;

	if( tymin > tmin )
		tmin = tymin;

	if( tymax < tmax )
		tmax = tymax;

	if( dir.z() >= 0 )
	{
		tzmin = (minCoord[2] - orig.z()) * invDir.z();
		tzmax = (maxCoord[2] - orig.z()) * invDir.z();
	}
	else
	{
		tzmin = (maxCoord[2] - orig.z()) * invDir.z();
		tzmax = (minCoord[2] - orig.z()) * invDir.z();
	}

	if( tmin > tzmax || tzmin > tmax )
		return false;

	if( tzmin > tmin )
		tmin = tzmin;

	if( tzmax < tmax )
		tmax = tzmax;

	// survived all tests
	return ((tmin < 1) && (tmax > 0));
}


/// @brief a binary tree
template <typename T>
struct AABBNode
{
	enum Type {
		EMPTY_NODE = -1,
		INTERNAL_NODE = -2
	};

	AABBNode():leftChild(nullptr), rightChild(nullptr){}
    AABB<T> aabb;
    vector<int> faces;
    int faceIdx;	// face index can be: EMPTY_NODE, INTERNAL_NODE and actual face index
    AABBNode* leftChild;
    AABBNode* rightChild;

private:
	// non-copyable
	AABBNode(const AABBNode&){}
	AABBNode& operator=(const AABBNode&){}
};

template <typename T>
class AABBTree : public Tree<AABBNode<T>>
{
public:
#ifdef WIN32
	typedef typename Point3<T> point_t;
	typedef typename Vector3<T> vector_t;
	typedef typename AABBNode<T> node_t;
#else
    typedef Point3<T> point_t;
    typedef Vector3<T> vector_t;
    typedef AABBNode<T> node_t;
#endif

    AABBTree():Tree<node_t>(){}

    AABBTree(const TriMesh& mesh);
	AABBTree(const QuadMesh& mesh);
    ~AABBTree();

    void paint();

    bool intersectTest(const point_t& origin, const vector_t& direction, float &t, int &faceIdx);
    bool intersectTestInRange(const point_t& origin, const point_t& destination, float &t, int &faceIdx);

private:
    AABBNode<T>* buildAABBTree_TriangleMesh(const vector<int>& faceIndices, const TriMesh&);
	AABBNode<T>* buildAABBTree_QuadMesh(const vector<int>& faceIndices, const QuadMesh&);
    void releaseTree(AABBNode<T>* node);

private:
    AABBTree(const AABBTree&);
    AABBTree<T>& operator=(const AABBTree<T>&);
};

template <typename T>
void AABBTree<T>::paint()
{
	glColor4f(1, 0, 0, 0.25);
	// BFS the tree and draw the boxes
	queue<node_t*> nodeset;
    nodeset.push(this->mRoot);
	while(!nodeset.empty())
	{
		node_t* node = nodeset.front();
		nodeset.pop();
		if( node->leftChild != nullptr )
			nodeset.push(node->leftChild);
		if( node->rightChild != nullptr )
			nodeset.push(node->rightChild);

		drawBoundingBox(
			Point3d(node->aabb.minX(), node->aabb.minY(), node->aabb.minZ()),
			Point3d(node->aabb.maxX(), node->aabb.maxY(), node->aabb.maxZ())
			);
	}
}

template <typename T>
bool AABBTree<T>::intersectTest(const Point3<T> &origin,
							 const Vector3<T> &direction,
							 float &t, int &faceIdx)
{
	throw lazy_exception();
}

template <typename T>
bool AABBTree<T>::intersectTestInRange(const Point3<T> &origin,
									const Point3<T> &destination,
									float &t, int &faceIdx)
{
    /*
	vector_t dir(origin, destination);
	vector_t invDir(1.0 / dir.x(), 1.0 / dir.y(), 1.0 / dir.z());
	if( dir.x() == 0 )
		invDir.x() = numeric_limits<T>::max();
	if( dir.y() == 0 )
		invDir.y() = numeric_limits<T>::max();
	if( dir.z() == 0 )
		invDir.z() = numeric_limits<T>::max();

	bool flag = false;

	// traverse the tree to look for a intersection
	t = numeric_limits<float>::max();

	queue<node_t*> nodeset;
	//int testCount = 0;
    nodeset.push(this->mRoot);
	while(!nodeset.empty())
	{
		node_t* node = nodeset.front();
		nodeset.pop();

		if( node->aabb.intersectTestInRange(origin, destination,
			dir, invDir) )
		{
			if( node->faceIdx == node_t::EMPTY_NODE )
				// empty node, ignore it
					continue;

			if( node->faceIdx == node_t::INTERNAL_NODE )
			{
				if( node->leftChild != nullptr )
				{
					//testCount ++;
					nodeset.push(node->leftChild);
				}
				if( node->rightChild != nullptr )
				{
					//testCount ++;
					nodeset.push(node->rightChild);
				}

				continue;
			}
			else
			{
				// hit an AABB
				float tHit;

				if( mMesh->intersection(origin, destination, tHit, node->faceIdx) )
				{
					if( tHit < t )
					{
						t = tHit;
						faceIdx = node->faceIdx;
						flag = true;
						mMesh->faceHitCount[faceIdx]++;
					}
				}
			}
		}
	}

	//cout << "testCount = " << testCount << endl;
	return flag;
    */
}

template <typename T>
AABBTree<T>::AABBTree(const TriMesh& mesh)
{
	vector<int> faceIndices(mesh.faceCount());
	for(int i=0;i<mesh.faceCount();i++)
		faceIndices[i] = i;

	cout << "building AABB tree ..." << endl;
    this->mRoot = buildAABBTree_TriangleMesh(faceIndices, mesh);
	cout << "AABB tree built." << endl;
}

template <typename T>
PhGUtils::AABBTree<T>::AABBTree(const QuadMesh& mesh)
{
	vector<int> faceIndices(mesh.faceCount());
	for(int i=0;i<mesh.faceCount();i++)
		faceIndices[i] = i;

	cout << "building AABB tree ..." << endl;
    this->mRoot = buildAABBTree_QuadMesh(faceIndices, mesh);
	cout << "AABB tree built." << endl;
}

template <typename T>
AABBTree<T>::~AABBTree()
{
    releaseTree(this->mRoot);
}

template <typename T>
void AABBTree<T>::releaseTree(AABBNode<T> *node)
{
	if( node!= 0 )
	{
		releaseTree(node->leftChild);
		releaseTree(node->rightChild);
		delete node->leftChild;
		delete node->rightChild;
	}
}

template <typename T>
AABBNode<T>* AABBTree<T>::buildAABBTree_TriangleMesh(const vector<int> &faceIndices, const TriMesh& mesh)
{
	// build an AABB Tree from a triangle mesh
	const vector<TriMesh::vert_t>& verts = mesh.v;
	const vector<TriMesh::face_t>& faces = mesh.f;

	// for all faces in the indices set, compute their centers
	// also get the AABB of these faces

	node_t* node = new node_t;
	AABB<T>& aabb = node->aabb;
	node->faces = faceIndices;

	if( faceIndices.size() == 0 )
	{
		// empty node
		node->faceIdx = node_t::EMPTY_NODE;
		node->leftChild = nullptr;
		node->rightChild = nullptr;
		return node;
	}
	else if( faceIndices.size() == 1 )
	{
		// leaf node
		int fidx = faceIndices[0];
		const TriMesh::face_t& face = faces[fidx];
		for(int k=0;k<3;k++)
		{
			float x = verts[face[k]].x;
			float y = verts[face[k]].y;
			float z = verts[face[k]].z;

			if( x > aabb.maxX() ) aabb.maxX() = x;
			if( y > aabb.maxY() ) aabb.maxY() = y;
			if( z > aabb.maxZ() ) aabb.maxZ() = z;

			if( x < aabb.minX() ) aabb.minX() = x;
			if( y < aabb.minY() ) aabb.minY() = y;
			if( z < aabb.minZ() ) aabb.minZ() = z;
		}
		node->faceIdx = fidx;
		node->leftChild = nullptr;
		node->rightChild = nullptr;
		return node;
	}
	else {
		// internal node
		node->faceIdx = node_t::INTERNAL_NODE;
		typedef pair<int, point_t> FaceCenterPair;
		vector<FaceCenterPair> fcpairs;
		fcpairs.reserve(faceIndices.size());

		for(auto it=faceIndices.begin();it!=faceIndices.end();it++)
		{
			const TriMesh::face_t& face = faces[(*it)];

			point_t center;

			for(int k=0;k<3;k++)
			{
				float x = verts[face[k]].x;
				float y = verts[face[k]].y;
				float z = verts[face[k]].z;

				center += point_t(x, y, z);

				if( x > aabb.maxX() ) aabb.maxX() = x;
				if( y > aabb.maxY() ) aabb.maxY() = y;
				if( z > aabb.maxZ() ) aabb.maxZ() = z;

				if( x < aabb.minX() ) aabb.minX() = x;
				if( y < aabb.minY() ) aabb.minY() = y;
				if( z < aabb.minZ() ) aabb.minZ() = z;
			}

			center /= 3.0;

			fcpairs.push_back(make_pair((*it), center));
		}

		// sort the pairs along the longest axis
		float ranges[3];
		ranges[0] = aabb.range(AABB<T>::X);
		ranges[1] = aabb.range(AABB<T>::Y);
		ranges[2] = aabb.range(AABB<T>::Z);

		if( ranges[0] >= ranges[1] )
		{
			if( ranges[0] >= ranges[2] )
			{
				// sort along x axis
				sort(fcpairs.begin(), fcpairs.end(), [] (const FaceCenterPair& fc1,
					const FaceCenterPair& fc2) -> bool {
						if( fc1.second.x < fc2.second.x )
							return true;
						else
							return false;
				});
			}
			else
			{
				// sort along z axis
				sort(fcpairs.begin(), fcpairs.end(), [] (const FaceCenterPair& fc1,
					const FaceCenterPair& fc2) -> bool {
						if( fc1.second.z < fc2.second.z )
							return true;
						else
							return false;
				});
			}
		}
		else
		{
			if( ranges[1] >= ranges[2] )
			{
				// sort along y axis
				sort(fcpairs.begin(), fcpairs.end(), [] (const FaceCenterPair& fc1,
					const FaceCenterPair& fc2) -> bool {
						if( fc1.second.y < fc2.second.y )
							return true;
						else
							return false;
				});
			}
			else
			{
				// sort along z axis
				sort(fcpairs.begin(), fcpairs.end(), [] (const FaceCenterPair& fc1,
					const FaceCenterPair& fc2) -> bool {
						if( fc1.second.z < fc2.second.z )
							return true;
						else
							return false;
				});
			}
		}

		// split the sorted faces set, build two children nodes
		vector<int> leftSet, rightSet;
		int midPoint = fcpairs.size() / 2;
		leftSet.reserve(midPoint);
		rightSet.reserve(fcpairs.size()-midPoint);

		//cout << fcpairs.size() << ", " << midPoint << endl;
		for(int i=0;i<midPoint;i++)
		{
			leftSet.push_back(fcpairs[i].first);
		}

		for(int i=midPoint;i<fcpairs.size();i++)
		{
			rightSet.push_back(fcpairs[i].first);
		}

		node->leftChild = buildAABBTree_TriangleMesh(leftSet, mesh);
		node->rightChild = buildAABBTree_TriangleMesh(rightSet, mesh);

		return node;
	}
}

template <typename T>
AABBNode<T>* PhGUtils::AABBTree<T>::buildAABBTree_QuadMesh(const vector<int>& faceIndices, const QuadMesh& mesh)
{
	// build an AABB Tree from a triangle mesh
	const vector<QuadMesh::vert_t>& verts = mesh.v;
	const vector<QuadMesh::face_t>& faces = mesh.f;

	// for all faces in the indices set, compute their centers
	// also get the AABB of these faces

	node_t* node = new node_t;
	AABB<T>& aabb = node->aabb;
	node->faces = faceIndices;

	if( faceIndices.size() == 0 )
	{
		// empty node
		node->faceIdx = node_t::EMPTY_NODE;
		node->leftChild = nullptr;
		node->rightChild = nullptr;
		return node;
	}
	else if( faceIndices.size() == 1 )
	{
		// leaf node
		int fidx = faceIndices[0];
		const QuadMesh::face_t& face = faces[fidx];
		for(int k=0;k<4;k++)
		{
			float x = verts[face[k]].x;
			float y = verts[face[k]].y;
			float z = verts[face[k]].z;

			if( x > aabb.maxX() ) aabb.maxX() = x;
			if( y > aabb.maxY() ) aabb.maxY() = y;
			if( z > aabb.maxZ() ) aabb.maxZ() = z;

			if( x < aabb.minX() ) aabb.minX() = x;
			if( y < aabb.minY() ) aabb.minY() = y;
			if( z < aabb.minZ() ) aabb.minZ() = z;
		}
		node->faceIdx = fidx;
		node->leftChild = nullptr;
		node->rightChild = nullptr;
		return node;
	}
	else {
		// internal node
		node->faceIdx = node_t::INTERNAL_NODE;
		typedef pair<int, point_t> FaceCenterPair;
		vector<FaceCenterPair> fcpairs;
		fcpairs.reserve(faceIndices.size());

		for(auto it=faceIndices.begin();it!=faceIndices.end();it++)
		{
			const QuadMesh::face_t& face = faces[(*it)];

			point_t center;

			for(int k=0;k<4;k++)
			{
				float x = verts[face[k]].x;
				float y = verts[face[k]].y;
				float z = verts[face[k]].z;

				center += point_t(x, y, z);

				if( x > aabb.maxX() ) aabb.maxX() = x;
				if( y > aabb.maxY() ) aabb.maxY() = y;
				if( z > aabb.maxZ() ) aabb.maxZ() = z;

				if( x < aabb.minX() ) aabb.minX() = x;
				if( y < aabb.minY() ) aabb.minY() = y;
				if( z < aabb.minZ() ) aabb.minZ() = z;
			}

			center /= 4.0;

			fcpairs.push_back(make_pair((*it), center));
		}

		// sort the pairs along the longest axis
		float ranges[3];
		ranges[0] = aabb.range(AABB<T>::X);
		ranges[1] = aabb.range(AABB<T>::Y);
		ranges[2] = aabb.range(AABB<T>::Z);

		if( ranges[0] >= ranges[1] )
		{
			if( ranges[0] >= ranges[2] )
			{
				// sort along x axis
				sort(fcpairs.begin(), fcpairs.end(), [] (const FaceCenterPair& fc1,
					const FaceCenterPair& fc2) -> bool {
						if( fc1.second.x < fc2.second.x )
							return true;
						else
							return false;
				});
			}
			else
			{
				// sort along z axis
				sort(fcpairs.begin(), fcpairs.end(), [] (const FaceCenterPair& fc1,
					const FaceCenterPair& fc2) -> bool {
						if( fc1.second.z < fc2.second.z )
							return true;
						else
							return false;
				});
			}
		}
		else
		{
			if( ranges[1] >= ranges[2] )
			{
				// sort along y axis
				sort(fcpairs.begin(), fcpairs.end(), [] (const FaceCenterPair& fc1,
					const FaceCenterPair& fc2) -> bool {
						if( fc1.second.y < fc2.second.y )
							return true;
						else
							return false;
				});
			}
			else
			{
				// sort along z axis
				sort(fcpairs.begin(), fcpairs.end(), [] (const FaceCenterPair& fc1,
					const FaceCenterPair& fc2) -> bool {
						if( fc1.second.z < fc2.second.z )
							return true;
						else
							return false;
				});
			}
		}

		// split the sorted faces set, build two children nodes
		vector<int> leftSet, rightSet;
		int midPoint = fcpairs.size() / 2;
		leftSet.reserve(midPoint);
		rightSet.reserve(fcpairs.size()-midPoint);

		//cout << fcpairs.size() << ", " << midPoint << endl;
		for(int i=0;i<midPoint;i++)
		{
			leftSet.push_back(fcpairs[i].first);
		}

		for(int i=midPoint;i<fcpairs.size();i++)
		{
			rightSet.push_back(fcpairs[i].first);
		}

		node->leftChild = buildAABBTree_QuadMesh(leftSet, mesh);
		node->rightChild = buildAABBTree_QuadMesh(rightSet, mesh);

		return node;
	}
}


}
