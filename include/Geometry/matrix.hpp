#pragma once

#include "../phgutils.h"
#include "vector.hpp"
#include "point.hpp"

template <typename T>
class Matrix3x3
{
public:
	typedef T elem_t;
	Matrix3x3(void){
		for(int i=0;i<3;i++)
			for(int j=0;j<3;j++)
				m[i][j] = 0;
	}
	Matrix3x3(const Matrix3x3& mat)
	{
		m[0][0] = mat(0, 0); m[0][1] = mat(0, 1); m[0][2] = mat(0, 2);
		m[1][0] = mat(1, 0); m[1][1] = mat(1, 1); m[1][2] = mat(1, 2);
		m[2][0] = mat(2, 0); m[2][1] = mat(2, 1); m[2][2] = mat(2, 2);
	}

	Matrix3x3(const vector<T>& elem){
		m[0][0] = elem[0]; m[0][1] = elem[1]; m[0][2] = elem[2];
		m[1][0] = elem[3]; m[1][1] = elem[4]; m[1][2] = elem[5];
		m[2][0] = elem[6]; m[2][1] = elem[7]; m[2][2] = elem[8];
	}

	Matrix3x3(T m00, T m01, T m02,
		T m10, T m11, T m12,
		T m20, T m21, T m22)
	{
		m[0][0] = m00; m[0][1] = m01; m[0][2] = m02;
		m[1][0] = m10; m[1][1] = m11; m[1][2] = m12;
		m[2][0] = m20; m[2][1] = m21; m[2][2] = m22;
	}

	~Matrix3x3(void){}

	bool operator==(const Matrix3x3& mat)
	{
		for(int i=0;i<3;i++)
			for(int j=0;j<3;j++)
				if( mat(i,j) != m[i][j] ) return false;

		return true;
	}

	static Matrix3x3 zero()
	{
		return Matrix3x3();
	}

	static Matrix3x3 identity()
	{
		Matrix3x3 mat;
		mat(0, 0) = mat(1, 1) = mat(2, 2) = 1;
		return mat;
	}

	// transpose matrix
	Matrix3x3 transposed() const
	{
		Matrix3x3 mat;
		mat(0, 0) = m[0][0]; mat(0, 1) = m[1][0]; mat(0, 2) = m[2][0];
		mat(1, 0) = m[0][1]; mat(1, 1) = m[1][1]; mat(1, 2) = m[2][1];
		mat(2, 0) = m[0][2]; mat(2, 1) = m[1][2]; mat(2, 2) = m[2][2];
		return mat;
	}

	// inverse matrix
	Matrix3x3 inv() const
	{
		Matrix3x3 mat;
		T dinv = 1.0 / det();

		mat(0, 0) = (m[1][1] * m[2][2] - m[2][1] * m[1][2]) * dinv;
		mat(0, 1) = (m[2][1] * m[0][2] - m[0][1] * m[2][2]) * dinv;
		mat(0, 2) = (m[0][1] * m[1][2] - m[1][1] * m[0][2]) * dinv;

		mat(1, 0) = (m[1][2] * m[2][0] - m[1][0] * m[2][2]) * dinv;
		mat(1, 1) = (m[0][0] * m[2][2] - m[2][0] * m[0][2]) * dinv;
		mat(1, 2) = (m[0][2] * m[1][0] - m[0][0] * m[1][2]) * dinv;

		mat(2, 0) = (m[1][0] * m[2][1] - m[1][1] * m[2][0]) * dinv;
		mat(2, 1) = (m[0][1] * m[2][0] - m[0][0] * m[2][1]) * dinv;
		mat(2, 2) = (m[0][0] * m[1][1] - m[0][1] * m[1][0]) * dinv;

		return mat;
	}

	T det() const
	{
		return m[0][0] * (m[1][1] * m[2][2] - m[1][2] * m[2][1]) 
			- m[0][1] * (m[1][0] * m[2][2] - m[1][2] * m[2][0]) 
			+ m[0][2] * (m[1][0] * m[2][1] - m[1][1] * m[2][0]);
	}

