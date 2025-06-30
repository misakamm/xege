/*
* EGE (Easy Graphics Engine)
* filename  math.cpp

定义所有数学相关的函数和类
*/

#include "ege_head.h"
#include <math.h>

namespace ege
{

void rotate_point3d_x(VECTOR3D* point, float rad)
{
    double   sr = sin(rad), cr = cos(rad);
    VECTOR3D t_pt = *point;

    t_pt.y = (float)(cr * point->y - sr * point->z);
    t_pt.z = (float)(cr * point->z + sr * point->y);
    *point    = t_pt;
}

void rotate_point3d_y(VECTOR3D* point, float rad)
{
    double   sr = sin(rad), cr = cos(rad);
    VECTOR3D t_pt = *point;

    t_pt.z = (float)(cr * point->z - sr * point->x);
    t_pt.x = (float)(cr * point->x + sr * point->z);
    *point    = t_pt;
}

void rotate_point3d_z(VECTOR3D* point, float rad)
{
    double   sr = sin(rad), cr = cos(rad);
    VECTOR3D t_pt = *point;

    t_pt.x = (float)(cr * point->x - sr * point->y);
    t_pt.y = (float)(cr * point->y + sr * point->x);
    *point    = t_pt;
}

VECTOR3D& VECTOR3D::operator+=(const VECTOR3D& vector)
{
    x += vector.x;
    y += vector.y;
    z += vector.z;
    return *this;
}

VECTOR3D& VECTOR3D::operator-=(const VECTOR3D& vector)
{
    x -= vector.x;
    y -= vector.y;
    z -= vector.z;
    return *this;
}

VECTOR3D VECTOR3D::operator+(const VECTOR3D& vector) const
{
    VECTOR3D v = *this;

    v.x += vector.x;
    v.y += vector.y;
    v.z += vector.z;
    return v;
}

VECTOR3D VECTOR3D::operator-(const VECTOR3D& vector) const
{
    VECTOR3D v = *this;

    v.x -= vector.x;
    v.y -= vector.y;
    v.z -= vector.z;
    return v;
}

VECTOR3D VECTOR3D::operator*(float scale) const
{
    VECTOR3D v = *this;

    v.x *= scale;
    v.y *= scale;
    v.z *= scale;
    return v;
}

VECTOR3D& VECTOR3D::operator*=(float scale)
{
    x *= scale;
    y *= scale;
    z *= scale;
    return *this;
}

float VECTOR3D::operator*(const VECTOR3D& vector) const
{
    return x * vector.x + y * vector.y + z * vector.z;
}

VECTOR3D VECTOR3D::operator&(const VECTOR3D& vector) const
{
    float tx = y * vector.z - vector.y * z;
    float ty = z * vector.x - vector.z * x;
    float tz = x * vector.y - vector.x * y;

    return VECTOR3D(tx, ty, tz);
}

VECTOR3D& VECTOR3D::operator&=(const VECTOR3D& vector)
{
    float tx = y * vector.z - vector.y * z;
    float ty = z * vector.x - vector.z * x;
    float tz = x * vector.y - vector.x * y;

    return *this = VECTOR3D(tx, ty, tz);
}

float VECTOR3D::GetModule() const
{
    return (float)sqrt(x * x + y * y + z * z);
}

VECTOR3D& VECTOR3D::Rotate(float rad, const VECTOR3D& v)
{
    VECTOR3D p = *this, a = v, b;
    float    cr = (float)cos(rad), sr = (float)sin(rad);
    a.SetModule(1.0f);

    *this  = p * cr;
    b      = a * (a * p) * (1.0f - cr);
    *this += b;
    b      = (a & p) * sr;

    return *this += b;
}

VECTOR3D& VECTOR3D::Rotate(const VECTOR3D& _e, const VECTOR3D& _s)
{
    VECTOR3D s = _s, e = _e;
    s.SetModule(1.0f);
    e.SetModule(1.0f);

    VECTOR3D p = *this, a = s & e, b;
    float    cr = s * e, sr = a.GetModule();

    if ((double)fabs(sr) < 1e-5) {
        return *this;
    }

    a.SetModule(1.0f);
    *this  = p * cr;
    b      = a * (a * p) * (1.0f - cr);
    *this += b;
    b      = (a & p) * sr;

    return *this += b;
}

float VECTOR3D::GetAngle(const VECTOR3D& _e, const VECTOR3D& _s)
{
    VECTOR3D s = _s, e = _e;
    s.SetModule(1.0f);
    e.SetModule(1.0f);

    VECTOR3D a  = s & e;
    float    sr = a.GetModule();

    return (float)asin(sr);
}
} // namespace ege
