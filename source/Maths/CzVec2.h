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

#ifndef __CV_VEC2_H__
#define __CV_VEC2_H__

#include "CzVec3.h"
#include "CzSVec2.h"

class CzVec2
{
public:
	float	x, y;

private:
public:
	CzVec2();
	CzVec2(float x, float y);

	void			Set(float n);
	void			Add(CzVec2& v);
	void			Add(float v);
	void			Zero();
	void			One();
	void			Sub(CzVec2& v);
	void			Sub(float v);
	void			Invert();
	void			Mul(CzVec2& v);
	void			Mul(float v);
	float			Dot(CzVec2& v) const;
	void			Normalise();
	void			Clamp(float max_mag);
	void			Clamp(CzVec2& clamp);
	float			Mag() const;
	float			MagSquared() const;
	float			PointDistanceToLine(CzVec2& v0, CzVec2& v1) const;
	float			PointClosestToLine(CzVec2 const &v0, CzVec2 const &v1, CzVec2 &out) const;

	bool			operator==	(CzVec2 const &v) const;
	bool			operator!=	(CzVec2 const &v) const;
	CzVec2			operator+	(CzVec2 const &v) const;
	CzVec2&			operator+=	(CzVec2 const &v);
	CzVec2			operator-	(CzVec2 const &v) const;
	CzVec2&			operator-=	(CzVec2 const &v);
	CzVec2			operator*	(CzVec2 const &v) const;
	CzVec2&			operator*=	(CzVec2 const &v);
	CzVec2			operator*	(float n) const;
	CzVec2&			operator*=	(float n);
	
	CzSVec2			toSVec2() const;
	CzVec3			toVec3() const;

};


#endif	// __CV_VEC2_H__