	// arithmetic operations
	Matrix3x3 operator+(const Matrix3x3& mat) const
	{
		Matrix3x3 res((*this));
		res(0, 0) += mat(0, 0); res(0, 1) += mat(0, 1); res(0, 2) += mat(0, 2);
		res(1, 0) += mat(1, 0); res(1, 1) += mat(1, 1); res(1, 2) += mat(1, 2);
		res(2, 0) += mat(2, 0); res(2, 1) += mat(2, 1); res(2, 2) += mat(2, 2);
		return res;
	}

	Matrix3x3 operator-(const Matrix3x3& mat) const
	{
		Matrix3x3 res((*this));
		res(0, 0) -= mat(0, 0); res(0, 1) -= mat(0, 1); res(0, 2) -= mat(0, 2);
		res(1, 0) -= mat(1, 0); res(1, 1) -= mat(1, 1); res(1, 2) -= mat(1, 2);
		res(2, 0) -= mat(2, 0); res(2, 1) -= mat(2, 1); res(2, 2) -= mat(2, 2);
		return res;
	}

	Matrix3x3 operator*(const Matrix3x3& mat) const
	{
		Matrix3x3 res;

		res(0, 0) = m[0][0] * mat(0, 0) + m[0][1] * mat(1, 0) + m[0][2] * mat(2, 0);
		res(0, 1) = m[0][0] * mat(0, 1) + m[0][1] * mat(1, 1) + m[0][2] * mat(2, 1);
		res(0, 2) = m[0][0] * mat(0, 2) + m[0][1] * mat(1, 2) + m[0][2] * mat(2, 2);

		res(1, 0) = m[1][0] * mat(0, 0) + m[1][1] * mat(1, 0) + m[1][2] * mat(2, 0);
		res(1, 1) = m[1][0] * mat(0, 1) + m[1][1] * mat(1, 1) + m[1][2] * mat(2, 1);
		res(1, 2) = m[1][0] * mat(0, 2) + m[1][1] * mat(1, 2) + m[1][2] * mat(2, 2);

		res(2, 0) = m[2][0] * mat(0, 0) + m[2][1] * mat(1, 0) + m[2][2] * mat(2, 0);
		res(2, 1) = m[2][0] * mat(0, 1) + m[2][1] * mat(1, 1) + m[2][2] * mat(2, 1);
		res(2, 2) = m[2][0] * mat(0, 2) + m[2][1] * mat(1, 2) + m[2][2] * mat(2, 2);

		return res;
	}

	Matrix3x3& operator*=(const Matrix3x3& mat)
	{
		(*this) = (*this) * mat;
		return (*this);
	}

	Matrix3x3 operator*(const T& factor) const
	{
		Matrix3x3 res((*this));

		res(0, 0) *= factor; res(0, 1) *= factor; res(0, 2) *= factor;
		res(1, 0) *= factor; res(1, 1) *= factor; res(1, 2) *= factor;
		res(2, 0) *= factor; res(2, 1) *= factor; res(2, 2) *= factor;

		return res;
	}

	Matrix3x3 operator/(const T& factor) const
	{
		Matrix3x3 res((*this));

		res(0, 0) /= factor; res(0, 1) /= factor; res(0, 2) /= factor;
		res(1, 0) /= factor; res(1, 1) /= factor; res(1, 2) /= factor;
		res(2, 0) /= factor; res(2, 1) /= factor; res(2, 2) /= factor;

		return res;
	}

	// matrix vector / point operations
	template <typename VT>
	Vector3<VT> operator*(const Vector3<VT>& v) const
	{
		T x, y, z;
		x = m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z;
		y = m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z;
		z = m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z;
		return Vector3<VT>(x, y, z);
	}

	template <typename PT>
	Point3<PT> operator*(const Point3<PT>& p) const
	{
		T x, y, z;
		x = m[0][0] * p.x + m[0][1] * p.y + m[0][2] * p.z;
		y = m[1][0] * p.x + m[1][1] * p.y + m[1][2] * p.z;
		z = m[2][0] * p.x + m[2][1] * p.y + m[2][2] * p.z;
		return Point3<PT>(x, y, z);
	}

