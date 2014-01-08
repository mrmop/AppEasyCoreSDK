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
#include "IwSoundManager.h"

#include "IwClassFactory.h"
#include "IwSingleton.h"
#include "IwSoundData.h"
#include "IwSoundGroup.h"
#include "IwSoundInst.h"
#include "IwSoundParams.h"
#include "IwSoundPCM.h"
#include "IwSoundSpec.h"
#include "s3eConfig.h"
#include "s3eSound.h"

#ifdef IW_DEBUG_IW_ENGINE_DEPENDENT
#include "IwRenderManager.h"
#endif // !IW_DEBUG_IW_ENGINE_DEPENDENT

//! Singleton
IW_SINGLETON_INSTANTIATE(SoundManager);

//--------------------------------------------------------------------------------
void    IwSoundInit()
{
    IW_CALLSTACK("IwSoundInit")

    new CIwSoundManager;

    // Register classes
    IW_CLASS_REGISTER(CIwSoundData);
    IW_CLASS_REGISTER(CIwSoundDataADPCM);
    IW_CLASS_REGISTER(CIwSoundGroup);
    IW_CLASS_REGISTER(CIwSoundSpec);
}
//--------------------------------------------------------------------------------
void    IwSoundTerminate()
{
    IW_CALLSTACK("IwSoundTerminate")

    IwGetSoundManager()->StopAll();

    delete IwGetSoundManager();
}

