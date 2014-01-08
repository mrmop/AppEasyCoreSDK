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

#include "CzSVec2.h"
#include "CzVec2.h"
#include "CzVec3.h"
#include "CzMath.h"

CzSVec2::CzSVec2()
{
	x = 0;
	y = 0;
}

CzSVec2::CzSVec2(float x, float y)
{
	this->x = (short)x;
	this->y = (short)y;
}

CzSVec2::CzSVec2(int x, int y)
{
	this->x = x;
	this->y = y;
}

CzSVec2::CzSVec2(short x, short y)
{
	this->x = (short)x;
	this->y = (short)y;
}

CzVec2 CzSVec2::toVec2()
{
	return CzVec2((float)x, (float)y);
}

CzVec3 CzSVec2::toVec3()
{
	return CzVec3((float)x, (float)y, 0);
}

bool CzSVec2::operator== (CzSVec2& v)
{
	if (x == v.x && y == v.y)
		return true;

	return false;
}

bool CzSVec2::operator!= (CzSVec2& v)
{
	if (x != v.x || y != v.y)
		return true;

	return false;
}