	template <typename MT, typename VT>
	friend Vector3<VT> operator*(const Vector3<VT>& v, const Matrix3x3<MT>& mat);

	template <typename MT, typename PT>
	friend Point3<PT> operator*(const Point3<PT>& p, const Matrix3x3<MT>& mat);


	// accessor
	T& operator()(int i, int j)
	{
		return m[i][j];
	}
	const T& operator()(int i, int j) const
	{
		return m[i][j];
	}

	template <typename T>
	friend ostream& operator<<(ostream& os, const Matrix3x3<T>& m);
	template <typename T>
	friend istream& operator>>(istream& is, Matrix3x3<T>& m);

	void print(const string& title = "")
	{
		if( !title.empty() )
			cout << title << " = " << endl;
		cout << '[' << m[0][0] << ", " << m[0][1] << ", " << m[0][2] << endl;
		cout << ' ' << m[1][0] << ", " << m[1][1] << ", " << m[1][2] << endl;
		cout << ' ' << m[2][0] << ", " << m[2][1] << ", " << m[2][2] << ']' << endl;
	}

private:
	T m[3][3];
};

template <typename T>
ostream& operator<<(ostream& os, const Matrix3x3<T>& m)
{
	os        << m(0, 0) << ' ' << m(0, 1) << ' ' << m(0, 2);
	os << ' ' << m(1, 0) << ' ' << m(1, 1) << ' ' << m(1, 2);
	os << ' ' << m(2, 0) << ' ' << m(2, 1) << ' ' << m(2, 2);
	return os;
}

template <typename T>
istream& operator>>(istream& is, Matrix3x3<T>& m)
{
	for(int i=0;i<3;i++)
		for(int j=0;j<3;j++)
			is >> m(i, j);
	return is;
}

template <typename MT, typename VT>
Vector3<VT> operator*(const Vector3<VT>& v, const Matrix3x3<MT>& m)
{
	T x, y, z;
	x = m(0, 0) * v.x + m(0, 1) * v.y + m(0, 2) * v.z;
	y = m(1, 0) * v.x + m(1, 1) * v.y + m(1, 2) * v.z;
	z = m(2, 0) * v.x + m(2, 1) * v.y + m(2, 2) * v.z;
	return Vector3<VT>(x, y, z);
}

template <typename MT, typename PT>
Point3<PT> operator*(const Point3<PT>& p, const Matrix3x3<MT>& m)
{
	T x, y, z;
	x = m(0, 0) * p.x + m(0, 1) * p.y + m(0, 2) * p.z;
	y = m(1, 0) * p.x + m(1, 1) * p.y + m(1, 2) * p.z;
	z = m(2, 0) * p.x + m(2, 1) * p.y + m(2, 2) * p.z;
	return Point3<PT>(x, y, z);
}

template <typename T>
class Matrix4x4
{
public:
	typedef T elem_t;
	Matrix4x4(void){
		for(int i=0;i<4;i++)
			for(int j=0;j<4;j++)
				m[i][j] = 0;
	}

	Matrix4x4(const Matrix4x4& mat)
	{
		m[0][0] = mat(0, 0); m[0][1] = mat(0, 1); m[0][2] = mat(0, 2); m[0][3] = mat(0, 3);
		m[1][0] = mat(1, 0); m[1][1] = mat(1, 1); m[1][2] = mat(1, 2); m[1][3] = mat(1, 3);
		m[2][0] = mat(2, 0); m[2][1] = mat(2, 1); m[2][2] = mat(2, 2); m[2][3] = mat(2, 3);
		m[3][0] = mat(3, 0); m[3][1] = mat(3, 1); m[3][2] = mat(3, 2); m[3][3] = mat(3, 3);
	}

	Matrix4x4(const vector<T>& elem) {
		m[0][0] = elem[0]; m[0][1] = elem[1]; m[0][2] = elem[2]; m[0][3] = elem[3];
		m[1][0] = elem[4]; m[1][1] = elem[5]; m[1][2] = elem[6]; m[1][3] = elem[7];
		m[2][0] = elem[8]; m[2][1] = elem[9]; m[2][2] = elem[10]; m[2][3] = elem[11];
		m[3][0] = elem[12]; m[3][1] = elem[13]; m[3][2] = elem[14]; m[3][3] = elem[15];
	}

