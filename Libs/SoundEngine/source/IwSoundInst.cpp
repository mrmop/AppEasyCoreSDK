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

#include "IwSoundInst.h"
#include "IwSound.h"
#include "s3eSound.h"

// Forward Declarations

//-----------------------------------------------------------------------------
// CIwSoundInst
//-----------------------------------------------------------------------------
CIwSoundInst::CIwSoundInst()
{
    IW_CALLSTACK("CIwSoundInst::CIwSoundInst")

    m_Spec  = NULL;
    m_Vol   = IW_GEOM_ONE;
    m_Pan   = 0;
    m_Pitch = IW_GEOM_ONE;
    m_Flags = FREE_F;
    m_EndSampleCB = NULL;
    m_PlayID = 0;
}
//-----------------------------------------------------------------------------
void    CIwSoundInst::Stop()
{
    IW_CALLSTACK("CIwSoundInst::Stop")

    if (IwGetSoundManager()->IsActive() == false)
        return;

    s3eSoundChannelStop(m_ChanID);
    m_Flags |= COMPLETE_F;
}
//-----------------------------------------------------------------------------
void    CIwSoundInst::Pause()
{
    IW_CALLSTACK("CIwSoundInst::Pause")

    if (IwGetSoundManager()->IsActive() == false)
        return;

    s3eSoundChannelPause(m_ChanID);
}
//-----------------------------------------------------------------------------
void    CIwSoundInst::Resume()
{
    IW_CALLSTACK("CIwSoundInst::Resume")

    if (IwGetSoundManager()->IsActive() == false)
        return;

    s3eSoundChannelResume(m_ChanID);
}
//-----------------------------------------------------------------------------
bool    CIwSoundInst::IsPlaying() const
{
    IW_CALLSTACK("CIwSoundInst::IsPlaying")

    if (IwGetSoundManager()->IsActive() == false)
        return false;

    return s3eSoundChannelGetInt(m_ChanID, S3E_CHANNEL_STATUS) == 1 &&
        s3eSoundChannelGetInt(m_ChanID, S3E_CHANNEL_PAUSED) != 1;
}
//-----------------------------------------------------------------------------
void    CIwSoundInst::SetVol(iwsfixed vol)
{
    IW_CALLSTACK("CIwSoundInst::SetVol")

    m_Vol = vol;
    m_Flags |= DIRTY_F;
}
//-----------------------------------------------------------------------------
void    CIwSoundInst::SetPan(iwsfixed pan)
{
    IW_CALLSTACK("CIwSoundInst::SetPan")

    m_Pan = pan;
    m_Flags |= DIRTY_F;
}
//-----------------------------------------------------------------------------
void    CIwSoundInst::SetPitch(iwsfixed pitch)
{
    IW_CALLSTACK("CIwSoundInst::SetPitch")

    m_Pitch = pitch;
    m_Flags |= DIRTY_F;
}
