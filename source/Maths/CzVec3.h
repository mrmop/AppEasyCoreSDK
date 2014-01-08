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

#ifndef __CZ_CVEC3_H__
#define __CZ_CVEC3_H__

#include "CzVec2.h"
#include "CzSVec2.h"

class CzVec3
{
public:
	float	x, y, z;

private:
public:
	CzVec3();
	CzVec3(float x, float y, float z);

	void			Set(float n);
	void			Add(CzVec3 const &v);
	void			Add(float v);
	void			Zero();
	void			One();
	void			Sub(CzVec3 const &v);
	void			Sub(float v);
	void			Invert();
	void			Mul(CzVec3 const &v);
	void			Mul(float v);
	float			Dot(CzVec3 const &v) const;
	CzVec3			Cross(CzVec3 const &v) const;
	void			Normalise();
	void			Clamp(float max_mag);
	void			Clamp(CzVec3 const &clamp);
	float			Mag() const;
	float			MagSquared() const;
	float			PointDistanceToLine(CzVec3 const &v0, CzVec3 const &v1, CzVec3& out) const;
	float			PointClosestToLine(CzVec3 const &v0, CzVec3 const &v1, CzVec3& out) const;

	bool			operator==	(CzVec3 const &v) const;
	bool			operator!=	(CzVec3 const &v) const;
	CzVec3			operator+	(CzVec3 const &v) const;
	CzVec3&			operator+=	(CzVec3 const &v);
	CzVec3			operator-	(CzVec3 const &v) const;
	CzVec3&			operator-=	(CzVec3 const &v);
	CzVec3			operator*	(CzVec3 const &v) const;
	CzVec3&			operator*=	(CzVec3 const &v);
	CzVec3			operator*	(float n) const;
	CzVec3&			operator*=	(float n);
	
	CzVec2			toVec2() const;
	CzSVec2			toSVec2() const;

};


#endif	// __CZ_CVEC3_H__

