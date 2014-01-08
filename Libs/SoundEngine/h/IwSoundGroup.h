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
/*
 * Group several sound specs for convenience.
 */
#ifndef IW_SOUNDGROUP_H
#define IW_SOUNDGROUP_H

// Includes
#include "IwGeomCore.h"
#include "IwResource.h"
#include "IwTextParserITX.h"

// Forward Declarations
struct s3eSoundEndSampleInfo;

/**
 * CIwSoundGroup
 */
class CIwSoundGroup : public CIwResource
{
public:
    // Private flags
    enum
    {
        DIRTY_F         = (1 << 0),     // data has been modified; need to update all dependents
        KILL_OLDEST_F   = (1 << 1),     // if exceeding max polyphony, kill oldest instance
    };

    IW_MANAGED_DECLARE(CIwSoundGroup);

    CIwSoundGroup();
    virtual void Serialise();
    virtual bool ParseAttribute(CIwTextParserITX* pParser, const char* pAttrName);
    virtual void ParseClose(CIwTextParserITX* pParser);

    //-------------------------------------------------------------------------
    // Public API
    //-------------------------------------------------------------------------
    //! Get group volume
    inline iwsfixed GetVol() const {return m_Vol;}

    //! Get group pan
    inline iwsfixed GetPan() const {return m_Pan;}

    //! Get group pitch
    inline iwsfixed GetPitch() const {return m_Pitch;}

    //! Set group volume
    inline void SetVol(iwsfixed vol) {m_Vol = vol; m_Flags |= DIRTY_F;}

    //! Set group pan
    inline void SetPan(iwsfixed pan) {m_Pan = pan; m_Flags |= DIRTY_F;}

    //! Set group pitch
    inline void SetPitch(iwsfixed pitch) {m_Pitch = pitch; m_Flags |= DIRTY_F;}

    //! Get flags
    inline uint16 GetFlags() const {return m_Flags;}

    //! Set max polyphony
    inline void SetMaxPolyphony(uint16 m) {m_MaxPolyphony = m;}

    //! Get max polyphony
    inline uint16 GetMaxPolyphony() const {return m_MaxPolyphony;}

    //! Get curr polyphony
    inline uint16 GetCurrPolyphony() const {return m_CurrPolyphony;}

    //! Stop all sounds
    void Stop();

    //! Pause all sounds
    void Pause();

    //! Resume all sounds
    void Resume();

    //-------------------------------------------------------------------------
    // Private API
    //-------------------------------------------------------------------------
    void    KillOldestInst(bool alreadyGotNewInst);

    // Members
private:
    iwsfixed    m_Vol;              //!< volume of group
    iwsfixed    m_Pan;              //!< pan of group
    iwsfixed    m_Pitch;            //!< pitch of group
    uint16      m_MaxPolyphony;     //!< max simultaneous playing sounds
    uint16      m_CurrPolyphony;    //!< curr simultaneous playing sounds
    uint16      m_Flags;
    uint16      pad;

    // Friends
    friend class CIwSoundManager;
    friend class CIwSoundSpec;
    friend int _IwSoundSpec_EndSampleCB(s3eSoundEndSampleInfo* esInfo, void* userData);
};

#endif /* !IW_SOUNDGROUP_H */