	Matrix4x4(T m00, T m01, T m02, T m03,
		T m10, T m11, T m12, T m13,
		T m20, T m21, T m22, T m23,
		T m30, T m31, T m32, T m33)
	{
		m[0][0] = m00; m[0][1] = m01; m[0][2] = m02; m[0][3] = m03;
		m[1][0] = m10; m[1][1] = m11; m[1][2] = m12; m[1][3] = m13;
		m[2][0] = m20; m[2][1] = m21; m[2][2] = m22; m[2][3] = m23;
		m[3][0] = m30; m[3][1] = m31; m[3][2] = m32; m[3][3] = m33;
	}

	~Matrix4x4(void){}

	static Matrix4x4 zero()
	{
		return Matrix4x4();
	}

	static Matrix4x4 identity()
	{
		Matrix4x4 mat;
		mat(0, 0) = mat(1, 1) = mat(2, 2) = mat(3, 3) = 1;
		return mat;
	}

	// transpose
	Matrix4x4 transposed() const
	{
		Matrix4x4 mat;
		mat(0, 0) = m[0][0]; mat(0, 1) = m[1][0]; mat(0, 2) = m[2][0]; mat(0, 3) = m[3][0];
		mat(1, 0) = m[0][1]; mat(1, 1) = m[1][1]; mat(1, 2) = m[2][1]; mat(1, 3) = m[3][1];
		mat(2, 0) = m[0][2]; mat(2, 1) = m[1][2]; mat(2, 2) = m[2][2]; mat(2, 3) = m[3][2];
		mat(3, 0) = m[0][3]; mat(3, 1) = m[1][3]; mat(3, 2) = m[2][3]; mat(3, 3) = m[3][3];
		return mat;
	}

	// inv
	Matrix4x4 inv() const
	{
		const T* pm = &(m[0][0]);

		T S0 = pm[0] * pm[5] - pm[1] * pm[4];
		T S1 = pm[0] * pm[6] - pm[2] * pm[4];
		T S2 = pm[0] * pm[7] - pm[3] * pm[4];
		T S3 = pm[1] * pm[6] - pm[2] * pm[5];
		T S4 = pm[1] * pm[7] - pm[3] * pm[5];
		T S5 = pm[2] * pm[7] - pm[3] * pm[6];

		T C5 = pm[10] * pm[15] - pm[11] * pm[14];
		T C4 = pm[9] * pm[15] - pm[11] * pm[13];
		T C3 = pm[9] * pm[14] - pm[10] * pm[13];
		T C2 = pm[8] * pm[15] - pm[11] * pm[12];
		T C1 = pm[8] * pm[14] - pm[10] * pm[12];
		T C0 = pm[8] * pm[13] - pm[9] * pm[12];

		// If determinant equals 0, there is no inverse
		T det = S0 * C5 - S1 * C4 + S2 * C3 + S3 * C2 - S4 * C1 + S5 * C0;
		if(fabs(det) <= 1e-8) return Matrix4x4();

		T dinv = 1.0 / det;

		// Compute adjugate matrix
		Matrix4x4 mat(
			pm[5] * C5  - pm[6] * C4  + pm[7] * C3,  -pm[1] * C5 + pm[2] * C4  - pm[3] * C3,
			pm[13] * S5 - pm[14] * S4 + pm[15] * S3, -pm[9] * S5 + pm[10] * S4 - pm[11] * S3,

			-pm[4] * C5  + pm[6] * C2  - pm[7] * C1,   pm[0] * C5 - pm[2] * C2  + pm[3] * C1,
			-pm[12] * S5 + pm[14] * S2 - pm[15] * S1,  pm[8] * S5 - pm[10] * S2 + pm[11] * S1,

			pm[4] * C4  - pm[5] * C2  + pm[7] * C0,  -pm[0] * C4 + pm[1] * C2  - pm[3] * C0,
			pm[12] * S4 - pm[13] * S2 + pm[15] * S0, -pm[8] * S4 + pm[9] * S2  - pm[11] * S0,

			-pm[4] * C3  + pm[5] * C1  - pm[6] * C0,   pm[0] * C3 - pm[1] * C1  + pm[2] * C0,
			-pm[12] * S3 + pm[13] * S1 - pm[14] * S0,  pm[8] * S3 - pm[9] * S1  + pm[10] * S0 
			);

		mat *= dinv;

		return mat;
	}

