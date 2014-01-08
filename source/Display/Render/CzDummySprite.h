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

#if !defined(_CCZ_DUMMY_SPRITE_H_)
#define _CCZ_DUMMY_SPRITE_H_

#include "CzSprite.h"
//
//
//
// CzDummySprite - A DummySprite is a visual that has no visual component and is usually only used for its attributes
//
//
//
class CzDummySprite : public CzSprite
{
	// Properties
protected:
public:
	// Properties End
public:
	CzDummySprite() : CzSprite()	{ setSpriteType(ST_Dummy);}
	virtual ~CzDummySprite() {}
	
	void	Draw();
};





#endif // _CCZ_DUMMY_SPRITE_H_
