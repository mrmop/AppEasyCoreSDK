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

#ifndef __CV_RECT_H__
#define __CV_RECT_H__

class CzRect
{
public:
	float	x, y, w, h;

private:
public:
	CzRect();
	CzRect(float x, float y, float w, float h);

};

class CzIRect
{
public:
	int	x, y, w, h;

private:
public:
	CzIRect();
	CzIRect(int x, int y, int w, int h);

};


#endif	// __CV_RECT_H__

