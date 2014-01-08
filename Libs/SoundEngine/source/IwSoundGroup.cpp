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

#include "IwSoundGroup.h"
#include "IwResManager.h"
#include "IwSound.h"
#include "IwSoundInst.h"
#include "IwSoundSpec.h"
#include "IwTextParserITX.h"

// Forward Declarations

//-----------------------------------------------------------------------------
// CIwSoundGroup
//-----------------------------------------------------------------------------
IW_CLASS_FACTORY(CIwSoundGroup);
IW_MANAGED_IMPLEMENT(CIwSoundGroup)
CIwSoundGroup::CIwSoundGroup()
{
    IW_CALLSTACK("CIwSoundGroup::CIwSoundGroup")

    m_Vol           = IW_GEOM_ONE;
    m_Pan           = 0;
    m_Pitch         = IW_GEOM_ONE;
    m_MaxPolyphony  = 4;
    m_CurrPolyphony = 0;
    m_Flags         = 0;
}
//-----------------------------------------------------------------------------
void    CIwSoundGroup::Serialise()
{
    IW_CALLSTACK("CIwSoundGroup::Serialise")

    CIwManaged::Serialise();
    IwSerialiseInt16(m_Vol);
    IwSerialiseInt16(m_Pan);
    IwSerialiseInt16(m_Pitch);
    IwSerialiseUInt16(m_MaxPolyphony);
    IwSerialiseUInt16(m_Flags);
}
//-----------------------------------------------------------------------------
void    CIwSoundGroup::KillOldestInst(bool alreadyGotNewInst)
{
    IW_CALLSTACK("CIwSoundGroup::KillOldestInst")

    CIwSoundInst* pOldest = NULL;
    uint16 age  = 0;
    uint32 mi   = IwGetSoundManager()->m_NumActiveInsts - ((alreadyGotNewInst == true) ? 1 : 0);
    for (uint32 i = 0; i < mi; i++)
    {
        CIwSoundInst* pInst = IwGetSoundManager()->m_SoundInstPtrs[i];
        if (pInst->GetSpec()->GetGroup() == this)
        {
            if (pInst->GetCount() > age)
            {
                pOldest = pInst;
                age = pInst->GetCount();
            }
        }
    }
#if IW_DEBUG
    IwAssertMsgTrap(SOUND, pOldest, ("Failed to find instance in this group"), return);
#endif

    // Kill oldest instance
    pOldest->Stop();

    // Decrease current polyphony count
    m_CurrPolyphony--;
}
//-----------------------------------------------------------------------------
void    CIwSoundGroup::Stop()
{
    IW_CALLSTACK("CIwSoundGroup::Stop")
    for (uint32 i = 0; i < IwGetSoundManager()->m_NumActiveInsts; i++)
    {
        CIwSoundInst* pInst = IwGetSoundManager()->m_SoundInstPtrs[i];
        if (pInst->GetSpec() && pInst->GetSpec()->GetGroup() == this)
            pInst->Stop();
    }
}
//-----------------------------------------------------------------------------
void    CIwSoundGroup::Pause()
{
    IW_CALLSTACK("CIwSoundGroup::Pause")
    for (uint32 i = 0; i < IwGetSoundManager()->m_NumActiveInsts; i++)
    {
        CIwSoundInst* pInst = IwGetSoundManager()->m_SoundInstPtrs[i];
        if (pInst->GetSpec()->GetGroup() == this)
            pInst->Pause();
    }
}
//-----------------------------------------------------------------------------
void    CIwSoundGroup::Resume()
{
    IW_CALLSTACK("CIwSoundGroup::Resume")
    for (uint32 i = 0; i < IwGetSoundManager()->m_NumActiveInsts; i++)
    {
        CIwSoundInst* pInst = IwGetSoundManager()->m_SoundInstPtrs[i];
        if (pInst->GetSpec()->GetGroup() == this)
            pInst->Resume();
    }
}
//-----------------------------------------------------------------------------
bool    CIwSoundGroup::ParseAttribute(CIwTextParserITX* pParser, const char* pAttrName)
{
    IW_CALLSTACK("CIwSoundGroup::ParseAttribute")

#ifndef IW_DEBUG
    IwAssertMsg(SOUND, false, ("Project not built with IW_DEBUG"));
    return false;
#else
    if (!strcmp(pAttrName, "addSpec"))
    {
        // Locate CIwSoundSpec resource
        char line[80];
        pParser->ReadString(line, 80);
        CIwSoundSpec* pSpec = (CIwSoundSpec*)IwGetResManager()->GetResNamed(line, IW_SOUND_RESTYPE_SPEC);
        IwAssertMsgTrap(SOUND, pSpec, ("Could not find CIwSoundSpec named %s", line), return true);
        pSpec->SetGroup(this);
    }
    else
    if (!strcmp(pAttrName, "vol"))
    {
        iwfixed v;
        pParser->ReadFixed(&v);
        m_Vol = (iwsfixed)v;
    }
    else
    if (!strcmp(pAttrName, "pan"))
    {
        iwfixed v;
        pParser->ReadFixed(&v);
        m_Pan = (iwsfixed)v;
    }
    else
    if (!strcmp(pAttrName, "pitch"))
    {
        iwfixed v;
        pParser->ReadFixed(&v);
        m_Pitch = (iwsfixed)v;
    }
    else
    if (!strcmp(pAttrName, "maxPolyphony"))
        pParser->ReadUInt16(&m_MaxPolyphony);
    else
    if (!strcmp(pAttrName, "killOldest"))
    {
        bool b;
        pParser->ReadBool(&b);
        if (b == true)
            m_Flags |= KILL_OLDEST_F;
        else
            m_Flags &= ~KILL_OLDEST_F;
    }
    else
        return CIwManaged::ParseAttribute(pParser, pAttrName);

#endif
    return true;
}
//-----------------------------------------------------------------------------
void    CIwSoundGroup::ParseClose(CIwTextParserITX* pParser)
{
    IW_CALLSTACK("CIwSoundGroup::ParseClose")

#ifndef IW_DEBUG
    IwAssertMsg(SOUND, false, ("Project not built with IW_DEBUG"));
#else
    // There must be a current resource group
    IwAssertMsgTrap(SOUND, IwGetResManager()->GetCurrentGroup(), ("No current resource group - don't know what to do with created object"), return);

    // Add to current resource group
    IwGetResManager()->GetCurrentGroup()->AddRes(IW_SOUND_RESTYPE_GROUP, this);
#endif
}

// TODO... NO HANDLER MEANS THESE WON'T GET FREED!
