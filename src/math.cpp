/*
* EGE (Easy Graphics Engine)
* filename  math.cpp

定义所有数学相关的函数和类
*/

#include "ege_head.h"
#include <math.h>

namespace ege {

void
rotate_point3d_x(VECTOR3D * pt, float r) {
	double sr = sin(r), cr = cos(r);
	VECTOR3D t_pt = *pt;

	t_pt.y = (float)(cr * pt->y - sr * pt->z);
	t_pt.z = (float)(cr * pt->z + sr * pt->y);
	*pt = t_pt;
}

void
rotate_point3d_y(VECTOR3D * pt, float r) {
	double sr = sin(r), cr = cos(r);
	VECTOR3D t_pt = *pt;

	t_pt.z = (float)(cr * pt->z - sr * pt->x);
	t_pt.x = (float)(cr * pt->x + sr * pt->z);
	*pt = t_pt;
}

void
rotate_point3d_z(VECTOR3D * pt, float r) {
	double sr = sin(r), cr = cos(r);
	VECTOR3D t_pt = *pt;

	t_pt.x = (float)(cr * pt->x - sr * pt->y);
	t_pt.y = (float)(cr * pt->y + sr * pt->x);
	*pt = t_pt;
}

VECTOR3D&
VECTOR3D::operator += (const VECTOR3D& _fp) {
	x += _fp.x; y += _fp.y; z += _fp.z;
	return *this;
}

VECTOR3D&
VECTOR3D::operator -= (const VECTOR3D& _fp) {
	x -= _fp.x; y -= _fp.y; z -= _fp.z;
	return *this;
}

VECTOR3D
VECTOR3D::operator + (const VECTOR3D& _fp) const {
	VECTOR3D v = *this;
	v.x += _fp.x; v.y += _fp.y; v.z += _fp.z;
	return v;
}

VECTOR3D
VECTOR3D::operator - (const VECTOR3D& _fp) const {
	VECTOR3D v = *this;
	v.x -= _fp.x; v.y -= _fp.y; v.z -= _fp.z;
	return v;
}

VECTOR3D
VECTOR3D::operator * (float f) const {
	VECTOR3D v = *this;
	v.x *= f; v.y *= f; v.z *= f;
	return v;
}

VECTOR3D&
VECTOR3D::operator *= (float f) {
	x *= f; y *= f; z *= f;
	return *this;
}

float
VECTOR3D::operator * (const VECTOR3D& _fp) const {
	return x * _fp.x + y * _fp.y + z * _fp.z;
}

VECTOR3D
VECTOR3D::operator & (const VECTOR3D& _fp) const {
	float tx = y * _fp.z - _fp.y * z;
	float ty = z * _fp.x - _fp.z * x;
	float tz = x * _fp.y - _fp.x * y;
	return VECTOR3D(tx, ty, tz);
}

VECTOR3D&
VECTOR3D::operator &= (const VECTOR3D& _fp) {
	float tx = y * _fp.z - _fp.y * z;
	float ty = z * _fp.x - _fp.z * x;
	float tz = x * _fp.y - _fp.x * y;
	return *this = VECTOR3D(tx, ty, tz);
}

float
VECTOR3D::GetModule() const {
	return (float)sqrt(x*x + y*y + z*z);
}

VECTOR3D&
VECTOR3D::Rotate(float rad, const VECTOR3D& v) {
	VECTOR3D p = *this, a = v, b;
	float cr = (float)cos(rad), sr = (float)sin(rad);
	a.SetModule(1.0f);
	*this = p * cr;
	b = a * (a * p) * (1.0f - cr);
	*this += b;
	b = (a & p) * sr;
	return *this += b;
}

VECTOR3D&
VECTOR3D::Rotate(const VECTOR3D& _e, const VECTOR3D& _s) {
	VECTOR3D s = _s, e = _e;
	s.SetModule(1.0f); e.SetModule(1.0f);
	VECTOR3D p = *this, a = s & e, b;
	float cr = s * e, sr = a.GetModule();
	if ((double)fabs(sr) < 1e-5) {
		return *this;
	}
	a.SetModule(1.0f);
	*this = p * cr;
	b = a * (a * p) * (1.0f - cr);
	*this += b;
	b = (a & p) * sr;
	return *this += b;
}

float
VECTOR3D::GetAngel(const VECTOR3D& _e, const VECTOR3D& _s) {
	VECTOR3D s = _s, e = _e;
	s.SetModule(1.0f); e.SetModule(1.0f);
	VECTOR3D a = s & e;
	float sr = a.GetModule();
	return (float)asin(sr);
}

#if !(__cplusplus >= 201103L)
int ege_round(float x) {
	return (x > 0.0) ? floor(x + 0.5) : ceil(x - 0.5);
}
#endif

} // namespace ege
