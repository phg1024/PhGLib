#pragma once

// @author	Peihong Guo
// @date	Sep. 30, 2013
// @brief	templated vector class
// @note	The memory layout in c++ makes it possible to reinterpret an array of vector as an a array of float

#include "point.hpp"

namespace PhGUtils {
template <typename T>
class Vector2
{
public:
    enum Coordinate {
        X = 0,
        Y = 1
    };
    typedef T elem_t;
    Vector2(void):x(0), y(0){}
    ~Vector2(void){}

    Vector2(const vector<T>& elem):x(elem[0]), y(elem[1]){}
    Vector2(T x, T y):x(x),y(y){}
    Vector2(const T* elem):x(elem[0]), y(elem[1]){}
    Vector2(const Vector2& p):x(p.x), y(p.y){}

    QVector2D toQVector() const { return QVector2D(x, y); }
    Vector2 fromQVector(const QVector2D& v) { return Vector2(v.x(), v.y()); }

    template <typename PT>
    Vector2(const Point2<PT>& p, const Point2<PT>& q):
    x(q.x - p.x), y(q.y - p.y){}

    Vector2& operator=(const Vector2& p){
        x = p.x; y = p.y;
        return (*this);
    }

    // special constructors
    static Vector2 zero()
    {
        return Vector2();
    }

    static Vector2 one()
    {
        return Vector2(1, 1);
    }

    // comparison operators
    bool operator==(const Vector2& p) const
    {
        return x == p.x && y == p.y;
    }

    // unitary operators
    Vector2 operator+(){ return (*this); }
    Vector2 operator-(){ return Vector2(-x, -y); }

    // arithmetic operators
    Vector2 operator+(const Vector2& p){ return Vector2(x+p.x, y+p.y); }
    Vector2 operator-(const Vector2& p){ return Vector2(x-p.x, y-p.y); }

    template <typename FT>
    Vector2 operator*(FT factor){ return Vector2(x*factor, y*factor); }
    template <typename FT>
    Vector2 operator/(FT factor){ return Vector2(x/factor, y/factor); }

    Vector2& operator+=(const Vector2& p){ x += p.x; y += p.y; return (*this); }
    Vector2& operator-=(const Vector2& p){ x -= p.x; y -= p.y; return (*this); }
    template <typename FT>
    Vector2& operator*=(FT factor){ x *= factor; y *= factor; return (*this); }
    template <typename FT>
    Vector2& operator/=(FT factor){ x /= factor; y /= factor; return (*this); }

    template <typename PT, typename FT>
    friend Vector2<PT> operator*(FT factor, const Vector2<PT>& p);

    // stream operators
    template <typename PT>
    friend ostream& operator<<(ostream& os, const Vector2<PT>& p);
    template <typename PT>
    friend istream& operator>>(istream& is, Vector2& p);

    // vector operations
    T dot(const Vector2& v) const
    {
        return x * v.x + y * v.y;
    }

    T cross(const Vector2& v) const
    {
        return x * v.y - y * v.x;
    }

    void normalize()
    {
      const double eps = 1e-12;
        T n = norm();
        if( n > eps )
        {
            (*this) /= n;
        }
    }

    T norm() const
    {
        return sqrt( x * x + y * y );
    }

    T normSquared() const
    {
        return x * x + y * y;
    }

    T& operator[](Coordinate c) {
        if( c == X ) return x;
        else return y;
    }

    const T& operator[](Coordinate c) const {
        if( c == X ) return x;
        else return y;
    }

    T& operator[](int c) {
        if( c == X ) return x;
        else return y;
    }

    const T& operator[](int c) const {
        if( c == X ) return x;
        else return y;
    }

    string toString() const {
        stringstream ss;

        ss << (*this);

        return ss.str();
    }

    T x, y;
};

template <typename PT, typename FT>
Vector2<PT> operator*(FT factor, const Vector2<PT>& p)
{
    return Vector2<PT>(p.x * factor, p.y * factor);
}

template <typename PT>
ostream& operator<<(ostream& os, const Vector2<PT>& p)
{
    os << '(' << p.x << ", " << p.y << ')';
    return os;
}

template <typename PT>
istream& operator>>(istream& is, Vector2<PT>& p)
{
    is >> p.x >> p.y;
    return is;
}

template <typename T>
T dot(const Vector2<T>& v, const Vector2<T>& u)
{
    return v.x * u.x + v.y * u.y;
}

template <typename T>
T cross(const Vector2<T>& v, const Vector2<T>& u)
{
    return v.x * u.y - v.y * u.x;
}

template <typename T>
class Vector3 : public Vector2<T>
{
public:
    enum Coordinate {
        X = 0,
        Y = 1,
        Z = 2
    };
    typedef T elem_t;
    Vector3(void):Vector2<T>(),z(0){}
    ~Vector3(void){}

    Vector3(const vector<T>& elem):Vector2<T>(elem), z(elem[2]){}
    Vector3(T x, T y, T z):Vector2<T>(x, y), z(z){}
    Vector3(const T* elem):Vector2<T>(elem), z(elem[2]){}
    Vector3(const Vector3& p):Vector2<T>(p.x, p.y), z(p.z){}

