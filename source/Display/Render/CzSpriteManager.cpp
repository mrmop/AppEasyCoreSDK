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

#include "CzSpriteManager.h"

//
//
//
// CzSpriteManager implementation
//
//
//
void CzSpriteManager::addSprite(CzSprite* sprite)
{
	Sprites.push_back(sprite);
	sprite->setManager(this);
}

void CzSpriteManager::removeSprite(CzSprite* sprite, bool delete_sprites)
{
	CzSprite* parent = sprite->getParent();
	if (parent != NULL)
		parent->removeChild(sprite, delete_sprites);
	else
	{
		Sprites.remove(sprite);
		if (delete_sprites && !sprite->isPooled())
			delete sprite;
/*		for (Iterator it = Sprites.begin(); it != Sprites.end(); ++it)
		{
			if (*it == sprite)
			{
				if (delete_sprites && !(*it)->isPooled())
					delete *it;
				Sprites.erase(it);
				break;
			}
		}*/
	}
}

void CzSpriteManager::Init(int max_layers)
{
	// Allocate layers
	Layers = new CzSlotArray<CzSprite*>[max_layers];
	MaxLayers = max_layers;
}

void CzSpriteManager::Draw()
{
	PLATFORM_RENDER->SetClipRect((int)ScreenClipRect.x, (int)ScreenClipRect.y, (int)ScreenClipRect.z, (int)ScreenClipRect.w);

	// Clear layers
	ClearLayers();

	// Update and organise sprites into layers
	for (Iterator it = Sprites.begin(); it != Sprites.end(); ++it)
	{
		if ((*it)->Update())
		{
			int layer = (*it)->getLayer();
			if (layer < 0) layer = 0;
			if (layer >= MaxLayers) layer = MaxLayers - 1;
			(Layers + layer)->add(*it);
		}
	}

	// Draw sprite layers
	CzSlotArray<CzSprite*>* layers = Layers;
	for (int t = 0; t < MaxLayers; t++)
	{
		int last_used = layers->getLastUsedSlot();
		for (int t2 = 0; t2 <= last_used; t2++)
		{
			CzSprite* sprite = layers->element_at(t2);
			if (sprite != NULL)
				sprite->Draw();
		}

		if (Batching)
			PLATFORM_RENDER->BatchDrawPrims();
		layers++;
	}
}

void CzSpriteManager::Release(bool delete_sprites)
{
	if (delete_sprites)
	{
		// Delete all sprites in the sprite manager
		for (Iterator it = Sprites.begin(); it != Sprites.end(); ++it)
		{
			if (!(*it)->isPooled())
				delete *it;
		}
	}

	Sprites.clear();

	// Clean up layers
	SAFE_DELETE_ARRAY(Layers)
}

void CzSpriteManager::DirtyChildTransforms()
{
	// Force all childreen to rebuild their transforms
	for (Iterator it = Sprites.begin(); it != Sprites.end(); ++it)
	{
		(*it)->forceTransformDirty();
	}
}

void CzSpriteManager::ClearLayers()
{
	for (int t = 0; t < MaxLayers; t++)
	{
		(Layers + t)->clear();
	}
}



