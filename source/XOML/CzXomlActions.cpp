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

#include "CzXomlActions.h"
#include "CzXoml.h"
#include "CzHashes.h"
#include "CzUtil.h"
#include "CzString.h"
#include "CzApp.h"
#include "CzScene.h"
#include "CzActor.h"

IzXomlResource* IzXomlAction::getContainer(IzXomlResource* source, CzApp*& app, CzScene*& scene, CzActor*& actor)
{
	app = NULL;
	scene = NULL;
	actor = NULL;

	if (source->getClassTypeHash() == CzHashes::Scene_Hash)
	{
		scene = (CzScene*)source;
		app = scene->getParent();
	}
	else
	if (source->getClassTypeHash() == CzHashes::Actor_Hash)
	{
		actor = (CzActor*)source;
		scene = actor->getScene();
		app = scene->getParent();
	}
	else
	if (source->getClassTypeHash() == CzHashes::Game_Hash)
		app = (CzApp*)source;
	else
		app = CzGlobals::App;	// Last resort

	return (actor != NULL) ? (IzXomlResource*)actor : (IzXomlResource*)scene;
}
