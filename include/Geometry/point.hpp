#pragma once

// @author	Peihong Guo
// @date	Sep. 30, 2013
// @brief	templated point class
// @note	The memory layout in c++ makes it possible to reinterpret an array of points as an a array of float

#include "../phgutils.h"

template <typename T>
class Vector3;

template <typename T>
class Point2
{
public:
	typedef T elem_t;
	Point2(void):x(0), y(0){}
	~Point2(void){}

	Point2(const vector<elem_t>& elem):x(elem[0]),y(elem[1]){}
	Point2(elem_t elem[]):x(elem[0]),y(elem[1]){}
	Point2(elem_t x, elem_t y):x(x),y(y){}
	Point2(const Point2& p):x(p.x), y(p.y){}

	Point2& operator=(const Point2& p){
		x = p.x; y = p.y;
		return (*this);
	}

	// comparison operators
	bool operator==(const Point2& p) const
	{
		return x == p.x && y == p.y;
	}

	// unitary operators
	Point2 operator+(){ return (*this); }
	Point2 operator-(){ return Point2(-x, -y); }

	// arithmetic operators
	Point2 operator+(const Point2& p){ return Point2(x+p.x, y+p.y); }
	Point2 operator-(const Point2& p){ return Point2(x-p.x, y-p.y); }
	template <typename FT>
	Point2 operator*(FT factor){ return Point2(x*factor, y*factor); }
	template <typename FT>
	Point2 operator/(FT factor){ return Point2(x/factor, y/factor); }

	Point2& operator+=(const Point2& p){ x += p.x; y += p.y; return (*this); }
	Point2& operator-=(const Point2& p){ x -= p.x; y -= p.y; return (*this); }

	template <typename FT>
	Point2& operator*=(FT factor){ x *= factor; y *= factor; return (*this); }
	template <typename FT>
	Point2& operator/=(FT factor){ x /= factor; y /= factor; return (*this); }

	template <typename PT>
	friend Point2<PT> operator*(PT factor, const Point2<PT>& p);

	T squaredDistanceTo(const Point2<T>& p) {
		T dx = p.x - x;
		T dy = p.y - y;
		return dx * dx + dy * dy;
	}

	T distanceTo(const Point2<T>& p) {
		T dx = p.x - x;
		T dy = p.y - y;
		return sqrt(dx * dx + dy * dy);
	}

	// stream operators
	template <typename PT>
	friend ostream& operator<<(ostream& os, const Point2<PT>& p);
	template <typename PT>
	friend istream& operator>>(istream& is, Point2& p);

	void print(const string& title)
	{
		cout << (title.empty()?"":title + " = (") << x << ", " << y << ")" << endl;
	}

	elem_t x, y;
};

template <typename PT>
Point2<PT> operator*(PT factor, const Point2<PT>& p)
{
	return Point2<PT>(p.x * factor, p.y * factor);
}

template <typename PT>
ostream& operator<<(ostream& os, const Point2<PT>& p)
{
	os << p.x << ' ' << p.y;
	return os;
}

template <typename PT>
istream& operator>>(istream& is, Point2<PT>& p)
{
	is >> p.x >> p.y;
	return is;
}

template <typename T>
class Point3 : public Point2<T>
{
public:
	typedef T elem_t;
	Point3(void):Point2(),z(0){}
	~Point3(void){}

	Point3(const vector<T>& elem):Point2(elem), z(elem[2]){}
	Point3(T elem[]):Point2(elem), z(elem[2]){}
	Point3(T x, T y, T z):Point2(x, y), z(z){}
	Point3(const Point3& p):Point2(p.x, p.y), z(p.z){}

	static Point3 zero() { return Point3(); }

	Point3& operator=(const Point3& p)
	{
		x = p.x; y = p.y; z = p.z;
		return (*this);
	}

	// comparison operators
	bool operator==(const Point2& p) const
	{
		return x == p.x && y == p.y;
	}

	// unitary operators
	Point3 operator+() const { return (*this); }
	Point3 operator-() const { return Point3(-x, -y, -z); }

	// arithmetic operators
	Point3 operator+(const Point3& p) const { return Point3(x+p.x, y+p.y, z+p.z); }
	Point3 operator-(const Point3& p) const { return Point3(x-p.x, y-p.y, z-p.z); }
	template <typename FT>
	Point3 operator*(FT factor) const { return Point3(x*factor, y*factor, z*factor); }
	template <typename FT>
	Point3 operator/(FT factor) const { return Point3(x/factor, y/factor, z/factor); }

	// point plus vector
	template <typename VT>
	Point3 operator+(const Vector3<VT>& v) const { return Point3(x+v.x, y+v.y, z+v.z); }
	// vector plus point
	template <typename PT, typename VT>
	friend Point3<PT> operator+(const Vector3<VT>& p, const Point3<PT>& v);

	Point3& operator+=(const Point3& p){ x += p.x; y += p.y; z+=p.z; return (*this); }
	Point3& operator-=(const Point3& p){ x -= p.x; y -= p.y; z-=p.z; return (*this); }

