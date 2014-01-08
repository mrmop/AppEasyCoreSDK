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

#ifndef __CV_SVEC2_H__
#define __CV_SVEC2_H__

class CzVec2;
class CzVec3;

class CzSVec2
{
public:
	short			x, y;

private:
public:
	CzSVec2();
	CzSVec2(float x, float y);
	CzSVec2(short x, short y);
	CzSVec2(int x, int y);

	bool			operator==	(CzSVec2& v);
	bool			operator!=	(CzSVec2& v);

	CzVec2			toVec2();
	CzVec3			toVec3();

};


#endif	// __CV_SVEC2_H__

