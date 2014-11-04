#pragma once

#include "vector.hpp"
#include "point.hpp"
#include "matrix.hpp"

namespace PhGUtils {
struct RigidTransformation;

template <typename T>
/* 
 Given a point, camera position and direction, plus a threshold, determine
 if the point is visible from the camera.
 */
bool checkVisibility(const Point3<T>& p, const Point3<T>& camPos, const Vector3<T>& camDir, float thres)
{
	Vector3<T> camToP(camPos, p);
	camToP.normalize();

	return (-camDir.dot(camToP) > thres);
}

template <typename T>
void projectPoint(T& u, T& v, const T& x, const T& y, const T& z, const Matrix3x3<T>& Pmat) {
  T nx, ny, nz;
  const T* m = Pmat.data();
  nx = m[0] * x + m[1] * y + m[2] * z;
  ny = m[3] * x + m[4] * y + m[5] * z;
  nz = m[6] * x + m[7] * y + m[8] * z;
  u = nx/nz, v = ny/nz;
}

template <typename T>
void projectPoint(T& u, T& v, T& d, const T& x, const T& y, const T& z, const Matrix3x3<T>& Pmat) {
  T nx, ny, nz;
  const T* m = Pmat.data();
  nx = m[0] * x + m[1] * y + m[2] * z;
  ny = m[3] * x + m[4] * y + m[5] * z;
  nz = m[6] * x + m[7] * y + m[8] * z;
  u = nx / nz, v = ny / nz, d = z;  // check if this is correct
}

template <typename T>
void transformPoint(T& x, T& y, T& z, const Matrix3x3<T>& Rmat, const Point3<T>& Tvec ) {
	T nx, ny, nz;
	const T* m = Rmat.data();
	nx = m[0] * x + m[1] * y + m[2] * z + Tvec.x;
	ny = m[3] * x + m[4] * y + m[5] * z + Tvec.y;
	nz = m[6] * x + m[7] * y + m[8] * z + Tvec.z;
	x = nx, y = ny, z = nz;
}

template <typename T>
void rotatePoint(T& x, T& y, T& z, const Matrix3x3<T>& Rmat ) {
	T nx, ny, nz;
	const T* m = Rmat.data();
	nx = m[0] * x + m[1] * y + m[2] * z;
	ny = m[3] * x + m[4] * y + m[5] * z;
	nz = m[6] * x + m[7] * y + m[8] * z;
	x = nx, y = ny, z = nz;
}

template <typename T>
/*
 Construct 3x3 rotation matrix with respect to X axis
 */
Matrix3x3<T> xRotationMatrix(float theta)
{
	Matrix3x3<T> mat;
	T cosTheta = cos(theta), sinTheta = sin(theta);

	mat(0, 0) = 1;

	mat(1, 1) = cosTheta; mat(1, 2) = -sinTheta;
	mat(2, 1) = sinTheta; mat(2, 2) = cosTheta;

	return mat;
}

template <typename T>
/*
 Construct 3x3 rotation matrix with respect to Y axis
 */
Matrix3x3<T> yRotationMatrix(float theta)
{
	Matrix3x3<T> mat;
	T cosTheta = cos(theta), sinTheta = sin(theta);

	mat(0, 0) = cosTheta; mat(0, 2) = sinTheta;
	mat(1, 1) = 1;
	mat(2, 0) = -sinTheta; mat(2, 2) = cosTheta;

	return mat;
}


template <typename T>
/*
 Construct 3x3 rotation matrix with respect to Z axis
 */
Matrix3x3<T> zRotationMatrix(float theta)
{
	Matrix3x3<T> mat;
	T cosTheta = cos(theta), sinTheta = sin(theta);

	mat(0, 0) = cosTheta; mat(0, 1) = -sinTheta;
	mat(1, 0) = sinTheta; mat(1, 1) = cosTheta;
	mat(2, 2) = 1;

	return mat;
}

template <typename T>
/*
 Construct the derivative of 3x3 rotation matrix with respect to X axis
 */
Matrix3x3<T> dxRotationMatrix(float theta)
{
	Matrix3x3<T> mat;
	T cosTheta = cos(theta), sinTheta = sin(theta);

	mat(1, 1) = -sinTheta; mat(1, 2) = -cosTheta;
	mat(2, 1) = cosTheta; mat(2, 2) = -sinTheta;

	return mat;
}

template <typename T>
/*
 Construct the derivative of 3x3 rotation matrix with respect to Y axis
 */
Matrix3x3<T> dyRotationMatrix(float theta)
{
	Matrix3x3<T> mat;
	T cosTheta = cos(theta), sinTheta = sin(theta);

	mat(0, 0) = -sinTheta; mat(0, 2) = cosTheta;
	mat(2, 0) = -cosTheta; mat(2, 2) = -sinTheta;

	return mat;
}


template <typename T>
/*
 Construct the derivative of 3x3 rotation matrix with respect to Z axis
 */
Matrix3x3<T> dzRotationMatrix(float theta)
{
	Matrix3x3<T> mat;
	T cosTheta = cos(theta), sinTheta = sin(theta);

	mat(0, 0) = -sinTheta; mat(0, 1) = -cosTheta;
	mat(1, 0) = cosTheta; mat(1, 1) = -sinTheta;

	return mat;
}


template <typename T>
/* 
 Construct 3x3 rotation matrix from three rotation angles
 */
Matrix3x3<T> rotationMatrix(T thetaX, T thetaY, T thetaZ)
{
	Matrix3x3<T> mat = zRotationMatrix<T>(thetaZ);
	mat *= yRotationMatrix<T>(thetaY);
	mat *= xRotationMatrix<T>(thetaX);

	return mat;
}

template <typename T>
/*
 Construct Jacobians of 3x3 rotation matrix
 */
void jacobian_rotationMatrix(T thetaX, T thetaY, T thetaZ,
							 Matrix3x3<T>& JRotX,
							 Matrix3x3<T>& JRotY,
							 Matrix3x3<T>& JRotZ)
{
	Matrix3x3<T> rotX = xRotationMatrix<T>(thetaX);
	Matrix3x3<T> rotY = yRotationMatrix<T>(thetaY);
	Matrix3x3<T> rotZ = zRotationMatrix<T>(thetaZ);

	JRotX = rotZ * rotY * dxRotationMatrix<T>(thetaX);
	JRotY = rotZ * dyRotationMatrix<T>(thetaY) * rotX;
	JRotZ = dzRotationMatrix<T>(thetaZ) * rotY * rotX;
}

template <typename T>
Matrix3x3<T> outerProduct(const Vector3<T>& u, const Vector3<T>& v) {
	return Matrix3x3<T>(
		u.x * v.x, u.x * v.y, u.x * v.z,
		u.y * v.x, u.y * v.y, u.y * v.z,
		u.z * v.x, u.z * v.y, u.z * v.z
		);
}

template <typename T>
Matrix4x4<T> outerProduct(const Vector4<T>& u, const Vector4<T>& v) {
	return Matrix4x4<T>(
		u.x * v.x, u.x * v.y, u.x * v.z, u.x * v.w,
		u.y * v.x, u.y * v.y, u.y * v.z, u.y * v.w,
		u.z * v.x, u.z * v.y, u.z * v.z, u.z * v.w,
		u.w * v.x, u.w * v.y, u.w * v.z, u.w * v.w
		);
}

template <typename T>
__forceinline void encodeIndex(int idx, T& r, T& g, T& b, T scaler = 255.0) {
	r = ((idx >> 16) & 0xff) / scaler;
	g = ((idx >>  8) & 0xff) / scaler;
	b = ( idx		 & 0xff) / scaler;
}

template <typename T>
__forceinline void decodeIndex(T r, T g, T b, int& idx, T scaler = 255.0) {
	int ri = r * scaler;
	int gi = g * scaler;
	int bi = b * scaler;
	idx = (ri << 16) | (gi << 8) | bi;
}

template <>
__forceinline void encodeIndex(int idx, int& r, int& g, int& b, int scaler) {
	r = (idx >> 16) & 0xff;
	g = (idx >> 8) & 0xff;
	b = idx & 0xff;
}

template <>
__forceinline void decodeIndex(int r, int g, int b, int& idx, int scaler) {
	idx = (r << 16) | (g << 8) | b;
}


//template <typename MT, typename PT>
///*
// Apply rotation and translation to a list of points
// */
//void applyRotationTranslation(Point3<PT>* res, const Point3<PT>* ori, 
//							  int n, const RigidTransformation& RT, float scale)
//{
//	Matrix3x3<MT> R = RT.rotation();
//	Vector3<PT> T = RT.translation();
//
//	for(int vi=0;vi<n;vi++)
//	{
//		Point3<PT> p(ori[vi]);
//		Point3<PT> Xr = R * p;
//		res[vi] = Xr + T;
//	}
//}

template <typename T>
/* Compute the distance from a point to a line */
float lineToPointDistance(const Point3<T>& lp1, const Point3<T>& lp2,
						  const Point3<T>& p)
{
	Vector3<T> dir(lp1, lp2);
	Vector3<T> pn(p, lp1);

	Vector3<T> k = cross(dir, pn);

	// @attention	return the squared distance
	return k.normSquared() / dir.normSquared();
}

template <typename T>
float pointToTriangleDistance(const Point3<T>& p0, 
						  const Point3<T>& p1, const Point3<T>& p2, const Point3<T>& p3,
						  Point3<T>& hit
						  ) 
{
	float dist = 0;

	Vector3<T> d(p0, p1);

	Vector3<T> e12(p1, p2), e13(p1, p3), e21(p2, p1), e23(p2, p3), e31(p3, p1), e32(p3, p2);

	Vector3<T> e12n = e12.normalized();
	Vector3<T> e13n = e13.normalized();
	Vector3<T> e21n = e21.normalized();
	Vector3<T> e23n = e23.normalized();
	Vector3<T> e31n = e31.normalized();
	Vector3<T> e32n = e32.normalized();

	Vector3<T> n(e12, e13);
	n.normalize();

	float dDOTn = d.dot(n);
	float dnorm = d.norm();
	float cosAlpha = dDOTn / dnorm;

	float dn = dDOTn * cosAlpha;
	Vector3<T> p0p0c = -dn * n;
	Point3<T> p0c = p0 + p0p0c;

	Vector3<T> v1 = e21n + e31n, v2 = e12n + e32n, v3 = e13n + e23n;
	Vector3<T> p0cp1(p0c, p1), p0cp2(p0c, p2), p0cp3(p0c, p3);

	Vector3<T> c1(p0cp1, p0cp2), c2(p0cp2, p0cp3), c3(p0cp3, p0cp1);

	T d1 = c1.dot(n);
	T d2 = c2.dot(n);
	T d3 = c3.dot(n);

	Point3<T> x0 = p0c, x1, x2;
	bool inside = true;
	if ( d1 < d2 && d1 < d3 && d1<0 )			//-- outside, p1, p2 side
	{
		inside = false;		
		x1 = p1; x2 = p2;
	}
	else if ( d2 < d1 && d2 < d3 && d2<0 )	//-- outside, p2, p3 side
	{
		inside = false;
		x1 = p2; x2 = p3;
	}
	else if ( d3 < d1 && d3 < d2 && d3<0 )	//-- outside, p3, p1 side
	{
		inside = false;
		x1 = p3; x2 = p1;
	}

	if (inside)
	{
		hit = p0c;
		dist = dn;
	}
	else
	{
		Vector3<T> x1x0(x1, x0), x2x0(x2, x0), x2x1(x2, x1);
		T L_x2x0 = x2x0.norm();
		T t = x1x0.dot(x2x0) / (L_x2x0 * L_x2x0);

		hit = x1 + t * x2x1;

		Vector3<T> line(hit, p0);
		dist = line.norm();
	}

	return dist;
}

template <typename T>
void computeBarycentricCoordinates(const Point3<T>& p, 
								   const Point3<T>& q1, const Point3<T>& q2, const Point3<T>& q3,
								   Point3f& bcoords) 
{
	Vector3<T> e23(q2, q3), e21(q2, q1), e31(q3, q1);
	Vector3<T> d2(q2, p), d3(q3, p);
	Vector3<T> oriN(e23, e21);
	Vector3<T> n = oriN.normalized();

	T btn = oriN.dot(n);

	T t1 = (e23.cross(d2)).dot(n) / btn;
	T t2 = (e31.cross(d3)).dot(n) / btn;
	T t3 = 1 - t1 - t2;

	bcoords.x = t1;
	bcoords.y = t2;
	bcoords.z = t3;
}

template <typename T>
bool isInside(const Point2<T>& p, const vector<Point2<T>>& convex) {

	int pos=0, neg=0;
	for(int i=0;i<convex.size();i++) {
		int cur = i;
		int nxt = i+1;
		nxt = nxt>(convex.size()-1)?0:nxt;

		const Point2<T>& pc = convex[cur];
		const Point2<T>& pn = convex[nxt];
		// check the sideness of current edge
		Point2<T> e(pn.x-pc.x, pn.y-pc.y);
		Vector2<T> n(e.y, -e.x);
		n.normalize();		

		float dx = p.x - pc.x, dy = p.y - pc.y;
		float d = n.x * dx + n.y * dy;

		// if the point is on an edge, count it as inside
		if( d >= 1e-3 ) pos++;
		else if( d < 1e-3 ) neg++;
		else return true;
	}

	return (pos == 0 || neg == 0);
}

// To find orientation of ordered triplet (p, q, r).
// The function returns following values
// 0 --> p, q and r are colinear
// 1 --> Clockwise
// 2 --> Counterclockwise
template <typename T>
int orientation(const Point2<T>& p, const Point2<T>& q, const Point2<T>& r)
{
	int val = (q.y - p.y) * (r.x - q.x) -
		(q.x - p.x) * (r.y - q.y);

	if (val == 0) return 0;  // colinear
	return (val > 0)? 1: 2; // clock or counterclock wise
}

template <typename T>
vector<Point2<T>> convexHull(const vector<Point2<T>>& inPts) {
#ifdef WIN32
	typedef typename Point2<T> point_t;
#else
    typedef Point2<T> point_t;
#endif

	vector<point_t> pts = inPts;
	int n = pts.size();
	// at least 3 points
	if( n < 3 ) return vector<point_t>();
	
	// Initialize Result
	vector<int> next(n, -1);

	// find the left most point
	int l=0;
	for(int i=0;i<n;i++) {
		if( pts[i].x < pts[l].x ) l = i;
	}
	// swap them
	point_t tmp = pts[0];
	pts[0] = pts[l];
	pts[l] = tmp;

	// sort the points by angle to point 0
	std::sort(pts.begin()+1, pts.end(), [&](const point_t& a, const point_t& b){
		float x1 = a.x - pts[0].x, y1 = a.y - pts[0].y;	
		float l1 = x1 * x1 + y1 * y1;
		float x2 = b.x - pts[0].x, y2 = b.y - pts[0].y;
		float l2 = x2 * x2 + y2 * y2;
		return y1/l1 < y2/l2;
	});

	// Start from leftmost point, keep moving counterclockwise
	// until reach the start point again
	int p = 0, q;
	do
	{
		// Search for a point 'q' such that orientation(p, i, q) is
		// counterclockwise for all points 'i'
		q = (p+1)%n;
		for (int i = 0; i < n; i++)
			if (orientation(pts[p], pts[i], pts[q]) == 2)
				q = i;

		next[p] = q;  // Add q to result as a next point of p
		p = q; // Set p as q for next iteration
	} while (p != 0);

	vector<point_t> hull;
	// Collect Result
	for (int i = 0; i < n; i++)
	{
		if (next[i] != -1) hull.push_back(pts[i]);
	}

	return hull;
}
}