//-----------------------------------------------------------------------------
// CIwSoundManager
//-----------------------------------------------------------------------------
CIwChannelPCM8*     CIwSoundManager::s_ChannelsPCM8;
CIwChannelPCM16*    CIwSoundManager::s_ChannelsPCM16;
CIwChannelADPCM*    CIwSoundManager::s_ChannelsADPCM;
//-----------------------------------------------------------------------------
CIwSoundManager::CIwSoundManager()
{
    IW_CALLSTACK("CIwSoundManager::CIwSoundManager")
    IW_SINGLETON_SET(SoundManager);

    m_GroupIdentity     = new CIwSoundGroup;
    m_ParamsIdentity    = new CIwSoundParams;
    m_SoundInsts        = NULL;
    m_SoundInstPtrs     = NULL;
    m_NumActiveInsts    = 0;
    m_Flags             = ACTIVE_F;

    m_MasterVol     = IW_GEOM_ONE;
    m_MasterPan     = 0;
    m_MasterPitch   = IW_GEOM_ONE;

    // Allocate our own channels so that they can be used to override Segundo
    // channels where necessary
    int cfgChannels = 8;
    s3eConfigGetInt("SOUND", "MaxChannels", &cfgChannels);
    int32 s3eChannels = s3eSoundGetInt(S3E_SOUND_NUM_CHANNELS);

    m_MaxInsts = MIN(cfgChannels, s3eChannels);

    s_ChannelsPCM8 = new CIwChannelPCM8[m_MaxInsts];
    s_ChannelsPCM16 = new CIwChannelPCM16[m_MaxInsts];
    s_ChannelsADPCM = new CIwChannelADPCM[m_MaxInsts];

    m_SoundInsts        = new CIwSoundInst[m_MaxInsts];
    m_SoundInstPtrs     = new CIwSoundInst*[m_MaxInsts];
    for (uint32 i = 0; i < m_MaxInsts; i++)
        m_SoundInstPtrs[i] = &m_SoundInsts[i];
}
//-----------------------------------------------------------------------------
CIwSoundManager::~CIwSoundManager()
{
    IW_CALLSTACK("CIwSoundManager::~CIwSoundManager")
    IW_SINGLETON_CLEAR(SoundManager);

    delete m_GroupIdentity;
    delete m_ParamsIdentity;
    delete[] m_SoundInsts;
    delete[] m_SoundInstPtrs;
    delete[] s_ChannelsPCM8;
    delete[] s_ChannelsPCM16;
    delete[] s_ChannelsADPCM;
}
//-----------------------------------------------------------------------------
void    CIwSoundManager::SetMaxSoundInsts(uint32 n)
{
    IW_CALLSTACK("CIwSoundManager::SetMaxSoundInsts")

    IwAssertMsg(SOUND, false, ("DEPRECATED (max insts set via [SOUND] MaxChannels - currently %u", m_MaxInsts));
}
//-----------------------------------------------------------------------------
void    CIwSoundManager::UpdateCompleted(int i)
{
    CIwSoundInst* pInst = m_SoundInstPtrs[i];

    if (pInst->GetSpec()->GetGroup())
        pInst->GetSpec()->GetGroup()->m_CurrPolyphony--;

    if (pInst->GetEndSampleCB())
        pInst->GetEndSampleCB()(pInst);

    pInst->m_Flags &= ~CIwSoundInst::COMPLETE_F;

    SetFreeInst(pInst);
}
//-----------------------------------------------------------------------------
void    CIwSoundManager::Update()
{
    IW_CALLSTACK("CIwSoundManager::Update")

    if (IsActive() == false)
        return;

    m_TimeStamp++;

    // Update all active insts
    uint32 i;
    for (i = 0; i < m_NumActiveInsts; i++)
    {
        CIwSoundInst* pInst = m_SoundInstPtrs[i];

        if (pInst->GetSpec() == NULL) // Safety!
            continue;

        if (pInst->GetFlags() & CIwSoundInst::COMPLETE_F)
        {
            UpdateCompleted(i);
            i--; //free instance is in this slot following SetFreeInst...
        }
        else
        {
            // Update inst
            pInst->m_Count++;
        }
    }

    for (i = 0; i < m_NumActiveInsts; i++)
    {
        CIwSoundInst* pInst = m_SoundInstPtrs[i];
        if (pInst->GetSpec() == NULL)
        {
            IwAssertMsg(SOUND, false, ("IwSoundManager::Update problem."));
            return;
        }

        const CIwSoundGroup* pGroup = pInst->GetSpec()->GetGroup() ? pInst->GetSpec()->GetGroup() : GetGroupIdentity();

        if  (
            (pInst->GetFlags() & CIwSoundInst::DIRTY_F) ||
            (pGroup->GetFlags() & CIwSoundGroup::DIRTY_F) ||
            (m_Flags & DIRTY_F)
            )
        {
            // Channel requires updating

            // Get new vol/pan/pitch
            // TODO... ALLOW CHANGES ON CIwSoundInst
            iwsfixed vol    = (iwsfixed)IW_FIXED_MUL(pInst->GetVol(), IW_FIXED_MUL(GetMasterVol(), pGroup->GetVol()));
//          iwsfixed pan    = (iwsfixed)MAX(-IW_GEOM_ONE, MIN(IW_GEOM_ONE, pInst->GetPan() + GetMasterPan() + pGroup->GetPan()));
            iwsfixed pitch  = (iwsfixed)IW_FIXED_MUL(pInst->GetPitch(), IW_FIXED_MUL(GetMasterPitch(), pGroup->GetPitch()));

            //TODO, FIX vol
            s3eSoundChannelSetInt(pInst->GetChanID(), S3E_CHANNEL_VOLUME, MIN(S3E_SOUND_MAX_VOLUME, vol >> 4));

            // TODO: PAN
            int32 pp = IW_FIXED_MUL(pitch, pInst->GetSpec()->GetData()->GetRecPitch());
            s3eSoundChannelSetInt(pInst->GetChanID(), S3E_CHANNEL_RATE, pp);

            // Clear inst dirty flags
            pInst->m_Flags &= ~CIwSoundInst::DIRTY_F;
        }
    }
    // Clear all group dirty flags
    for (i = 0; i < m_NumActiveInsts; i++)
    {
        CIwSoundInst* pInst = m_SoundInstPtrs[i];

        if (pInst->GetSpec() == NULL) // Safety!
            continue;

        CIwSoundGroup* pGroup = (CIwSoundGroup*)pInst->GetSpec()->GetGroup();
        if (pGroup)
            pGroup->m_Flags &= ~CIwSoundGroup::DIRTY_F;
    }

    // Clear manager dirty flag
    m_Flags &= ~DIRTY_F;
}
//-----------------------------------------------------------------------------
int32 CIwSoundManager::GetFreeChannel(CIwSoundInst* pInst, IwSoundDataFormat format)
{
    if (IsActive() == false)
        return -1;

    // Get a channel from Segundo
    int32 id = s3eSoundGetFreeChannel();
    if (id >= (int32)m_MaxInsts)
        return -1;

    // Now set the channel's callbacks based on the requested format
    switch (format)
    {
    case PCM_8BIT_MONO:
        {
            CIwChannelPCM8* pChannel;
            pChannel = &s_ChannelsPCM8[id];
            s3eSoundChannelRegister(id, S3E_CHANNEL_GEN_AUDIO, (s3eCallback)CIwChannelPCM8::GenerateAudioCB, (void*)pChannel);
            pChannel->pos = 0;
            pChannel->m_Instance = pInst;
        }
        break;
    case PCM_16BIT_MONO:
        {
            CIwChannelPCM16* pChannel;
            pChannel = (CIwChannelPCM16*) &s_ChannelsPCM16[id];
            s3eSoundChannelRegister(id, S3E_CHANNEL_GEN_AUDIO, (s3eCallback)CIwChannelPCM16::GenerateAudioCB, (void*)pChannel);
            //s3eSoundChannelUnRegister(id, S3E_CHANNEL_GEN_AUDIO);
            pChannel->pos = 0;
            pChannel->m_Instance = pInst;
        }
        break;
    case ADPCM_MONO:
        {
            CIwChannelADPCM* pChannel;
            pChannel = (CIwChannelADPCM*) &s_ChannelsADPCM[id];
            s3eSoundChannelRegister(id, S3E_CHANNEL_GEN_AUDIO, (s3eCallback)CIwChannelADPCM::GenerateAudioCB, (void*)pChannel);
            pChannel->pos = 0;
        }
        break;

    default:
        break;
    }

    return id;
}
//-----------------------------------------------------------------------------
CIwSoundInst* CIwSoundManager::GetFreeInst()
{
    IW_CALLSTACK("CIwSoundManager::GetFreeInst")

    // Make first free inst active
    IwAssertMsg(SOUND, m_NumActiveInsts < m_MaxInsts, ("No free CIwSoundInst"));

    if (m_NumActiveInsts >= m_MaxInsts)
        return NULL;

    m_NumActiveInsts++;

    CIwSoundInst* pInst = m_SoundInstPtrs[m_NumActiveInsts - 1];
    IwAssert(SOUND, m_SoundInstPtrs[m_NumActiveInsts - 1]->GetFlags() & CIwSoundInst::FREE_F);
    pInst->m_Flags &= ~CIwSoundInst::FREE_F;

    return pInst;
}
//-----------------------------------------------------------------------------
void    CIwSoundManager::SetFreeInst(CIwSoundInst* pInst)
{
    IW_CALLSTACK("CIwSoundManager::SetFreeInst")
    IwAssertMsg(SOUND, pInst, ("Null ptr"));
#if IW_DEBUG
    IwAssertMsgTrap(SOUND, m_NumActiveInsts > 0, ("Must have at least one active inst"), return);
#endif

    pInst->m_Flags |= CIwSoundInst::FREE_F;
    pInst->m_PlayID++; //increment here so playID is immediately invalid

    // Get ptr to last active inst
    CIwSoundInst** ppLastActive = &m_SoundInstPtrs[m_NumActiveInsts - 1];

    // Find ptr to this inst
    CIwSoundInst** ppPtr = m_SoundInstPtrs;
    while (ppPtr <= ppLastActive)
    {
        if (*ppPtr == pInst)
        {
            *ppPtr = *ppLastActive;
            *ppLastActive = pInst;
            m_NumActiveInsts--;
            return;
        }
        ++ppPtr;
    }
    IwAssertMsg(SOUND, false, ("Failed to find ptr to this inst in the list"));
}
//-----------------------------------------------------------------------------
void    CIwSoundManager::StopAll()
{
    IW_CALLSTACK("CIwSoundManager::StopAll")
    for (uint32 i = 0; i < m_NumActiveInsts; i++)
    {
        CIwSoundInst* pInst = m_SoundInstPtrs[i];
        pInst->Stop();
    }
    // Ensure all CIwSoundInsts become inactive
    Update();

    s3eDeviceYield(50);
}
//-----------------------------------------------------------------------------
void    CIwSoundManager::StopSoundSpec(CIwSoundSpec* pSpec)
{
    IW_CALLSTACK("CIwSoundManager::StopSoundSpec")
    if (pSpec == NULL)
        return;
    for (int32 i = m_NumActiveInsts-1; i>=0; i--)
    {
        CIwSoundInst* pInst = m_SoundInstPtrs[i];
        if (pInst->GetSpec() == pSpec)
        {
            pInst->Stop();
            UpdateCompleted(i);
        }
    }

    s3eDeviceYield(50);
}
//-----------------------------------------------------------------------------
void    CIwSoundManager::PauseAll()
{
    IW_CALLSTACK("CIwSoundManager::PauseAll")
    for (uint32 i = 0; i < m_NumActiveInsts; i++)
    {
        CIwSoundInst* pInst = m_SoundInstPtrs[i];
        pInst->Pause();
    }
}
//-----------------------------------------------------------------------------
void    CIwSoundManager::ResumeAll()
{
    IW_CALLSTACK("CIwSoundManager::ResumeAll")
    for (uint32 i = 0; i < m_NumActiveInsts; i++)
    {
        CIwSoundInst* pInst = m_SoundInstPtrs[i];
        pInst->Resume();
    }
}

