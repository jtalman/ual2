///////////////////////////////////////////////////////////////////////////////
// Quaternion.h
// ============
// Quaternion class represented as sum of a scalar and a vector(rotation axis)
// parts; [s, v] = s + (ix + jy + kz)
//
// When the quaternion is used for 3D rotation, initialize the quaternion with
// the rotation axis (unit vector) and the half rotation angle (radian) because
// of double multiplication, qpq*.
// q = cos(a) + sin(a)(ix + jy + kz)
//
// If there is no rotation, initialize the quaternion (1,0,0,0) because of
// cos(0) = 1, sin(0) = 0
//
//  AUTHOR: Song Ho Ahn (song.ahn@gmail.com)
// CREATED: 2011-12-04
// UPDATED: 2023-03-05
//
// Copyright (C) 2011 Song Ho Ahn
///////////////////////////////////////////////////////////////////////////////

#ifndef MATH_QUATERNION_H
#define MATH_QUATERNION_H

#include "Vectors.h"
#include "Matrices.h"
#include <cmath>
#include <iostream>

struct Quaternion
{
    float s;    // scalar part, s
    float x;    // vector part (x, y, z)
    float y;
    float z;

    // ctors
    Quaternion() : s(0), x(0), y(0), z(0) {}
    Quaternion(float s, float x, float y, float z) : s(s), x(x), y(y), z(z) {}
    Quaternion(const Vector3& axis, float angle);       // rot axis & angle (radian)

    // util functions
    void        set(float s, float x, float y, float z);
    void        set(const Vector3& axis, float angle);  // half angle (radian)
    float       length() const;                         // compute norm of q
    Quaternion& normalize();                            // convert it to unit q
    Quaternion& conjugate();                            // convert it to conjugate
    Quaternion& invert();                               // convert it to inverse q
    Matrix4     getMatrix() const;                      // return as 4x4 matrix
    Vector3     getVector() const;                      // return as Vector3

    // operators
    Quaternion  operator-() const;                      // unary operator (negate)
    Quaternion  operator+(const Quaternion& rhs) const; // addition
    Quaternion  operator-(const Quaternion& rhs) const; // subtraction
    Quaternion  operator*(float a) const;               // scalar multiplication
    Quaternion  operator*(const Quaternion& rhs) const; // multiplication
    Quaternion  operator*(const Vector3& v) const;      // conjugation for rotation
    Quaternion& operator+=(const Quaternion& rhs);      // addition
    Quaternion& operator-=(const Quaternion& rhs);      // subtraction
    Quaternion& operator*=(float a);                    // scalar multiplication
    Quaternion& operator*=(const Quaternion& rhs);      // multiplication
    bool        operator==(const Quaternion& rhs) const;// exact comparison
    bool        operator!=(const Quaternion& rhs) const;// exact comparison

    // friend functions
    friend Quaternion operator*(const float a, const Quaternion q);
    friend std::ostream& operator<<(std::ostream& os, const Quaternion& q);

    // static functions
    // find quaternion for rotating from v1 to v2
    static Quaternion getQuaternion(const Vector3& v1, const Vector3& v2);
    // return quaternion from Euler angles (x, y) or (x, y, z)
    // The rotation order is x->y->z.
    static Quaternion getQuaternion(const Vector2& angles);
    static Quaternion getQuaternion(const Vector3& angles);
};



///////////////////////////////////////////////////////////////////////////////
// inline functions for Quaternion
///////////////////////////////////////////////////////////////////////////////

inline Quaternion::Quaternion(const Vector3& axis, float angle)
{
    // angle is radian
    set(axis, angle);
}



inline void Quaternion::set(float s, float x, float y, float z)
{
    this->s = s;  this->x = x;  this->y = y;  this->z = z;
}



inline void Quaternion::set(const Vector3& axis, float angle)
{
    // use only half angle because of double multiplication, qpq*,
    // q at the front and its conjugate at the back
    Vector3 v = axis;
    v.normalize();                  // convert to unit vector
    float sine = sinf(angle);       // angle is radian
    s = cosf(angle);
    x = v.x * sine;
    y = v.y * sine;
    z = v.z * sine;
}