	// determinant
	T det() const
	{
		Matrix3x3<T> A00(
			m[1][1], m[1][2], m[1][3],
			m[2][1], m[2][2], m[2][3],
			m[3][1], m[3][2], m[3][3]
		);
		Matrix3x3<T> A01(
			m[1][0], m[1][2], m[1][3],
			m[2][0], m[2][2], m[2][3],
			m[3][0], m[3][2], m[3][3]
		);
		Matrix3x3<T> A02(
			m[1][0], m[1][1], m[1][3],
			m[2][0], m[2][1], m[2][3],
			m[3][0], m[3][1], m[3][3]
		);
		Matrix3x3<T> A03(
			m[1][0], m[1][1], m[1][2],
			m[2][0], m[2][1], m[2][2],
			m[3][0], m[3][1], m[3][2]
		);
		return m[0][0] * A00.det() - m[0][1] * A01.det() + m[0][2] * A02.det() - m[0][3] * A03.det();
	}

	// arithmetic
	Matrix4x4 operator+(const Matrix4x4& mat) const
	{
		Matrix4x4 res((*this));
		res(0, 0) += mat(0, 0); res(0, 1) += mat(0, 1); res(0, 2) += mat(0, 2); res(0, 3) += mat(0, 3);
		res(1, 0) += mat(1, 0); res(1, 1) += mat(1, 1); res(1, 2) += mat(1, 2); res(1, 3) += mat(1, 3);
		res(2, 0) += mat(2, 0); res(2, 1) += mat(2, 1); res(2, 2) += mat(2, 2); res(2, 3) += mat(2, 3);
		res(3, 0) += mat(3, 0); res(3, 1) += mat(3, 1); res(3, 2) += mat(3, 2); res(3, 3) += mat(3, 3);
		return res;
	}

	Matrix4x4 operator-(const Matrix4x4& mat) const
	{
		Matrix4x4 res((*this));
		res(0, 0) -= mat(0, 0); res(0, 1) -= mat(0, 1); res(0, 2) -= mat(0, 2); res(0, 3) -= mat(0, 3);
		res(1, 0) -= mat(1, 0); res(1, 1) -= mat(1, 1); res(1, 2) -= mat(1, 2); res(1, 3) -= mat(1, 3);
		res(2, 0) -= mat(2, 0); res(2, 1) -= mat(2, 1); res(2, 2) -= mat(2, 2); res(2, 3) -= mat(2, 3);
		res(3, 0) -= mat(3, 0); res(3, 1) -= mat(3, 1); res(3, 2) -= mat(3, 2); res(3, 3) -= mat(3, 3);
		return res;
	}

