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

#ifndef __CZ_CVEC4_H__
#define __CZ_CVEC4_H__

#include "CzVec2.h"
#include "CzVec3.h"
#include "CzSVec2.h"

class CzVec4
{
public:
	float	x, y, z, w;

private:
public:
	CzVec4();
	CzVec4(float x, float y, float z, float w);

	void			Set(float n);
	void			Add(CzVec4 const &v);
	void			Add(float v);
	void			Zero();
	void			One();
	void			Sub(CzVec4 const &v);
	void			Sub(float v);
	void			Invert();
	void			Mul(CzVec4 const &v);
	void			Mul(float v);
	float			Dot(CzVec4 const &v) const;
	void			Normalise();
	void			Clamp(float max_mag);
	void			Clamp(CzVec4& clamp);
	float			Mag() const;
	float			MagSquared() const;

	bool			operator==	(CzVec4 const &v) const;
	bool			operator!=	(CzVec4 const &v) const;
	CzVec4			operator+	(CzVec4 const &v) const;
	CzVec4&			operator+=	(CzVec4 const &v);
	CzVec4			operator-	(CzVec4 const &v) const;
	CzVec4&			operator-=	(CzVec4 const &v);
	CzVec4			operator*	(CzVec4 const &v) const;
	CzVec4&			operator*=	(CzVec4 const &v);
	CzVec4			operator*	(float n) const;
	CzVec4&			operator*=	(float n);
	
	CzVec2			toVec2() const;
	CzVec3			toVec3() const;
	CzSVec2			toSVec2() const;

};


#endif	// __CZ_CVEC4_H__

