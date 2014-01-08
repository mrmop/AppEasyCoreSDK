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

#include "CzVec2.h"
#include "CzMath.h"

CzVec2::CzVec2()
{
	x = 0;
	y = 0;
}

CzVec2::CzVec2(float x, float y)
{
	this->x = x;
	this->y = y;
}

void CzVec2::Set(float n)
{
	x = n;
	y = n;
}

void CzVec2::Add(CzVec2& v)
{
	x += v.x;
	y += v.y;
}

void CzVec2::Add(float v)
{
	x += v;
	y += v;
}

void CzVec2::Zero()
{
	x = 0;
	y = 0;
}

void CzVec2::One()
{
	x = 1.0f;
	y = 1.0f;
}

void CzVec2::Sub(CzVec2& v)
{
	x -= v.x;
	y -= v.y;
}

void CzVec2::Sub(float v)
{
	x -= v;
	y -= v;
}

void CzVec2::Invert()
{
	x = -x;
	y = -y;
}

void CzVec2::Mul(CzVec2& v)
{
	x *= v.x;
	y *= v.y;
}

void CzVec2::Mul(float v)
{
	x *= v;
	y *= v;
}

float CzVec2::Dot(CzVec2& v) const
{
	return x * v.x + y * v.y;	
}

void CzVec2::Normalise()
{
	float d = CzMath::sqrt(x * x + y * y);
	
	x /= d;
	y /= d;
}

void CzVec2::Clamp(float max_mag)
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
}

void CzVec2::Clamp(CzVec2& clamp)
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
}

float CzVec2::Mag() const
{
	return CzMath::sqrt(x * x + y * y);
}

float CzVec2::MagSquared() const
{
	return x * x + y * y;
}

bool CzVec2::operator== (CzVec2 const &v) const 
{
	if (x == v.x && y == v.y)
		return true;

	return false;
}

bool CzVec2::operator!= (CzVec2 const &v) const 
{
	if (x != v.x || y != v.y)
		return true;

	return false;
}

CzVec2& CzVec2::operator+= (CzVec2 const &v)
{
	x += v.x;
	y += v.y;

	return *this;
}

CzVec2 CzVec2::operator+	(CzVec2 const &v) const
{
	return CzVec2(x + v.x, y + v.y);
}

CzVec2& CzVec2::operator-= (CzVec2 const &v)
{
	x -= v.x;
	y -= v.y;

	return *this;
}

CzVec2 CzVec2::operator-	(CzVec2 const &v)  const
{
	return CzVec2(x - v.x, y - v.y);
}

CzVec2& CzVec2::operator*= (CzVec2 const &v)
{
	x *= v.x;
	y *= v.y;

	return *this;
}

CzVec2 CzVec2::operator*	(CzVec2 const &v) const
{
	return CzVec2(x * v.x, y * v.y);
}

CzVec2& CzVec2::operator*= (float n)
{
	x *= n;
	y *= n;

	return *this;
}

CzVec2 CzVec2::operator*	(float n) const
{
	return CzVec2(x * n, y * n);
}

CzSVec2 CzVec2::toSVec2() const
{
	return CzSVec2((short)x, (short)y);
}

CzVec3 CzVec2::toVec3() const
{
	return CzVec3(x, y, 0);
}

float CzVec2::PointDistanceToLine(CzVec2& v0, CzVec2& v1) const
{
	float	vx = (v1.x - v0.x);
	float	vy = (v1.y - v0.y);
	float	wx = (x - v0.x);
	float	wy = (y - v0.y);

	float c1 = (wx * vx) + (wy * vy);
	if (c1 <= 0)
		return CzMath::sqrt(wx * wx + wy * wy);
	
	float c2 = (vx * vx) + (vy * vy);
	if (c2 <= c1)
	{
		wx = (x - v1.x);
		wy = (y - v1.y);
		return CzMath::sqrt(wx * wx + wy * wy);
	}

	float d = c1 / c2;
	float pbx = v0.x + d * vx;
	float pby = v0.y + d * vy;
	
	pbx = x - pbx;
	pby = y - pby;
	
	return CzMath::sqrt(pbx * pbx + pby * pby);
}

float CzVec2::PointClosestToLine(CzVec2 const &v0, CzVec2 const &v1, CzVec2 &out) const
{
	CzVec2 ab = v1 - v0;
	CzVec2 ca = *this - v0;
	
	float t = ca.Dot(ab) / ab.Dot(ab);
	
	if (t < 0.0f)
		t = 0.0f;
		
	if (t > 1.0f)
		t = 1.0f;

	out.x = v0.x + t * ab.x;
	out.y = v0.y + t * ab.y;
	
	return t;
}
