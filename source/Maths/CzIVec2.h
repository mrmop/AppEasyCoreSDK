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

#ifndef __CV_IVEC2_H__
#define __CV_IVEC2_H__

#include "CzVec3.h"
#include "CzIVec2.h"
#include "CzSVec2.h"

class CzIVec2
{
public:
	int	x, y;

private:
public:
	CzIVec2();
	CzIVec2(int x, int y);

	void			Set(int n);
	void			Add(CzIVec2 const &v);
	void			Add(int v);
	void			Zero();
	void			One();
	void			Sub(CzIVec2 const &v);
	void			Sub(int v);
	void			Invert();
	void			Mul(CzIVec2 const &v);
	void			Mul(int v);
	int				Dot(CzIVec2 const &v) const;
	void			Clamp(CzIVec2 const & clamp);
	int				Mag() const;
	int				MagSquared() const;

	bool			operator==	(CzIVec2 const &v) const;
	bool			operator!=	(CzIVec2 const &v) const;
	CzIVec2			operator+	(CzIVec2 const &v) const;
	CzIVec2&		operator+=	(CzIVec2 const &v);
	CzIVec2			operator-	(CzIVec2 const &v) const;
	CzIVec2&		operator-=	(CzIVec2 const &v);
	CzIVec2			operator*	(CzIVec2 const &v) const;
	CzIVec2&		operator*=	(CzIVec2 const &v);
	CzIVec2			operator*	(int n) const;
	CzIVec2&		operator*=	(int n);
	
	CzVec2			toVec2() const;
	CzSVec2			toSVec2() const;
	CzVec3			toVec3() const;

};


#endif	// __CV_IVEC2_H__

