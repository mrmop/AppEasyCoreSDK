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

#if !defined(_CZ_ACTOR_TILEMAP_H_)
#define _CZ_ACTOR_TILEMAP_H_

#include "CzActorImage.h"

/**
 @addtogroup Actors
 @{
 */

/**
 @class	CzActorTilemap

 @brief	An actor that connects two points or two actors together like a string.

 A Tilemap actor is an image actor that connects either two actors together or an actor to an anchor point. Tilemap actors are useful for creating all sorts of objects such as 
 strings and ropes.

 A Tilemap actor is declared using the ActorTilemap XOML tag. The ActorTilemap example has been provided to show how they work. Lets take a quick look at some example XOML:

 @par XOML Example
 @code
<ActorTilemap Name="Joiner1" Size="100, 20" Brush="Button1Brush" TargetA="Box1" TargetB="Box2" />
 @endcode

 The above XOML creates a Tilemap actor that connects Box1 and Box2 actors together using a visual Tilemap that is 20 units in width and covers 100% of the length of the Tilemap.

 In addition to basic actor properties Tilemap actors have a number of new or changed properties, which include:
 - Size (length, width) - Size defines the width of the Tilemap as well as the length as apercentage of the distance between the two end points of the Tilemap. For example, if the 
 length is set to 100% then the actor will stretch from the centre of target A to the centre of target B. If the length is less than 100% then the actor will fall short of the centre 
 points of targets A and B;
 - TargetA (actor) - Defines the actor to fix the start point of the Tilemap actor
 - TargetB (actor) - Defines the actor to fix the end point of the Tilemap actor
 - OffsetA (x, y) - An amount to offset the connection point from Target A. If TargetA actor is not specified then this will be classed as a static scene position
 - OffsetB (x, y) - An amount to offset the connection point from Target B. If TargetB actor is not specified then this will be classed as a static scene position

 */

class CzActorTilemap : public CzActor
{
public:

protected:
	// Properties
public:
	bool					getProperty(unsigned int property_name, CzXomlProperty& prop);
	bool					setProperty(unsigned int property_name, const CzXomlProperty& data, bool delta);
	bool					setProperty(unsigned int property_name, const CzString& data, bool delta);
	// Properties end

protected:
	bool					UpdateBinding(unsigned int property_name, CzXomlVariable* var);
public:
	CzActorTilemap() : CzActor() {  setActualClassType("tilemap"); }

	bool					Update(float dt);

	// Implementation of IzXomlClass interface
	int						LoadFromXoml(IzXomlResource* parent, bool load_children, CzXmlNode* node);

	bool					UpdateFromAnimation(CzAnimInstance *animation);

	// Internal (used by XOML system to setup and cleanup the XOML class properties system
protected:
	static CzXomlClassDef*	ActorTilemapClassDef;								// XOML class definition

public:
	static void				InitClass();
	static void				ReleaseClass();
};

/// @}


/**
 @class	CzActorTilemapCreator

 @brief	Creates an instance of a CzActorConnctor object.

 Used by the XOML system to instantiate a CzActorTilemap object.

 */

class CzActorTilemapCreator : public IzXomlClassCreator
{
public:
	CzActorTilemapCreator()
	{
		setClassName("tilemap");
	}
	IzXomlResource*	CreateInstance(IzXomlResource* parent)	{ return new CzActorTilemap(); }
};



#endif // _CZ_ACTOR_TILEMAP_H_
