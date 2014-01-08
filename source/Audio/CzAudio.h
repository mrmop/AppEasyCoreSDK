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

#if !defined(_CZ_AUDIO_H_)
#define _CZ_AUDIO_H_

#include "CzUtil.h"
#include "CzString.h"
#include "CzXoml.h"
#include "IzPlatformAudio.h"

class CzAudio;
class CzScene;
class CzFile;
class CzSoundCollection;

/**
 @addtogroup Audio
 @{
 */

/**
 @def	CZ_AUDIO

 @brief	A short cut macro that calls the global audio manager singleton

 */
 
#define CZ_AUDIO	CzAudio::getInstance()

/**
 @class	CzSound

 @brief	CzSound - A sound effect.

 */

class CzSound : public IzXomlResource
{
public:

	/**
	 @enum	eState
	
	 @brief	Values that sound loaded states
	 */

	enum eState
	{
		State_Invalid,	///< Sound effect is invalid
		State_Loaded,	///< Sound effect jas been loaded
	};

	// Proprties
protected:
	eState				State;				///< State of sound
//	CIwResGroup*		ResourceGroup;		///< Resource group that our image lives in
	CzSoundInst			SoundInstance;		///< Sound instance
public:
	eState				getState() const					{ return State; }
	CzSoundSpec			getSoundSpec()						{ if (Load()) return SoundSpec; return NULL; }
	// Properties end

protected:
	CzSoundData			SoundData;			///< Sound data
	CzSoundSpec			SoundSpec;			///< Sound info
	CzFile*				File;				///< File object (if sound is file based)
public:

	CzSound() : IzXomlResource(), File(NULL), SoundSpec(NULL), SoundData(NULL), /*ResourceGroup(NULL),*/ State(State_Invalid) { setClassType("sound"); }
	virtual ~CzSound();

/*	void			Init(const char* ResourceName, CIwResGroup* resource_group)		// Init an sound from an soundspec located within a resource group
	{
		setName(ResourceName);
		ResourceGroup = resource_group;
	}*/

	bool			Init(void* memory_file, int memory_file_size);	// Init a sound from a memory based file (sound is loaded)
	void			Init(const char* filename);						// Init a sound from a file (sound is not loaded)
	bool			Load(bool blocking = true);						// Force load the sound

	CzSoundInst		Play(float volume = 1.0f, float pitch = 1.0f, float pan = 0, bool looped = false);

	// Implementation of IzXomlResource interface
	int				LoadFromXoml(IzXomlResource* parent, bool load_children, CzXmlNode* node);

	// Internal
	void			FinishLoad();									// Called back when aysnc loading is completed
};

/**
 @class	CzSoundCreator

 @brief	CzSoundCreator - Creates an instance of a sound object.

 */

class CzSoundCreator : public IzXomlClassCreator
{
public:
	CzSoundCreator()
	{
		setClassName("sound");
	}
	IzXomlResource*	CreateInstance(IzXomlResource* parent)	{ return new CzSound(); }
};

/// @}

#endif	// _CAUDIO_H_