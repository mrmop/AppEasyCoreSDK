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
 * An instance of a playing sound.
 */
#ifndef IW_SOUNDINST_H
#define IW_SOUNDINST_H

// Includes
#include "IwManaged.h"
#include "IwGeomCore.h"

// Forward Declarations
class CIwSoundSpec;
struct s3eSoundEndSampleInfo;

//-----------------------------------------------------------------------------
// CIwSoundInst
//-----------------------------------------------------------------------------
//! An instance of a playing sound.
class CIwSoundInst
{
public:
    //! Typedef for sound instance end callback
    typedef void(*IwSoundInstEndSampleCB)(CIwSoundInst* pInst);

    // Private flags
    enum
    {
        DIRTY_F     = (1 << 0),     //!< data has been modified; need to update all dependents
        COMPLETE_F  = (1 << 1),     //!< instance is finished
        FREE_F      = (1 << 2),     //!< instance is free
    };

    CIwSoundInst();

    //-------------------------------------------------------------------------
    // Public API
    //-------------------------------------------------------------------------
    //! Get flags
    inline uint16 GetFlags() const {return m_Flags;}

    //! Get the spec
    inline const CIwSoundSpec* GetSpec() const {return m_Spec;}

    //! Get the channel ID
    inline const uint16 GetChanID() const {return m_ChanID;}

    //! Get volume
    inline iwsfixed GetVol() const {return m_Vol;}

    //! Get pan
    inline iwsfixed GetPan() const {return m_Pan;}

    //! Get pitch
    inline iwsfixed GetPitch() const {return m_Pitch;}

    //! Set Volume
    void SetVol(iwsfixed vol);

    //! Set Pan
    void SetPan(iwsfixed pan);

    //! Set Pitch
    void SetPitch(iwsfixed pitch);

    //! Get count
    inline uint16 GetCount() const {return m_Count;}

    //! Stop this instance
    void Stop();

    //! Pause this instance
    void Pause();

    //! Resume this instance
    void Resume();

    //! Is this instance playing?
    bool IsPlaying() const;

    //! Set callback: end sample
    inline void SetEndSampleCB(IwSoundInstEndSampleCB cb) {m_EndSampleCB = cb;}

    //! Get callback: end sample
    inline IwSoundInstEndSampleCB GetEndSampleCB() const {return m_EndSampleCB;}

    uint16 GetPlayID() { return m_PlayID; }

    //-------------------------------------------------------------------------
    // Private API
    //-------------------------------------------------------------------------

    // Members
private:
    CIwSoundSpec*   m_Spec;     //!< sound spec for instance
    iwsfixed        m_Vol;      //!< vol we were played at (spec * params)
    iwsfixed        m_Pan;      //!< pan we were played at (spec * params)
    iwsfixed        m_Pitch;    //!< pitch we were played at (spec * params * raw-data-recorded-rate)
    uint16          m_Flags;
    uint16          m_ChanID;   //!< ID of s3eSound channel we are playing on
    uint16          m_Count;    //!< counts up from 0 as sound is playing
    uint16          m_PlayID;   //!< number of times this instance has been played - so clients can tell whether their use is still valid
    IwSoundInstEndSampleCB m_EndSampleCB;

    // Friends
    friend class CIwSoundGroup;
    friend class CIwSoundManager;
    friend class CIwSoundSpec;
    friend int _IwSoundSpec_EndSampleCB(s3eSoundEndSampleInfo* esInfo, void* userData);
};

#endif
