#pragma once

#include "vector.hpp"
#include "point.hpp"
#include "matrix.hpp"

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
	Matrix3x3<T> rotX = xRotationMatrix(thetaX);
	Matrix3x3<T> rotY = yRotationMatrix(thetaY);
	Matrix3x3<T> rotZ = zRotationMatrix(thetaZ);

	JRotX = rotZ * rotY * dxRotationMatrix(thetaX);
	JRotY = rotZ * dyRotationMatrix(thetaY) * rotX;
	JRotZ = dzRotationMatrix(thetaZ) * rotY * rotX;
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