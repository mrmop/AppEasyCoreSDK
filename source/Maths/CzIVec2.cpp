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

#include "CzIVec2.h"
#include "CzMath.h"

CzIVec2::CzIVec2()
{
	x = 0;
	y = 0;
}

CzIVec2::CzIVec2(int x, int y)
{
	this->x = x;
	this->y = y;
}

void CzIVec2::Set(int n)
{
	x = n;
	y = n;
}

void CzIVec2::Add(CzIVec2 const &v)
{
	x += v.x;
	y += v.y;
}

void CzIVec2::Add(int v)
{
	x += v;
	y += v;
}

void CzIVec2::Zero()
{
	x = 0;
	y = 0;
}

void CzIVec2::One()
{
	x = 1;
	y = 1;
}

void CzIVec2::Sub(CzIVec2 const &v)
{
	x -= v.x;
	y -= v.y;
}

void CzIVec2::Sub(int v)
{
	x -= v;
	y -= v;
}

void CzIVec2::Invert()
{
	x = -x;
	y = -y;
}

void CzIVec2::Mul(CzIVec2 const &v)
{
	x *= v.x;
	y *= v.y;
}

void CzIVec2::Mul(int v)
{
	x *= v;
	y *= v;
}

int CzIVec2::Dot(CzIVec2 const &v) const
{
	return x * v.x + y * v.y;	
}

void CzIVec2::Clamp(CzIVec2 const &clamp)
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

int CzIVec2::Mag() const
{
	return (int)CzMath::sqrt((float)(x * x + y * y));
}

int CzIVec2::MagSquared() const
{
	return x * x + y * y;
}

bool CzIVec2::operator== (CzIVec2 const &v) const
{
	if (x == v.x && y == v.y)
		return true;

	return false;
}

bool CzIVec2::operator!= (CzIVec2 const &v) const
{
	if (x != v.x || y != v.y)
		return true;

	return false;
}

CzIVec2& CzIVec2::operator+= (CzIVec2 const &v)
{
	x += v.x;
	y += v.y;

	return *this;
}

CzIVec2 CzIVec2::operator+	(CzIVec2 const &v) const
{
	return CzIVec2(x + v.x, y + v.y);
}

CzIVec2& CzIVec2::operator-= (CzIVec2 const &v)
{
	x -= v.x;
	y -= v.y;

	return *this;
}

CzIVec2 CzIVec2::operator-	(CzIVec2 const &v) const
{
	return CzIVec2(x - v.x, y - v.y);
}

CzIVec2& CzIVec2::operator*= (CzIVec2 const &v)
{
	x *= v.x;
	y *= v.y;

	return *this;
}

CzIVec2 CzIVec2::operator*	(CzIVec2 const &v) const
{
	return CzIVec2(x * v.x, y * v.y);
}

CzIVec2& CzIVec2::operator*= (int n)
{
	x *= n;
	y *= n;

	return *this;
}

CzIVec2 CzIVec2::operator*	(int n) const
{
	return CzIVec2(x * n, y * n);
}

CzVec2 CzIVec2::toVec2() const
{
	return CzVec2((float)x, (float)y);
}

CzSVec2 CzIVec2::toSVec2() const
{
	return CzSVec2((short)x, (short)y);
}

CzVec3 CzIVec2::toVec3() const
{
	return CzVec3((float)x, (float)y, 0);
}