//-----------------------------------------------------------------------------
// Debug
//-----------------------------------------------------------------------------
#ifdef IW_DEBUG

#ifdef IW_DEBUG_IW_ENGINE_DEPENDENT
void    CIwSoundManager::DebugRender()
{
    IW_CALLSTACK("CIwSoundManager::DebugRender")

    // Display all active CSoundInst
    int16 x = 0x4;
    int16 y = 0x4;
    uint32 i;
    for (i = 0; i < m_NumActiveInsts; i++)
    {
        CIwSoundInst* pInst = m_SoundInstPtrs[i];

        CIwSVec3 pos(x, y, IW_RENDERMANAGER_DEBUG_TEXT_2D);

        char line[80];
        sprintf(line, "%s %d", pInst->GetSpec()->DebugGetName(), (int)pInst->GetCount());
        IwGetRenderManager()->DebugAddText(line, pos, CIwMenuItem::TITLE);

        y += 0x8;
    }
}
#endif // !IW_DEBUG_IW_ENGINE_DEPENDENT

//-----------------------------------------------------------------------------
// CIwMenuItemSound
//-----------------------------------------------------------------------------
void    CIwMenuItemSound::Select()
{
    IW_CALLSTACK("CIwMenuItemSound::Select")

    // Create debug menu
    CIwMenu* pMenu = new CIwMenu;
    pMenu->AddItem(new CIwMenuItemTitle("IwSound"));
    pMenu->AddItem(new CIwMenuItemEditUInt32("NumActiveInsts", &IwGetSoundManager()->m_NumActiveInsts, 0));
//  pMenu->AddItem(new CIwMenuItemEditFixed("MasterVol", &IwGetSoundManager()->m_MasterVol, 12, 0));

    pMenu->SetItemID(1);
    IwGetMenuManager()->AddMenu(pMenu);
}
#endif  // IW_DEBUG
