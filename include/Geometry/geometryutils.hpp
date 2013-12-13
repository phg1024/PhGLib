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

template <typename MT, typename PT>
/*
 Apply rotation and translation to a list of points
 */
void applyRotationTranslation(Point3<PT>* res, const Point3<PT>* ori, 
							  int n, const RigidTransformation& RT, float scale)
{
	Matrix3x3<MT> R = RT.rotation();
	Vector3<PT> T = RT.translation();

	for(int vi=0;vi<n;vi++)
	{
		Point3<PT> p(ori[vi]);
		Point3<PT> Xr = R * p;
		res[vi] = Xr + T;
	}
}

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

}