inline float Quaternion::length() const
{
    return sqrtf(s*s + x*x + y*y + z*z);
}



inline Quaternion& Quaternion::normalize()
{
    const float EPSILON = 0.00001f;
    float d = s*s + x*x + y*y + z*z;
    if(d < EPSILON)
        return *this; // do nothing if it is zero

    float invLength = 1.0f / sqrtf(d);
    s *= invLength;  x *= invLength;  y *= invLength;  z *= invLength;
    return *this;
}



inline Quaternion& Quaternion::conjugate()
{
    x = -x;  y = -y;  z = -z;
    return *this;
}



inline Quaternion& Quaternion::invert()
{
    const float EPSILON = 0.00001f;
    float d = s*s + x*x + y*y + z*z;
    if(d < EPSILON)
        return *this; // do nothing if it is zero

    Quaternion q = *this;
    *this = q.conjugate() * (1.0f / d); // q* / |q||q|
    return *this;
}



inline Matrix4 Quaternion::getMatrix() const
{
    // NOTE: assume the quaternion is unit length
    // compute common values
    float x2  = x + x;
    float y2  = y + y;
    float z2  = z + z;
    float xx2 = x * x2;
    float xy2 = x * y2;
    float xz2 = x * z2;
    float yy2 = y * y2;
    float yz2 = y * z2;
    float zz2 = z * z2;
    float sx2 = s * x2;
    float sy2 = s * y2;
    float sz2 = s * z2;

    // build 4x4 matrix (column-major) and return
    return Matrix4(1 - (yy2 + zz2),  xy2 + sz2,        xz2 - sy2,        0, // column 0
                   xy2 - sz2,        1 - (xx2 + zz2),  yz2 + sx2,        0, // column 1
                   xz2 + sy2,        yz2 - sx2,        1 - (xx2 + yy2),  0, // column 2
                   0,                0,                0,                1);// column 3

    // for non-unit quaternion
    // ss+xx-yy-zz, 2xy+2sz,     2xz-2sy,     0
    // 2xy-2sz,     ss-xx+yy-zz, 2yz-2sx,     0
    // 2xz+2sy,     2yz+2sx,     ss-xx-yy+zz, 0
    // 0,           0,           0,           1
}



inline Vector3 Quaternion::getVector() const
{
    return Vector3(x, y, z);
}



inline Quaternion Quaternion::operator-() const
{
    return Quaternion(-s, -x, -y, -z);
}



inline Quaternion Quaternion::operator+(const Quaternion& rhs) const
{
    return Quaternion(s + rhs.s, x + rhs.x, y + rhs.y, z + rhs.z);
}



inline Quaternion Quaternion::operator-(const Quaternion& rhs) const
{
    return Quaternion(s - rhs.s, x - rhs.x, y - rhs.y, z - rhs.z);
}



inline Quaternion Quaternion::operator*(float a) const
{
    return Quaternion(a*s, a*x, a*y, a*z);
}



inline Quaternion Quaternion::operator*(const Quaternion& rhs) const
{
    // qq' = [s,v] * [s',v'] = [(ss' - v . v'), v x v' + sv' + s'v]
    //NOTE: quaternion multiplication is not commutative
    Vector3 v1(x, y, z);                            // vector part of q
    Vector3 v2(rhs.x, rhs.y, rhs.z);                // vector part of q'

    Vector3 cross = v1.cross(v2);                   // v x v' (cross product)
    float dot = v1.dot(v2);                         // v . v' (inner product)
    Vector3 v3 = cross + (s * v2) + (rhs.s * v1);   // v x v' + sv' + s'v

    return Quaternion(s * rhs.s - dot, v3.x, v3.y, v3.z);
}



inline Quaternion Quaternion::operator*(const Vector3& v) const
{
    Quaternion q(0, x, y, z);
    return *this * q;
}



