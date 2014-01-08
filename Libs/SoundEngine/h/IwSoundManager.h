/*
 * This file is part of the Marmalade SDK Code Samples.
 *
 * Copyright (C) 2001-2012 Ideaworks3D Ltd.
 * All Rights Reserved.
 *
 * This source code is intended only as a supplement to Ideaworks Labs
 * Development Tools and/or on-line documentation.
 *
 * THIS CODE AND INFORMATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
 * KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
 * PARTICULAR PURPOSE.
 */
#ifndef IW_SOUND_MANAGER_H
#define IW_SOUND_MANAGER_H

// Includes
#ifdef IW_DEBUG_IW_ENGINE_DEPENDENT
    #include "IwEngine.h" // TODO: SORT!!
#endif // !IW_DEBUG_IW_ENGINE_DEPENDENT

#include "IwGeomCore.h"
#include "IwManagedList.h"
#include "IwSingleton.h"
#include "IwSoundPCM.h"
#include "IwSoundADPCM.h"
#include "s3eTypes.h"

// Forward Declarations
class CIwSoundGroup;
class CIwSoundInst;
class CIwSoundParams;

//! Resource types
#define IW_SOUND_RESTYPE_DATA   "CIwSoundData"
#define IW_SOUND_RESTYPE_SPEC   "CIwSoundSpec"
#define IW_SOUND_RESTYPE_GROUP  "CIwSoundGroup"

//! Initialise IwSound module
void    IwSoundInit();

//! Shut down IwSound module
void    IwSoundTerminate();

//! Supported sound data formats
typedef enum IwSoundDataFormat
{
    PCM_8BIT_MONO,              //!< Raw 8-bit per sample single-channel PCM data
    PCM_16BIT_MONO,             //!< Raw 16-bit per sample single-channel PCM data
    ADPCM_MONO                  //!< ADPCM compressed  single-channel data
} IwSoundDataFormat;

//-----------------------------------------------------------------------------
// CIwSoundManager
//-----------------------------------------------------------------------------
//! All state specific to IwAnim
class CIwSoundManager
{
public:
    // Private flags
    enum
    {
        DIRTY_F     = (1 << 0), // data has been modified; need to update all dependents
        ACTIVE_F    = (1 << 1), // sound manager is active
    };

    // Methods
    CIwSoundManager();
    virtual ~CIwSoundManager();

    //! DEPRECATED: max insts set via [SOUND] MaxChannels
    void    SetMaxSoundInsts(uint32 n);

    //! Update all sounds
    void    Update();

    //! Get group identity
    const CIwSoundGroup* GetGroupIdentity() const {return m_GroupIdentity;}

    //! Get params identity
    const CIwSoundParams* GetParamsIdentity() const {return m_ParamsIdentity;}

    //! Get the ID of a free sound channel
    int32 GetFreeChannel(CIwSoundInst* pInst, IwSoundDataFormat format = PCM_16BIT_MONO);

    //! Get a free sound instance
    CIwSoundInst* GetFreeInst();

    //! Set a sound instance as now free
    void SetFreeInst(CIwSoundInst* pInst);

    //! Get the master volume
    inline iwsfixed GetMasterVol() const {return m_MasterVol;}

    //! Get the master pan
    inline iwsfixed GetMasterPan() const {return m_MasterPan;}

    //! Get the master pitch
    inline iwsfixed GetMasterPitch() const {return m_MasterPitch;}

    //! Set the master volume
    inline void SetMasterVol(iwsfixed vol) {m_MasterVol = vol; m_Flags |= DIRTY_F;}

    //! Set the master pan
    inline void SetMasterPan(iwsfixed pan) {m_MasterPan = pan; m_Flags |= DIRTY_F;}

    //! Set the master pitch
    inline void SetMasterPitch(iwsfixed pitch) {m_MasterPitch = pitch; m_Flags |= DIRTY_F;}

    //! Set a channel pitch
    void SetPitch(iwsfixed pitch, int channelID);

    //! Get flags
    inline uint16 GetFlags() const {return m_Flags;}

    //! Max simultaneous sound insts
    uint32 GetMaxChannels() const { return m_MaxInsts; }

    //! Set active status. If false, no calls are passed through to underlying sound system.
    inline void SetActive(bool status)
    {
        if (status == true)
            m_Flags |= ACTIVE_F;
        else
            m_Flags &= ~ACTIVE_F;
    }

    //! Get active status
    inline bool IsActive() const
    {
        return (m_Flags & ACTIVE_F) != 0;
    }

    //! Stop all sounds
    void StopAll();

    //! Stop all sounds using the specified SoundSpec (called from CIwSoundSpec::~CIwSoundSpec)
    void StopSoundSpec(CIwSoundSpec* pSpec);

    //! Pause all sounds
    void PauseAll();

    //! Resume all sounds
    void ResumeAll();

#ifdef IW_DEBUG_IW_ENGINE_DEPENDENT
    virtual void DebugRender();
#endif // !IW_DEBUG_IW_ENGINE_DEPENDENT

    //-------------------------------------------------------------------------
    // Accessors
    //-------------------------------------------------------------------------
    //! Get number of *currently active* sound instance
    uint32 GetNumInsts() const { return m_NumActiveInsts; }

    //! Get pointer to a *currently active* sound instance
    CIwSoundInst* GetSoundInst(int32 i) const { return m_SoundInstPtrs[i]; }

private:

    //! Called internally to update information about a sound instance which has finished/been stopped.
    void UpdateCompleted(int i);

    //-------------------------------------------------------------------------
    // Members
    //-------------------------------------------------------------------------
    uint32          m_TimeStamp;        //!< increases each update
    CIwSoundInst*   m_SoundInsts;       //!< allocation of N CIwSoundInst*
    CIwSoundInst**  m_SoundInstPtrs;    //!< allocation of N CIwSoundInst
    uint32          m_MaxInsts;         //!< max number of CIwSoundInsts
    uint32          m_NumActiveInsts;   //!< number of active CIwSoundInsts

    iwsfixed        m_MasterVol;        //!< master volume
    iwsfixed        m_MasterPan;        //!< master pan
    iwsfixed        m_MasterPitch;      //!< master pitch
    uint16          m_Flags;
    CIwSoundGroup*  m_GroupIdentity;    //!< default group
    CIwSoundParams* m_ParamsIdentity;   //!< default params

    //-------------------------------------------------------------------------
    // Static API
    //-------------------------------------------------------------------------
    static CIwChannelPCM8*  s_ChannelsPCM8;        //!< PCM 8-bit channels
    static CIwChannelPCM16* s_ChannelsPCM16;       //!< PCM 16-bit channels
    static CIwChannelADPCM* s_ChannelsADPCM;       //!< ADPCM channels

    // Friends
    friend class CIwSoundGroup;
    friend class CIwSoundSpec;
#ifdef IW_DEBUG
    friend class CIwMenuItemSound;
#endif
};
IW_SINGLETON_EXTERN(SoundManager);

#ifdef IW_DEBUG
//-----------------------------------------------------------------------------
// CIwMenuItemSound
//-----------------------------------------------------------------------------
//! Helper class to spawn debug menu
class CIwMenuItemSound : public CIwMenuItem
{
public:
    CIwMenuItemSound() : CIwMenuItem("", "IwSound", 0, CIwMenuItem::MENU) {};
    void Select();
};
#endif // IW_DEBUG

#endif