    QVector3D toQVector() const { return QVector3D(this->x, this->y, this->z); }
    Vector3 fromQVector(const QVector3D& v) { return Vector3(v.x(), v.y(), v.z()); }

    template <typename PT>
    /* Construct a vector originate from point p and going to point q */
    Vector3(const Point3<PT>& p, const Point3<PT>& q)
    {
        this->x = q.x - p.x;
        this->y = q.y - p.y;
        this->z = q.z - p.z;
    }

    Vector3(const Vector3& v1, const Vector3& v2) {
        this->x = v1.y * v2.z - v1.z * v2.y;
        this->y = v1.z * v2.x - v1.x * v2.z;
        this->z = v1.x * v2.y - v1.y * v2.x;
    }

    Vector3& operator=(const Vector3& p)
    {
        this->x = p.x; this->y = p.y; this->z = p.z;
        return (*this);
    }

    // special constructors
    static Vector3 zero()
    {
        return Vector3();
    }

    static Vector3 one()
    {
        return Vector3(1, 1, 1);
    }

    // comparison operators
    bool operator==(const Vector3<T>& p) const
    {
        return this->x == p.x && this->y == p.y && this->z == p.z;
    }

    // unitary operators
    Vector3 operator+(){ return (*this); }
    Vector3 operator-(){ return Vector3(-this->x, -this->y, -this->z); }

    // arithmetic operators
    Vector3 operator+(const Vector3& p){ return Vector3(this->x+p.x, this->y+p.y, this->z+p.z); }
    Vector3 operator-(const Vector3& p){ return Vector3(this->x-p.x, this->y-p.y, this->z-p.z); }
    template <typename FT>
    Vector3 operator*(FT factor){ return Vector3(this->x*factor, this->y*factor, this->z*factor); }
    template <typename FT>
    Vector3 operator/(FT factor){ return Vector3(this->x/factor, this->y/factor, this->z/factor); }

    Vector3& operator+=(const Vector3& p){ this->x += p.x; this->y += p.y; this->z+=p.z; return (*this); }
    Vector3& operator-=(const Vector3& p){ this->x -= p.x; this->y -= p.y; this->z-=p.z; return (*this); }
    template <typename FT>
    Vector3& operator*=(FT factor){ this->x *= factor; this->y *= factor; this->z *= factor; return (*this); }
    template <typename FT>
    Vector3& operator/=(FT factor){ this->x /= factor; this->y /= factor; this->z /= factor; return (*this); }

    template <typename PT, typename FT>
    friend Vector3<PT> operator*(FT factor, const Vector3<PT>& p);

    // stream operators
    template <typename PT>
    friend ostream& operator<<(ostream& os, const Vector3<PT>& p);
    template <typename PT>
    friend istream& operator>>(istream& is, Vector3& p);

    // vector operations
    T dot(const Vector3& v) const
    {
        return this->x * v.x + this->y * v.y + this->z * v.z;
    }

    Vector3 cross(const Vector3& v) const
    {
        return Vector3(
            this->y * v.z - this->z * v.y,
            this->z * v.x - this->x * v.z,
            this->x * v.y - this->y * v.x
            );
    }

    // normalize the vector
    void normalize()
    {
      const double eps = 1e-12;
        T n = norm();
        if( n > eps )
        {
            (*this) /= n;
        }
    }

    Vector3 normalized() const {
        Vector3 n = (*this);
        n.normalize();
        return n;
    }

    T norm() const
    {
        return sqrt( this->x * this->x + this->y * this->y + this->z * this->z );
    }

    T normSquared() const
    {
        return this->x * this->x + this->y * this->y + this->z * this->z;
    }

    void print(const string& title = "") {
        if( !title.empty() )
            cout << title << " = " << endl;
        cout << '(' << this->x << ", " << this->y << ", " << this->z << ')';
    }

    T& operator[](Coordinate c) {
        if( c == X ) return this->x;
        else if( c == Y ) return this->y;
        else return this->z;
    }

    const T& operator[](Coordinate c) const {
        if( c == X ) return this->x;
        else if( c == Y ) return this->y;
        else return this->z;
    }

    T& operator[](int c) {
        if( c == X ) return this->x;
        else if( c == Y ) return this->y;
        else return this->z;
    }

    const T& operator[](int c) const {
        if( c == X ) return this->x;
        else if( c == Y ) return this->y;
        else return this->z;
    }

    string toString() const {
        stringstream ss;

        ss << (*this);

        return ss.str();
    }