	Matrix4x4 operator*(const Matrix4x4& mat) const
	{
		Matrix4x4 res;

		res(0, 0) = m[0][0] * mat(0, 0) + m[0][1] * mat(1, 0) + m[0][2] * mat(2, 0) + m[0][3] * mat(3, 0);
		res(0, 1) = m[0][0] * mat(0, 1) + m[0][1] * mat(1, 1) + m[0][2] * mat(2, 1) + m[0][3] * mat(3, 1);
		res(0, 2) = m[0][0] * mat(0, 2) + m[0][1] * mat(1, 2) + m[0][2] * mat(2, 2) + m[0][3] * mat(3, 2);
		res(0, 3) = m[0][0] * mat(0, 3) + m[0][1] * mat(1, 3) + m[0][2] * mat(2, 3) + m[0][3] * mat(3, 3);

		res(1, 0) = m[1][0] * mat(0, 0) + m[1][1] * mat(1, 0) + m[1][2] * mat(2, 0) + m[1][3] * mat(3, 0);
		res(1, 1) = m[1][0] * mat(0, 1) + m[1][1] * mat(1, 1) + m[1][2] * mat(2, 1) + m[1][3] * mat(3, 1);
		res(1, 2) = m[1][0] * mat(0, 2) + m[1][1] * mat(1, 2) + m[1][2] * mat(2, 2) + m[1][3] * mat(3, 2);
		res(1, 3) = m[1][0] * mat(0, 3) + m[1][1] * mat(1, 3) + m[1][2] * mat(2, 3) + m[1][3] * mat(3, 3);

		res(2, 0) = m[2][0] * mat(0, 0) + m[2][1] * mat(1, 0) + m[2][2] * mat(2, 0) + m[2][3] * mat(3, 0);
		res(2, 1) = m[2][0] * mat(0, 1) + m[2][1] * mat(1, 1) + m[2][2] * mat(2, 1) + m[2][3] * mat(3, 1);
		res(2, 2) = m[2][0] * mat(0, 2) + m[2][1] * mat(1, 2) + m[2][2] * mat(2, 2) + m[2][3] * mat(3, 2);
		res(2, 3) = m[2][0] * mat(0, 3) + m[2][1] * mat(1, 3) + m[2][2] * mat(2, 3) + m[2][3] * mat(3, 3);

		res(3, 0) = m[3][0] * mat(0, 0) + m[3][1] * mat(1, 0) + m[3][2] * mat(2, 0) + m[3][3] * mat(3, 0);
		res(3, 1) = m[3][0] * mat(0, 1) + m[3][1] * mat(1, 1) + m[3][2] * mat(2, 1) + m[3][3] * mat(3, 1);
		res(3, 2) = m[3][0] * mat(0, 2) + m[3][1] * mat(1, 2) + m[3][2] * mat(2, 2) + m[3][3] * mat(3, 2);
		res(3, 3) = m[3][0] * mat(0, 3) + m[3][1] * mat(1, 3) + m[3][2] * mat(2, 3) + m[3][3] * mat(3, 3);

		return res;
	}

	Matrix4x4& operator*=(const Matrix4x4& mat)
	{
		(*this) = (*this) * mat;
		return (*this);
	}

	Matrix4x4 operator*(const T& factor) const
	{
		Matrix4x4 res((*this));

		res(0, 0) *= factor; res(0, 1) *= factor; res(0, 2) *= factor; res(0, 3) *= factor;
		res(1, 0) *= factor; res(1, 1) *= factor; res(1, 2) *= factor; res(1, 3) *= factor;
		res(2, 0) *= factor; res(2, 1) *= factor; res(2, 2) *= factor; res(2, 3) *= factor;
		res(3, 0) *= factor; res(3, 1) *= factor; res(3, 2) *= factor; res(3, 3) *= factor;

		return res;
	}

	Matrix4x4& operator*=(const T& factor) {
		(*this) = (*this) * factor;
		return (*this);
	}

	Matrix4x4 operator/(const T& factor) const
	{
		Matrix4x4 res((*this));

		res(0, 0) /= factor; res(0, 1) /= factor; res(0, 2) /= factor; res(0, 3) /= factor;
		res(1, 0) /= factor; res(1, 1) /= factor; res(1, 2) /= factor; res(1, 3) /= factor;
		res(2, 0) /= factor; res(2, 1) /= factor; res(2, 2) /= factor; res(2, 3) /= factor;
		res(3, 0) /= factor; res(3, 1) /= factor; res(3, 2) /= factor; res(3, 3) /= factor;

		return res;
	}

	// matrix vector / point operations
	template <typename VT>
	Vector4<VT> operator*(const Vector4<VT>& v) const
	{
		T x, y, z, w;
		x = m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z + m[0][3] * v.w;
		y = m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z + m[1][3] * v.w;
		z = m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z + m[2][3] * v.w;
		w = m[3][0] * v.x + m[3][1] * v.y + m[3][2] * v.z + m[3][3] * v.w;
		return Vector4<VT>(x, y, z, w);
	}