inline Quaternion& Quaternion::operator+=(const Quaternion& rhs)
{
    s += rhs.s;  x += rhs.x;  y += rhs.y;  z += rhs.z;
    return *this;
}



inline Quaternion& Quaternion::operator-=(const Quaternion& rhs)
{
    s -= rhs.s;  x -= rhs.x;  y -= rhs.y;  z -= rhs.z;
    return *this;
}



inline Quaternion& Quaternion::operator*=(float a)
{
    s *= a;  x *= a;  y *= a; z *= a;
    return *this;
}



inline Quaternion& Quaternion::operator*=(const Quaternion& rhs)
{
    *this = *this * rhs;    // q = qq'
    return *this;
}



inline bool Quaternion::operator==(const Quaternion& rhs) const
{
    // exact comparison
    return (s == rhs.s) && (x == rhs.x) && (y == rhs.y) && (z == rhs.z);
}



inline bool Quaternion::operator!=(const Quaternion& rhs) const
{
    // exact comparison
    return (s != rhs.s) || (x != rhs.x) || (y != rhs.y) || (z != rhs.z);
}



///////////////////////////////////////////////////////////////////////////////
// static functions
///////////////////////////////////////////////////////////////////////////////

// find quaternion for rotating from v1 to v2
inline Quaternion Quaternion::getQuaternion(const Vector3& v1, const Vector3& v2)
{
    const float EPSILON = 0.001f;
    const float HALF_PI = acos(-1) * 0.5f;

    // if two vectors are equal return the vector with 0 rotation
    if(v1.equal(v2, EPSILON))
    {
        return Quaternion(v1, 0);
    }
    // if two vectors are opposite return a perpendicular vector with 180 angle
    else if(v1.equal(-v2, EPSILON))
    {
        Vector3 v;
        if(v1.x > -EPSILON && v1.x < EPSILON)       // if x ~= 0
            v.set(1, 0, 0);
        else if(v1.y > -EPSILON && v1.y < EPSILON)  // if y ~= 0
            v.set(0, 1, 0);
        else                                        // if z ~= 0
            v.set(0, 0, 1);
        return Quaternion(v, HALF_PI);
    }

    Vector3 u1 = v1;                    // convert to normal vector
    Vector3 u2 = v2;
    u1.normalize();
    u2.normalize();

    Vector3 v = u1.cross(u2);           // compute rotation axis
    float angle = acosf(u1.dot(u2));    // rotation angle
    return Quaternion(v, angle * 0.5f); // half angle
}



// find quaternion from 2D rotation angle (ax, ay)
inline Quaternion Quaternion::getQuaternion(const Vector2& angles)
{
    Quaternion qx = Quaternion(Vector3(1,0,0), angles.x);   // rotate along X
    Quaternion qy = Quaternion(Vector3(0,1,0), angles.y);   // rotate along Y
    return qx * qy; // order: y->x
}



// find quaternion from 3D rotation angle (ax, ay, az)
inline Quaternion Quaternion::getQuaternion(const Vector3& angles)
{
    Quaternion qx = Quaternion(Vector3(1,0,0), angles.x);   // rotate along X
    Quaternion qy = Quaternion(Vector3(0,1,0), angles.y);   // rotate along Y
    Quaternion qz = Quaternion(Vector3(0,0,1), angles.z);   // rotate along Z
    return qx * qy * qz;    // order: z->y->x
}



///////////////////////////////////////////////////////////////////////////////
// friend functions
///////////////////////////////////////////////////////////////////////////////

// a * q (scalar * quat)
inline Quaternion operator*(const float a, const Quaternion& q) {
    return Quaternion(a*q.s, a*q.x, a*q.y, a*q.z);
}



inline std::ostream& operator<<(std::ostream& os, const Quaternion& q) {
    os << "(" << q.s << ", " << q.x << ", " << q.y << ", " << q.z << ")";
    return os;
}

#endif
