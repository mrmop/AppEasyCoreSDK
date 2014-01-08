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

#include "CzModifier.h"
#include "CzScene.h"
#include "CzActor.h"
#include "CzHashes.h"

// 
// 
//
//
// CzModifierManager implementation
//
//
//
//

bool CzModifierManager::addModifier(IzModifier* modifier)
{
#if defined(_DEBUG)
	if (findModifier(modifier->getNameHash()))
	{
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Modifier - Already exists in modifier list - ", modifier->getName().c_str());
		return false;
	}
#endif	//(_DEBUG)
	Modifiers.push_back(modifier);
	modifier->setParent(this);

	return true;
}

bool CzModifierManager::removeModifier(IzModifier* modifier)
{
	for (_Iterator it = Modifiers.begin(); it != Modifiers.end(); ++it)
	{
		if (*it == modifier)
		{
			delete *it;
			Modifiers.erase(it);
			return true;
		}
	}

	return false;
}

IzModifier* CzModifierManager::findModifier(unsigned int name_hash)
{
	for (_Iterator it = Modifiers.begin(); it != Modifiers.end(); ++it)
	{
		if ((*it)->getNameHash() == name_hash)
			return *it;
	}

	return NULL;
}

IzModifier* CzModifierManager::findModifier(const char* name)
{
	return findModifier(CzString::CalculateHash(name));
}

void CzModifierManager::clearModifiers()
{
	// Remove and delete all images from the manager
	for (_Iterator it = Modifiers.begin(); it != Modifiers.end(); ++it)
		delete *it;

	Modifiers.clear();
}

void CzModifierManager::Execute(IzXomlResource* target, float dt)
{
	if (Modifiers.size() == 0)
		return;

	CzList<IzModifier*>	Removals;

	for (_Iterator it = begin(); it != end(); ++it)
	{
		if ((*it)->isActive())
		{
			if (!(*it)->isInitialised())
			{
				(*it)->InitModifier(target);
				(*it)->setInitialised(true);
			}
			else
			{
				if (!(*it)->UpdateModifier(target, dt))
				{
					(*it)->ReleaseModifier(target);
					Removals.push_back(*it);
				}
			}
		}
	}

	// Remove deleted modifiers
	for (_Iterator it = Removals.begin(); it != Removals.end(); ++it)
		removeModifier(*it);
}

int CzModifierManager::LoadFromXoml(IzXomlResource* parent, bool load_children, CzXmlNode* node)
{
	if (parent->getClassTypeHash() != CzHashes::Actor_Hash && parent->getClassTypeHash() != CzHashes::Scene_Hash)
	{
		CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, "ModifierManager - Modifiers list needs to be declared inside an actor or scene", DebugInfo.c_str());
		return 0;
	}
	
	if (parent->getClassTypeHash() == CzHashes::Actor_Hash)
	{
		CzActor* actor = (CzActor*)parent;
		actor->setModifiers(this);
	}
	else
	if (parent->getClassTypeHash() == CzHashes::Scene_Hash)
	{
		CzScene* scene = (CzScene*)parent;
		scene->setModifiers(this);
	}

	// Process modifiers list attributes
	for (CzXmlNode::_AttribIterator it = node->attribs_begin(); it != node->attribs_end(); it++)
	{
		unsigned int name_hash = (*it)->getName().getHash();

		if (name_hash == CzHashes::Name_Hash)
			setName((*it)->getValue().c_str());
	}

	// Prrocess modifiers
	for (CzXmlNode::_Iterator it2 = node->begin(); it2 != node->end(); ++it2)
	{
		unsigned int name_hash = (*it2)->GetName().getHash();
		if (name_hash == CzHashes::Modifier_Hash)
		{
			CzString	name;
			CzString	params[4];
			bool		active = true;

			for (CzXmlNode::_AttribIterator it = (*it2)->attribs_begin(); it != (*it2)->attribs_end(); ++it)
			{
				unsigned int attrib_hash = (*it)->getName().getHash();

				if (attrib_hash == CzHashes::Name_Hash)
					name = (*it)->getValue();
				else
				if (attrib_hash == CzHashes::Active_Hash)
					active = (*it)->getValueAsBool();
				else
				if (attrib_hash == CzHashes::Param1_Hash || attrib_hash == CzHashes::P1_Hash)
					params[0] = (*it)->getValue();
				else
				if (attrib_hash == CzHashes::Param2_Hash || attrib_hash == CzHashes::P2_Hash)
					params[1] = (*it)->getValue();
				else
				if (attrib_hash == CzHashes::Param3_Hash || attrib_hash == CzHashes::P3_Hash)
					params[2] = (*it)->getValue();
				else
				if (attrib_hash == CzHashes::Param4_Hash || attrib_hash == CzHashes::P4_Hash)
					params[3] = (*it)->getValue();
			}

			// Find the modifiers creator
			IzModifierCreator* creator = CZ_MODS->findCreator(name.getHash());
			if (creator != NULL)
			{
				IzModifier* mod = creator->CreateInstance();
				if (mod != NULL)
				{
					mod->setName(name.c_str());
					mod->setActive(active);
					for (int t = 0; t < 4; t++)
						mod->setParameter(t, params[t]);
					addModifier(mod);
				}
			}
			else
				CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "ModifierManager - Modifier not found - ", name.c_str(), DebugInfo.c_str());

		}
	}

	return 1;
}

//
// 
// 
//
// CzMods Implementation
// 
// 
// 
//
CDECLARE_SINGLETON(CzMods)

IzModifierCreator* CzMods::findCreator(unsigned int name_hash)
{
	for (_Iterator it = ModifierCreators.begin(); it != ModifierCreators.end(); ++it)
	{
		if ((*it)->getClassNameHash() == name_hash)
			return *it;
	}

	return NULL;
}

IzModifierCreator* CzMods::findCreator(const char* name)
{
	return findCreator(CzString::CalculateHash(name));
}

void CzMods::Init()
{
}

void CzMods::Release()
{
	// Clean up modifier creators
	for (_Iterator it = begin(); it != end(); ++it)
	{
		delete *it;
	}
	ModifierCreators.clear();
}
