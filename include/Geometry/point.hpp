#pragma once

// @author	Peihong Guo
// @date	Sep. 30, 2013
// @brief	templated point class
// @note	The memory layout in c++ makes it possible to reinterpret an array of points as an a array of float

#include "../phgutils.h"

#include <QVector2D>
#include <QVector3D>
#include <QVector4D>

namespace PhGUtils {
template <typename T>
class Vector3;

template <typename T>
class Point2
{
public:
    enum Coordinate {
        X = 0,
        Y = 1
    };
    typedef T elem_t;
    Point2(void):x(0), y(0){}
    ~Point2(void){}

    Point2(const vector<elem_t>& elem):x(elem[0]),y(elem[1]){}
    Point2(elem_t elem[]):x(elem[0]),y(elem[1]){}
    Point2(elem_t x, elem_t y):x(x),y(y){}
    Point2(const Point2& p):x(p.x), y(p.y){}

    QVector2D toQVector() const { return QVector2D(x, y); }
    static Point2 fromQVector(const QVector2D& v) { return Point2(v.x(), v.y()); }

    Point2& operator=(const Point2& p){
        x = p.x; y = p.y;
        return (*this);
    }

    // comparison operators
    bool operator==(const Point2& p) const
    {
        return x == p.x && y == p.y;
    }
    bool operator==(elem_t val) const {
        return x == val && y == val;
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

    string toString() const {
        stringstream ss;
        ss << "(" << x << ", " << y << ")";
        return ss.str();
    }

    elem_t operator[](Coordinate c) const {
        if( c == X ) return x;
        else return y;
    }

    elem_t& operator[](Coordinate c) {
        if( c == X ) return x;
        else return y;
    }

    elem_t operator[](int c) const {
        if( c == X ) return x;
        else return y;
    }

    elem_t& operator[](int c) {
        if( c == X ) return x;
        else return y;
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
    enum Coordinate {
        X = 0,
        Y = 1,
        Z = 2
    };
    typedef T elem_t;
    Point3(void):Point2<T>(),z(0){}
    ~Point3(void){}

    Point3(const vector<T>& elem):Point2<T>(elem), z(elem[2]){}
    Point3(T elem[]):Point2<T>(elem), z(elem[2]){}
    Point3(T x, T y, T z):Point2<T>(x, y), z(z){}
    Point3(const Point3& p):Point2<T>(p.x, p.y), z(p.z){}

    QVector3D toQVector() const { return QVector3D(this->x, this->y, this->z); }
    static Point3 fromQVector(const QVector3D &v) { return Point3(v.x(), v.y(), v.z()); }

    static Point3 zero() { return Point3(); }

    Point3& operator=(const Point3& p)
    {
        this->x = p.x; this->y = p.y; this->z = p.z;
        return (*this);
    }

    // comparison operators
    bool operator==(const Point3& p) const
    {
        return this->x == p.x && this->y == p.y && this->z == p.z;
    }
    bool operator==(elem_t val) const {
        return this->x == val && this->y == val && this->z == val;
    }

    // unitary operators
    Point3 operator+() const { return (*this); }
    Point3 operator-() const { return Point3(-this->x, -this->y, -this->z); }

    // arithmetic operators
    Point3 operator+(const Point3& p) const { return Point3(this->x+p.x, this->y+p.y, this->z+p.z); }
    Point3 operator-(const Point3& p) const { return Point3(this->x-p.x, this->y-p.y, this->z-p.z); }
    template <typename FT>
    Point3 operator*(FT factor) const { return Point3(this->x*factor, this->y*factor, this->z*factor); }
    template <typename FT>
    Point3 operator/(FT factor) const { return Point3(this->x/factor, this->y/factor, this->z/factor); }

    // point plus vector
    template <typename VT>
    Point3 operator+(const Vector3<VT>& v) const { return Point3(this->x+v.x, this->y+v.y, this->z+v.z); }
    // vector plus point
    template <typename PT, typename VT>
    friend Point3<PT> operator+(const Vector3<VT>& p, const Point3<PT>& v);

    Point3& operator+=(const Point3& p){ this->x += p.x; this->y += p.y; this->z+=p.z; return (*this); }
    Point3& operator-=(const Point3& p){ this->x -= p.x; this->y -= p.y; this->z-=p.z; return (*this); }

    template <typename FT>
    Point3& operator*=(FT factor){ this->x *= factor; this->y *= factor; this->z *= factor; return (*this); }
    template <typename FT>
    Point3& operator/=(FT factor){ this->x /= factor; this->y /= factor; this->z /= factor; return (*this); }

    template <typename FT, typename PT>
    friend Point3<PT> operator*(FT factor, const Point3<PT>& p);

    T squaredDistanceTo(const Point3<T>& p) {
        T dx = p.x - this->x;
        T dy = p.y - this->y;
        T dz = p.z - this->z;
        return dx * dx + dy * dy + dz * dz;
    }

    T distanceTo(const Point3<T>& p) {
        T dx = p.x - this->x;
        T dy = p.y - this->y;
        T dz = p.z - this->z;
        return sqrt(dx * dx + dy * dy + dz * dz);
    }

    void print(const string& title)
    {
        cout << (title.empty()?"":title + " = (") << this->x << ", " << this->y << ", " << this->z << ")" << endl;
    }

    string toString() const {
        stringstream ss;
        ss << "(" << this->x << ", " << this->y << ", " << this->z << ")";
        return ss.str();
    }

    // stream operators
    template <typename PT>
    friend ostream& operator<<(ostream& os, const Point3<PT>& p);
    template <typename PT>
    friend istream& operator>>(istream& is, Point3& p);

    elem_t operator[](Coordinate c) const {
        if( c == X ) return this->x;
        else if( c == Y ) return this->y;
        else return this->z;
    }

    elem_t& operator[](Coordinate c) {
        if( c == X ) return this->x;
        else if( c == Y ) return this->y;
        else return this->z;
    }

    elem_t operator[](int c) const {
        if( c == X ) return this->x;
        else if( c == Y ) return this->y;
        else return this->z;
    }

    elem_t& operator[](int c) {
        if( c == X ) return this->x;
        else if( c == Y ) return this->y;
        else return this->z;
    }

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
    enum Coordinate {
        X = 0,
        Y = 1,
        Z = 2,
        W = 3
    };
    typedef T elem_t;
    Point4(void):Point3<T>(){}
    ~Point4(void){}

    Point4(const vector<T>& elem):Point3<T>(elem),w(elem[3]){}
    Point4(T elem[]):Point3<T>(elem),w(elem[3]){}
    Point4(T x, T y, T z, T w):Point3<T>(x, y, z), w(w){}
    Point4(const Point4& p):Point3<T>(p.x, p.y, p.z), w(p.w){}

    QVector4D toQVector() const { return QVector4D(this->x, this->y, this->z, this->w); }
    static Point4 fromQVector(const QVector4D &v) { return Point4(v.x(), v.y(), v.z(), v.w()); }

    Point4& operator=(const Point4& p)
    {
        this->x = p.x; this->y = p.y; this->z = p.z; this->w = p.w;
        return (*this);
    }

    // comparison operator
    bool operator==(const Point4& p) const
    {
        return this->x == p.x && this->y == p.y && this->z == p.z && this->w == p.w;
    }

    bool operator==(elem_t val) const {
        return this->x == val && this->y == val && this->z == val && this->w == val;
    }

    // unitary operators
    Point4 operator+(){ return (*this); }
    Point4 operator-(){ return Point4(-this->x, -this->y, -this->z, -this->w); }

    // arithmetic operators
    Point4 operator+(const Point4& p) const{ return Point4(this->x+p.x, this->y+p.y, this->z+p.z, this->w+p.w); }
    Point4 operator-(const Point4& p) const{ return Point4(this->x-p.x, this->y-p.y, this->z-p.z, this->w-p.w); }

    template <typename FT>
    Point4 operator*(FT factor){ return Point4(this->x*factor, this->y*factor, this->z*factor, this->w*factor); }
    template <typename FT>
    Point4 operator/(FT factor){ return Point4(this->x/factor, this->y/factor, this->z/factor, this->w/factor); }

    Point4& operator+=(const Point4& p){ this->x += p.x; this->y += p.y; this->z+=p.z; this->w+=p.w; return (*this); }
    Point4& operator-=(const Point4& p){ this->x -= p.x; this->y -= p.y; this->z-=p.z; this->w-=p.w; return (*this); }

    template <typename FT>
    Point4& operator*=(FT factor){ this->x *= factor; this->y *= factor; this->z *= factor; this->w *= factor; return (*this); }
    template <typename FT>
    Point4& operator/=(FT factor){ this->x /= factor; this->y /= factor; this->z /= factor; this->w /= factor; return (*this); }

    template <typename PT, typename FT>
    friend Point4<PT> operator*(FT factor, const Point4<PT>& p);

    // stream operators
    template <typename PT>
    friend ostream& operator<<(ostream& os, const Point4<PT>& p);
    template <typename PT>
    friend istream& operator>>(istream& is, Point4& p);

    elem_t operator[](Coordinate c) const {
        if( c == X ) return this->x;
        else if( c == Y ) return this->y;
        else if( c == Z ) return this->z;
        else return this->w;
    }

    elem_t& operator[](Coordinate c) {
        if( c == X ) return this->x;
        else if( c == Y ) return this->y;
        else if( c == Z ) return this->z;
        else return this->w;
    }

    elem_t operator[](int c) const {
        if( c == X ) return this->x;
        else if( c == Y ) return this->y;
        else if( c == Z ) return this->z;
        else return this->w;
    }

    elem_t& operator[](int c) {
        if( c == X ) return this->x;
        else if( c == Y ) return this->y;
        else if( c == Z ) return this->z;
        else return this->w;
    }

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

}