	template <typename FT>
	Point3& operator*=(FT factor){ x *= factor; y *= factor; z *= factor; return (*this); }
	template <typename FT>
	Point3& operator/=(FT factor){ x /= factor; y /= factor; z /= factor; return (*this); }

	template <typename FT, typename PT>
	friend Point3<PT> operator*(FT factor, const Point3<PT>& p);

	T squaredDistanceTo(const Point3<T>& p) {
		T dx = p.x - x;
		T dy = p.y - y;
		T dz = p.z - z;
		return dx * dx + dy * dy + dz * dz;
	}

	T distanceTo(const Point3<T>& p) {
		T dx = p.x - x;
		T dy = p.y - y;
		T dz = p.z - z;
		return sqrt(dx * dx + dy * dy + dz * dz);
	}

	// stream operators
	template <typename PT>
	friend ostream& operator<<(ostream& os, const Point3<PT>& p);
	template <typename PT>
	friend istream& operator>>(istream& is, Point3& p);

	T z;
};

template <typename FT, typename PT>
Point3<PT> operator*(FT factor, const Point3<PT>& p)
{
	return Point3<PT>(p.x * factor, p.y * factor, p.z * factor);
}

template <typename PT, typename VT>
Point3<PT> operator+(const Vector3<VT>& p, const Point3<PT>& v)
{
	return Point3<PT>(p.x + v.x, p.y + v.y, p.z + v.z);
}

template <typename PT>
ostream& operator<<(ostream& os, const Point3<PT>& p)
{
	os << p.x << ' ' << p.y << ' ' << p.z;
	return os;
}

template <typename PT>
istream& operator>>(istream& is, Point3<PT>& p)
{
	is >> p.x >> p.y >> p.z;
	return is;
}

template <typename T>
class Point4 : public Point3<T>
{
public:
	typedef T elem_t;
	Point4(void):Point3(){}
	~Point4(void){}

	Point4(const vector<T>& elem):Point3(elem),w(elem[3]){}
	Point4(T elem[]):Point3(elem),w(elem[3]){}
	Point4(T x, T y, T z, T w):Point3(x, y, z), w(w){}
	Point4(const Point4& p):Point3(p.x, p.y, p.z), w(p.w){}

	Point4& operator=(const Point4& p)
	{
		x = p.x; y = p.y; z = p.z; w = p.w;
		return (*this);
	}

	// comparison operator
	bool operator==(const Point4& p) const
	{
		return x == p.x && y == p.y && z == p.z && w == p.w;
	}

	// unitary operators
	Point4 operator+(){ return (*this); }
	Point4 operator-(){ return Point4(-x, -y, -z, -w); }

	// arithmetic operators
	Point4 operator+(const Point4& p){ return Point4(x+p.x, y+p.y, z+p.z, w+p.w); }
	Point4 operator-(const Point4& p){ return Point4(x-p.x, y-p.y, z-p.z, w-p.w); }

	template <typename FT>
	Point4 operator*(FT factor){ return Point4(x*factor, y*factor, z*factor, w*factor); }
	template <typename FT>
	Point4 operator/(FT factor){ return Point4(x/factor, y/factor, z/factor, w/factor); }

	Point4& operator+=(const Point4& p){ x += p.x; y += p.y; z+=p.z; w+=p.w; return (*this); }
	Point4& operator-=(const Point4& p){ x -= p.x; y -= p.y; z-=p.z; w-=p.w; return (*this); }

	template <typename FT>
	Point4& operator*=(FT factor){ x *= factor; y *= factor; z *= factor; w *= factor; return (*this); }
	template <typename FT>
	Point4& operator/=(FT factor){ x /= factor; y /= factor; z /= factor; w /= factor; return (*this); }

	template <typename PT, typename FT>
	friend Point4<PT> operator*(FT factor, const Point4<PT>& p);

	// stream operators
	template <typename PT>
	friend ostream& operator<<(ostream& os, const Point4<PT>& p);
	template <typename PT>
	friend istream& operator>>(istream& is, Point4& p);

	T w;
};

template <typename PT, typename FT>
Point4<PT> operator*(FT factor, const Point4<PT>& p)
{
	return Point4<PT>(p.x * factor, p.y * factor, p.z * factor, p.w * factor);
}

template <typename PT>
ostream& operator<<(ostream& os, const Point4<PT>& p)
{
	os << p.x << ' ' << p.y << ' ' << p.z << ' ' << p.w;
	return os;
}

template <typename PT>
istream& operator>>(istream& is, Point4<PT>& p)
{
	is >> p.x >> p.y >> p.z >> p.w;
	return is;
}

typedef Point2<int> Point2i;
typedef Point2<float> Point2f;
typedef Point2<double> Point2d;

typedef Point3<int> Point3i;
typedef Point3<float> Point3f;
typedef Point3<double> Point3d;

typedef Point4<int> Point4i;
typedef Point4<float> Point4f;
typedef Point4<double> Point4d;