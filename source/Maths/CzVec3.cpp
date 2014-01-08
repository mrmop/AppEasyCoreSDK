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

#include "CzVec3.h"
#include "CzMath.h"

CzVec3::CzVec3()
{
	x = 0;
	y = 0;
	z = 0;
}

CzVec3::CzVec3(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

void CzVec3::Set(float n)
{
	x = n;
	y = n;
	z = n;
}

void CzVec3::Add(CzVec3 const &v)
{
	x += v.x;
	y += v.y;
	z += v.z;
}

void CzVec3::Add(float v)
{
	x += v;
	y += v;
	z += v;
}

void CzVec3::Zero()
{
	x = 0;
	y = 0;
	z = 0;
}

void CzVec3::One()
{
	x = 1.0f;
	y = 1.0f;
	z = 1.0f;
}

void CzVec3::Sub(CzVec3 const &v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
}

void CzVec3::Sub(float v)
{
	x -= v;
	y -= v;
	z -= v;
}

void CzVec3::Invert()
{
	x = -x;
	y = -y;
	z = -z;
}

void CzVec3::Mul(CzVec3 const &v)
{
	x *= v.x;
	y *= v.y;
	z *= v.z;
}

void CzVec3::Mul(float v)
{
	x *= v;
	y *= v;
	z *= v;
}

float CzVec3::Dot(CzVec3 const &v) const
{
	return x * v.x + y * v.y + z * v.z;	
}

CzVec3 CzVec3::Cross(CzVec3 const &v) const
{
	CzVec3 ov;
	
	ov.x = y * v.z - z * v.y;
	ov.y = z * v.x - x * v.z;
	ov.z = x * v.y - y * v.x;
	
	return ov;
}

void CzVec3::Normalise()
{
	float d = CzMath::sqrt(x * x + y * y + z * z);
	
	x /= d;
	y /= d;
	z /= d;
}

void CzVec3::Clamp(float max_mag)
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
}

void CzVec3::Clamp(CzVec3 const &clamp)
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
}

float CzVec3::Mag() const
{
	return CzMath::sqrt(x * x + y * y + z * z);
}

float CzVec3::MagSquared() const
{
	return x * x + y * y + z * z;
}

bool CzVec3::operator== (CzVec3 const &v) const
{
	if (x == v.x && y == v.y && z == v.z)
		return true;

	return false;
}

bool CzVec3::operator!= (CzVec3 const &v) const
{
	if (x != v.x || y != v.y || z != v.z)
		return true;

	return false;
}

CzVec3& CzVec3::operator+= (CzVec3 const &v)
{
	x += v.x;
	y += v.y;
	z += v.z;

	return *this;
}

CzVec3 CzVec3::operator+	(CzVec3 const &v) const
{
	return CzVec3(x + v.x, y + v.y, z + v.z);
}

CzVec3& CzVec3::operator-= (CzVec3 const &v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;

	return *this;
}

CzVec3 CzVec3::operator-	(CzVec3 const & v) const
{
	return CzVec3(x - v.x, y - v.y, z - v.z);
}

CzVec3& CzVec3::operator*= (CzVec3 const &v)
{
	x *= v.x;
	y *= v.y;
	z *= v.z;

	return *this;
}

CzVec3 CzVec3::operator*	(CzVec3 const &v) const
{
	return CzVec3(x * v.x, y * v.y, z * v.z);
}

CzVec3& CzVec3::operator*= (float n)
{
	x *= n;
	y *= n;
	z *= n;

	return *this;
}

CzVec3 CzVec3::operator*	(float n) const
{
	return CzVec3(x * n, y * n, z * n);
}

CzVec2 CzVec3::toVec2() const
{
	return CzVec2(x, y);
}

CzSVec2 CzVec3::toSVec2() const
{
	return CzSVec2((short)x, (short)y);
}

float CzVec3::PointDistanceToLine(CzVec3 const &v0, CzVec3 const &v1, CzVec3& out) const
{
	CzVec3 v = *this - v0;
	CzVec3 s = v1 - v0;
	
	float l = s.MagSquared();
	float dp = v.Dot(s) / l;
	
	CzVec3 disp = s * dp;
	
	out.x = v0.x + disp.x;
	out.y = v0.y + disp.y;
	out.z = v0.z + disp.z;		
	
	v.Sub(disp);
	
	return v.Mag();		
}

float CzVec3::PointClosestToLine(CzVec3 const &v0, CzVec3 const &v1, CzVec3& out) const
{
	CzVec3 ab = v1 - v0;
	CzVec3 ca = *this - v0;
	
	float t = ca.Dot(ab) / ab.Dot(ab);
	
	if (t < 0.0f)
		t = 0.0f;
		
	if (t > 1.0f)
		t = 1.0f;

	out.x = v0.x + t * ab.x;
	out.y = v0.y + t * ab.y;
	out.z = v0.z + t * ab.z;
	
	return t;
}