    T z;
};

template <typename PT, typename FT>
Vector3<PT> operator*(FT factor, const Vector3<PT>& p)
{
    return Vector3<PT>(p.x * factor, p.y * factor, p.z * factor);
}

template <typename PT>
ostream& operator<<(ostream& os, const Vector3<PT>& p)
{
    os << '(' << p.x << ", " << p.y << ", " << p.z << ')';
    return os;
}

template <typename PT>
istream& operator>>(istream& is, Vector3<PT>& p)
{
    is >> p.x >> p.y >> p.z;
    return is;
}

template <typename T>
T dot(const Vector3<T>& u, const Vector3<T>& v)
{
    return (u.x * v.x + u.y * v.y + u.z * v.z);
}

template <typename T>
Vector3<T> cross(const Vector3<T>& u, const Vector3<T>& v)
{
    return Vector3<T>(
        u.y * v.z - u.z * v.y,
        u.z * v.x - u.x * v.z,
        u.x * v.y - u.y * v.x
        );
}

template <typename T>
class Vector4 : public Vector3<T>
{
public:
    enum Coordinate {
        X = 0,
        Y = 1,
        Z = 2,
        W = 3
    };
    typedef T elem_t;
    Vector4(void):Vector3<T>(){}
    ~Vector4(void){}

    Vector4(const vector<T>& elem):Vector3<T>(elem), w(elem[3]){}
    Vector4(T x, T y, T z, T w):Vector3<T>(x, y, z), w(w){}
    Vector4(const Vector4& p):Vector3<T>(p.x, p.y, p.z), w(p.w){}

    QVector4D toQVector() const { return QVector4D(this->x, this->y, this->z, this->w); }
    Vector4 fromQVector(const QVector4D& v) { return Vector4(v.x(), v.y(), v.z(), v.w()); }

    Vector4& operator=(const Vector4& p)
    {
        this->x = p.x; this->y = p.y; this->z = p.z; this->w = p.w;
        return (*this);
    }

    // comparison operator
    bool operator==(const Vector4& p) const
    {
        return this->x == p.x && this->y == p.y && this->z == p.z && this->w == p.w;
    }

    // unitary operators
    Vector4 operator+(){ return (*this); }
    Vector4 operator-(){ return Vector4(-this->x, -this->y, -this->z, -this->w); }

    // arithmetic operators
    Vector4 operator+(const Vector4& p){ return Vector4(this->x+p.x, this->y+p.y, this->z+p.z, this->w+p.w); }
    Vector4 operator-(const Vector4& p){ return Vector4(this->x-p.x, this->y-p.y, this->z-p.z, this->w-p.w); }
    template <typename FT>
    Vector4 operator*(FT factor){ return Vector4(this->x*factor, this->y*factor, this->z*factor, this->w*factor); }
    template <typename FT>
    Vector4 operator/(FT factor){ return Vector4(this->x/factor, this->y/factor, this->z/factor, this->w/factor); }

    Vector4& operator+=(const Vector4& p){ this->x += p.x; this->y += p.y; this->z+=p.z; this->w+=p.w; return (*this); }
    Vector4& operator-=(const Vector4& p){ this->x -= p.x; this->y -= p.y; this->z-=p.z; this->w-=p.w; return (*this); }
    template <typename FT>
    Vector4& operator*=(FT factor){ this->x *= factor; this->y *= factor; this->z *= factor; this->w *= factor; return (*this); }
    template <typename FT>
    Vector4& operator/=(FT factor){ this->x /= factor; this->y /= factor; this->z /= factor; this->w /= factor; return (*this); }

    template <typename PT, typename FT>
    friend Vector4<PT> operator*(FT factor, const Vector4<PT>& p);

    // stream operators
    template <typename PT>
    friend ostream& operator<<(ostream& os, const Vector4<PT>& p);
    template <typename PT>
    friend istream& operator>>(istream& is, Vector4& p);

    T& operator[](Coordinate c) {
        if( c == X ) return this->x;
        else if( c == Y ) return this->y;
        else if( c == Z ) return this->z;
        else return this->w;
    }

    const T& operator[](Coordinate c) const {
        if( c == X ) return this->x;
        else if( c == Y ) return this->y;
        else if( c == Z ) return this->z;
        else return this->w;
    }

    T& operator[](int c) {
        if( c == X ) return this->x;
        else if( c == Y ) return this->y;
        else if( c == Z ) return this->z;
        else return this->w;
    }

    const T& operator[](int c) const {
        if( c == X ) return this->x;
        else if( c == Y ) return this->y;
        else if( c == Z ) return this->z;
        else return this->w;
    }

    string toString() const {
        stringstream ss;

        ss << (*this);

        return ss.str();
    }

    T w;
};

template <typename PT, typename FT>
Vector4<PT> operator*(FT factor, const Vector4<PT>& p)
{
    return Vector4<PT>(p.x * factor, p.y * factor, p.z * factor, p.w * factor);
}

template <typename PT>
ostream& operator<<(ostream& os, const Vector4<PT>& p)
{
    os << '(' << p.x << ", " << p.y << ", " << p.z << ", " << p.w << ')';
    return os;
}

template <typename PT>
istream& operator>>(istream& is, Vector4<PT>& p)
{
    is >> p.x >> p.y >> p.z >> p.w;
    return is;
}

typedef Vector2<int> Vector2i;
typedef Vector2<float> Vector2f;
typedef Vector2<double> Vector2d;

typedef Vector3<int> Vector3i;
typedef Vector3<float> Vector3f;
typedef Vector3<double> Vector3d;

typedef Vector4<int> Vector4i;
typedef Vector4<float> Vector4f;
typedef Vector4<double> Vector4d;

}
