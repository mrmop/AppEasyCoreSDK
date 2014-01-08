// 
//
// AppEasy SDK - Cross Platform Multi-purpose Game and App Engine
//
// Developed by Matthew Hopwood of Pocketeers Limited - www.pocketeers.co.uk
//
// For updates, tutorials and more details check out www.appeasymobile.com
//
// This code is provided free of charge and without any warranty whatsoever. You must ensure that this whole notice is present in all files and derivatives, so the reader knows its origin.
// If you use this SDK in your product then please ensure that you credit AppEasy's usage appropriately. Please see www.appeasymobile.com for licensing details and support
//
//

#include "CzVec4.h"
#include "CzMath.h"

CzVec4::CzVec4()
{
	x = 0;
	y = 0;
	z = 0;
	w = 0;
}

CzVec4::CzVec4(float x, float y, float z, float w)
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}

void CzVec4::Set(float n)
{
	x = n;
	y = n;
	z = n;
	w = n;
}
void CzVec4::Add(CzVec4 const &v)
{
	x += v.x;
	y += v.y;
	z += v.z;
	w += v.w;
}

void CzVec4::Add(float v)
{
	x += v;
	y += v;
	z += v;
	w += v;
}

void CzVec4::Zero()
{
	x = 0;
	y = 0;
	z = 0;
	w = 0;
}

void CzVec4::One()
{
	x = 1.0f;
	y = 1.0f;
	z = 1.0f;
	w = 1.0f;
}

void CzVec4::Sub(CzVec4 const &v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
	w -= v.w;
}

void CzVec4::Sub(float v)
{
	x -= v;
	y -= v;
	z -= v;
	w -= v;
}

void CzVec4::Invert()
{
	x = -x;
	y = -y;
	z = -z;
	w = -w;
}

void CzVec4::Mul(CzVec4 const &v)
{
	x *= v.x;
	y *= v.y;
	z *= v.z;
	w *= v.w;
}

void CzVec4::Mul(float v)
{
	x *= v;
	y *= v;
	z *= v;
	w *= v;
}

float CzVec4::Dot(CzVec4 const &v) const
{
	return x * v.x + y * v.y + z * v.z + w * v.w;
}

void CzVec4::Normalise()
{
	float d = CzMath::sqrt(x * x + y * y + z * z);
	
	x /= d;
	y /= d;
	z /= d;
}

void CzVec4::Clamp(float max_mag)
{
	float mag = Mag();
	float d = max_mag - mag;

	// Vector is shorter than clamping magnitude so leave unchanged
	if (d >= 0)
		return;

	// Scale the normalised vector to the magnitude to clamp it
	Normalise();
	x *= max_mag;
	y *= max_mag;
	z *= max_mag;
	w *= max_mag;
}

void CzVec4::Clamp(CzVec4& clamp)
{
	if (x < -clamp.x)
		x = -clamp.x;
	else
	if (x > clamp.x)
		x = clamp.x;

	if (y < -clamp.y)
		y = -clamp.y;
	else
	if (y > clamp.y)
		y = clamp.y;

	if (z < -clamp.z)
		z = -clamp.z;
	else
	if (z > clamp.z)
		z = clamp.z;

	if (w < -clamp.w)
		w = -clamp.w;
	else
	if (w > clamp.w)
		w = clamp.w;
}

float CzVec4::Mag() const
{
	return CzMath::sqrt(x * x + y * y + z * z + w * w);
}

float CzVec4::MagSquared() const
{
	return x * x + y * y + z * z + w * w;
}

bool CzVec4::operator== (CzVec4 const& v) const
{
	if (x == v.x && y == v.y && z == v.z && w == v.w)
		return true;

	return false;
}

bool CzVec4::operator!= (CzVec4 const &v) const 
{
	if (x != v.x || y != v.y || z != v.z || w != v.w)
		return true;

	return false;
}

CzVec4& CzVec4::operator+= (CzVec4 const &v)
{
	x += v.x;
	y += v.y;
	z += v.z;
	w += v.w;

	return *this;
}

CzVec4 CzVec4::operator+	(CzVec4 const &v) const
{
	return CzVec4(x + v.x, y + v.y, z + v.z, w + v.w);
}

CzVec4& CzVec4::operator-= (CzVec4 const &v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
	w -= v.w;

	return *this;
}

CzVec4 CzVec4::operator-	(CzVec4 const &v) const
{
	return CzVec4(x - v.x, y - v.y, z - v.z, w - v.w);
}

CzVec4& CzVec4::operator*= (CzVec4 const &v)
{
	x *= v.x;
	y *= v.y;
	z *= v.z;
	w *= v.w;

	return *this;
}

CzVec4 CzVec4::operator*	(CzVec4 const &v) const
{
	return CzVec4(x * v.x, y * v.y, z * v.z, w * v.w);
}

CzVec4& CzVec4::operator*= (float n)
{
	x *= n;
	y *= n;
	z *= n;
	w *= n;

	return *this;
}

CzVec4 CzVec4::operator*	(float n) const
{
	return CzVec4(x * n, y * n, z * n, w * n);
}

CzVec2 CzVec4::toVec2() const
{
	return CzVec2(x, y);
}

CzVec3 CzVec4::toVec3() const
{
	return CzVec3(x, y, z);
}

CzSVec2 CzVec4::toSVec2() const
{
	return CzSVec2((short)x, (short)y);
}

