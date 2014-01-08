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

#include "CzModifierExt.h"
#include "CzModifier.h"
#include "CzScene.h"
#include "CzActor.h"
#include "CzScript.h"
#include "CzHashes.h"

// 
// 
//
// CzModCollisionNotiify implementation
//
//
//

bool CzModCollisionNotify::UpdateModifier(IzXomlResource* target, float dt)
{
	if (target->getClassTypeHash() != CzHashes::Actor_Hash)
		return true;

	CzActor* actor = (CzActor*)target;
	CzBox2dBody* body = actor->getBox2dBody();
	if (body == NULL)
		return false;

	// Check begin contacts
	bool notify = false;
	CzSlotArray<CzBox2dCollidable*>& started = body->getCollisionsStarted();
	for (int t = 0; t < started.getSize(); t++)
	{
		CzBox2dCollidable* collision = started.element_at(t);
		if (collision != NULL)
		{
			CzActor* actor2 = (CzActor*)collision->getUserData();
			if (actor2 != NULL && (actor2->getType() & CollisionMask) == CollisionMask)
			{
				notify = true;
				actor->NotifyCollision(actor2);
			}
		}
	}
	if (notify)
		actor->ProcessEventActions(CzHashes::OnCollisionStart_Hash);

	// Check end contacts
	notify = false;
	CzSlotArray<CzBox2dCollidable*>& ended = body->getCollisionsEnded();
	for (int t = 0; t < ended.getSize(); t++)
	{
		CzBox2dCollidable* collision = ended.element_at(t);
		if (collision != NULL)
		{
			CzActor* actor2 = (CzActor*)collision->getUserData();
			if (actor2 != NULL && (actor2->getType() & CollisionMask) == CollisionMask)
			{
				notify = true;
				break;
			}
		}
	}
	if (notify)
		actor->ProcessEventActions(CzHashes::OnCollisionEnd_Hash);

	return true;
}

// 
// 
//
// CzModScript implementation
//
//
//

bool CzModScript::UpdateModifier(IzXomlResource* target, float dt)
{
	CzActor* actor = NULL;
	CzScene* scene = NULL;
	
	if (target->getClassTypeHash() == CzHashes::Actor_Hash)
		actor = (CzActor*)target;
	else
	if (target->getClassTypeHash() == CzHashes::Scene_Hash)
		scene = (CzScene*)target;

	CzString *p2 = NULL;
	CzString *p3 = NULL;
	CzString *p4 = NULL;

	if (!Params[1].isEmpty())
		p2 = &Params[1];
	if (!Params[2].isEmpty())
		p3 = &Params[2];
	if (!Params[3].isEmpty())
		p4 = &Params[3];

	if (actor != NULL)
		actor->getScene()->getScriptEngine()->CallFunction(actor, &Params[0], p2, p3, p4);
	else
		scene->getScriptEngine()->CallFunction(scene, &Params[0], p2, p3, p4);

	return true;
}