	template <typename PT>
	Point4<PT> operator*(const Point4<PT>& p) const
	{
		T x, y, z, w;
		x = m[0][0] * p.x + m[0][1] * p.y + m[0][2] * p.z + m[0][3] * p.w;
		y = m[1][0] * p.x + m[1][1] * p.y + m[1][2] * p.z + m[1][3] * p.w;
		z = m[2][0] * p.x + m[2][1] * p.y + m[2][2] * p.z + m[2][3] * p.w;
		w = m[3][0] * p.x + m[3][1] * p.y + m[3][2] * p.z + m[3][3] * p.w;
		return Point4<PT>(x, y, z, w);
	}

	template <typename MT, typename VT>
	friend Vector4<VT> operator*(const Vector4<VT>& v, const Matrix4x4<MT>& mat);

	template <typename MT, typename PT>
	friend Point4<PT> operator*(const Point4<PT>& p, const Matrix4x4<MT>& mat);

	// accessors
	T& operator()(int i, int j){ return m[i][j]; }
	const T& operator()(int i, int j) const { return m[i][j]; }


	template <typename MT>
	friend ostream& operator<<(ostream& os, const Matrix4x4& mat);

	template <typename MT>
	friend istream& operator>>(istream& is, Matrix4x4& mat);

	void print(const string& title = "") const
	{
		const Matrix4x4& m = (*this);
		if( !title.empty() )
			cout << title << " = " << endl;
		cout << '[' << m(0, 0) << ", " << m(0, 1) << ", " << m(0, 2) << ", " << m(0, 3) << endl;
		cout << ' ' << m(1, 0) << ", " << m(1, 1) << ", " << m(1, 2) << ", " << m(1, 3) << endl;
		cout << ' ' << m(2, 0) << ", " << m(2, 1) << ", " << m(2, 2) << ", " << m(2, 3) << endl;
		cout << ' ' << m(3, 0) << ", " << m(3, 1) << ", " << m(3, 2) << ", " << m(3, 3) << ']' << endl;
	}

	T* data() {
		return &(m[0][0]);
	}

private:
	T m[4][4];
};

template <typename MT, typename VT>
Vector4<VT> operator*(const Vector4<VT>& v, const Matrix4x4<MT>& m)
{
	T x, y, z, w;
	x = m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z + m[0][3] * v.w;
	y = m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z + m[1][3] * v.w;
	z = m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z + m[2][3] * v.w;
	w = m[3][0] * v.x + m[3][1] * v.y + m[3][2] * v.z + m[3][3] * v.w;
	return Vector4<VT>(x, y, z, w);
}

template <typename MT, typename PT>
Point4<PT> operator*(const Point4<PT>& p, const Matrix4x4<MT>& m)
{
	T x, y, z, w;
	x = m[0][0] * p.x + m[0][1] * p.y + m[0][2] * p.z + m[0][3] * p.w;
	y = m[1][0] * p.x + m[1][1] * p.y + m[1][2] * p.z + m[1][3] * p.w;
	z = m[2][0] * p.x + m[2][1] * p.y + m[2][2] * p.z + m[2][3] * p.w;
	w = m[3][0] * p.x + m[3][1] * p.y + m[3][2] * p.z + m[3][3] * p.w;
	return Point4<PT>(x, y, z);
}

template <typename MT>
ostream& operator<<(ostream& os, const Matrix4x4<MT>& m) {
	os        << m(0, 0) << ' ' << m(0, 1) << ' ' << m(0, 2) << m(0, 3);
	os << ' ' << m(1, 0) << ' ' << m(1, 1) << ' ' << m(1, 2) << m(1, 3);
	os << ' ' << m(2, 0) << ' ' << m(2, 1) << ' ' << m(2, 2) << m(2, 3);
	os << ' ' << m(3, 0) << ' ' << m(3, 1) << ' ' << m(3, 2) << m(3, 3);
	return os;
}

template <typename MT>
istream& operator>>(istream& is, Matrix4x4<MT>& m) {
	for(int i=0;i<4;i++)
		for(int j=0;j<4;j++)
			is >> m(i, j);
	return is;
}

typedef Matrix3x3<float> Matrix3x3f;
typedef Matrix3x3<double> Matrix3x3d;

typedef Matrix4x4<float> Matrix4x4f;
typedef Matrix4x4<double> Matrix4x4d;