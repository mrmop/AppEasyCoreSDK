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
 * Sound description.
 */
#include "IwSoundSpec.h"
#include "IwResManager.h"
#include "s3eSound.h"
#include "IwSound.h"
#include "IwSoundData.h"
#include "IwSoundGroup.h"
#include "IwSoundInst.h"
#include "IwSoundParams.h"
#include "IwSoundPCM.h"
#include "IwSoundADPCM.h"
#include "IwDebug.h"
#include "IwRandom.h"

// Forward Declarations

//-----------------------------------------------------------------------------
// CIwSoundSpec callbacks
//-----------------------------------------------------------------------------
int _IwSoundSpec_EndSampleCB(s3eSoundEndSampleInfo* esInfo, void* userData)
{
    IW_CALLSTACK("_IwSoundSpec_EndSampleCB")

    if (esInfo->m_RepsRemaining)
        return 1;

    // End sound
    CIwSoundInst* pInst = (CIwSoundInst*)userData;
    pInst->m_Flags |= CIwSoundInst::COMPLETE_F;

    return 0;
}

//-----------------------------------------------------------------------------
// CIwSoundSpec
//-----------------------------------------------------------------------------
IW_CLASS_FACTORY(CIwSoundSpec);
IW_MANAGED_IMPLEMENT(CIwSoundSpec)
CIwSoundSpec::CIwSoundSpec()
{
    IW_CALLSTACK("CIwSoundSpec::CIwSoundSpec")

    m_Data      = NULL;
    m_VolMin    = IW_GEOM_ONE;
    m_VolMax    = IW_GEOM_ONE;
    m_PanMin    = 0;
    m_PanMax    = 0;
    m_PitchMin  = IW_GEOM_ONE;
    m_PitchMax  = IW_GEOM_ONE;
    m_Group     = NULL;
    m_Flags     = 0;
}
//-----------------------------------------------------------------------------
CIwSoundSpec::~CIwSoundSpec()
{
    IW_CALLSTACK("CIwSoundSpec::~CIwSoundSpec");

    if (IwGetSoundManager())
        IwGetSoundManager()->StopSoundSpec(this);
}
//-----------------------------------------------------------------------------
void    CIwSoundSpec::Serialise()
{
    IW_CALLSTACK("CIwSoundSpec::Serialise")

    CIwManaged::Serialise();
    IwSerialiseUInt16(m_Flags);
    IwSerialiseInt16(m_VolMin);
    IwSerialiseInt16(m_VolMax);
    IwSerialiseInt16(m_PanMin);
    IwSerialiseInt16(m_PanMax);
    IwSerialiseInt16(m_PitchMin);
    IwSerialiseInt16(m_PitchMax);

    // To be resolved:
    IwSerialiseManagedHash(&m_Data);
    IwSerialiseManagedHash(&m_Group);
}
//-----------------------------------------------------------------------------
void    CIwSoundSpec::Resolve()
{
    IW_CALLSTACK("CIwSoundSpec::Resolve")

    CIwManaged::Resolve();
    IwResolveManagedHash(&m_Data, IW_SOUND_RESTYPE_DATA);
    IwResolveManagedHash(&m_Group, IW_SOUND_RESTYPE_GROUP);
}
//-----------------------------------------------------------------------------
CIwSoundInst* CIwSoundSpec::Play(const CIwSoundParams* pParams)
{
    IW_CALLSTACK("CIwSoundSpec::Play")

    // Get free inst - Fail if not available
    CIwSoundInst* pInst = IwGetSoundManager()->GetFreeInst();
    if (!pInst)
        return NULL;

    // Fail if no free channel
    int32 chanID = IwGetSoundManager()->GetFreeChannel(pInst, m_Data->m_Format);
    if (chanID < 0) {
        IwGetSoundManager()->SetFreeInst(pInst);
        return NULL;
    }

    IwTrace(SOUND, ("Playing %s %p", DebugGetName(), m_Data->m_Samples));

    // Get specified or identity modifying params
    if (!pParams)
        pParams = IwGetSoundManager()->GetParamsIdentity();

    // Get specified or identity group
    CIwSoundGroup* pGroup = NULL;
    if (m_Group)
    {
        pGroup = m_Group;
        if (pGroup->GetMaxPolyphony())
        {
            if (pGroup->GetMaxPolyphony() == pGroup->GetCurrPolyphony())
            {
                //!RICH - I have changed the default behaviour here from killing
                //!RICH - the oldest instance to just preventing any new sounds
                //!RICH - being played. This is because there are multithreading
                //!RICH - issues to consider, which aren't trivial. The current
                //!RICH - KillOldestInst isn't strictly correct, as the polyphony
                //!RICH - gets decremented twice - once in our thread, once in
                //!RICH - the sound thread. Also, because of the delayed stop,
                //!RICH - if many instances are fired off at once, the fixed
                //!RICH - buffer can overflow
                if (pGroup->GetFlags() & CIwSoundGroup::KILL_OLDEST_F)
                {
                    // Kill oldest instance
                    pGroup->KillOldestInst(true);
                }
                else
                {
                    // Sorry, we're at our limit
                    IwGetSoundManager()->SetFreeInst(pInst);
                    return NULL;
                }
            }
        }
        // Increase curr polyphony
        pGroup->m_CurrPolyphony++;
    }
    else
    {
        pGroup = (CIwSoundGroup*)IwGetSoundManager()->GetGroupIdentity();
    }

    pInst->m_Count = 0;

    // Get vol/pan/pitch to store with inst
    iwsfixed vol    = (m_VolMin == m_VolMax) ? m_VolMin : (iwsfixed)IwRandMinMax(m_VolMin, m_VolMax);
    iwsfixed pan    = (m_PanMin == m_PanMax) ? m_PanMin : (iwsfixed)IwRandMinMax(m_PanMin, m_PanMax);
    iwsfixed pitch  = (m_PitchMin == m_PitchMax) ? m_PitchMin : (iwsfixed)IwRandMinMax(m_PitchMin, m_PitchMax);

    // Write to inst
    pInst->m_Spec   = this;
    pInst->m_Vol    = (iwsfixed)IW_FIXED_MUL(vol, pParams->m_Vol);
    pInst->m_Pan    = (iwsfixed)MAX(-IW_GEOM_ONE, MIN(IW_GEOM_ONE, pan + pParams->m_Pan));
    pInst->m_Pitch  = (iwsfixed)IW_FIXED_MUL(pitch, pParams->m_Pitch);
    pInst->m_ChanID = (uint16)chanID;
    pInst->m_EndSampleCB = NULL;

    // Get final vol/pan/pitch
    vol     = (iwsfixed)IW_FIXED_MUL(pInst->m_Vol, IW_FIXED_MUL(IwGetSoundManager()->GetMasterVol(), pGroup->m_Vol));
    pan     = (iwsfixed)MAX(-IW_GEOM_ONE, MIN(IW_GEOM_ONE, pan + IwGetSoundManager()->GetMasterPan() + pGroup->m_Pan));
    pitch   = (iwsfixed)IW_FIXED_MUL(pitch, IW_FIXED_MUL(IwGetSoundManager()->GetMasterPitch(), pGroup->m_Pitch));

    bool looped = IsLooped();

    if (IwGetSoundManager()->IsActive() == false)
        return pInst;

    s3eSoundChannelRegister(chanID, S3E_CHANNEL_END_SAMPLE, (s3eCallback)_IwSoundSpec_EndSampleCB, pInst);

    // Set volume
//  s3eSoundChannelSetInt(chanID, S3E_CHANNEL_VOLUME, vol << 4);
    // PROPOSED FIX TO #379
    s3eSoundChannelSetInt(chanID, S3E_CHANNEL_VOLUME, MIN(S3E_SOUND_MAX_VOLUME, vol >> 4)); // convert from .12 to .8 format

    // TODO: SET PAN

    // Set pitch
    int32 pp = IW_FIXED_MUL(pitch, pInst->GetSpec()->GetData()->m_RecPitch);
    s3eSoundChannelSetInt(chanID, S3E_CHANNEL_RATE, pp);

    // Play sound according to sample formatzzscz
    switch (m_Data->m_Format)
    {
    case PCM_8BIT_MONO:
    {
        CIwArray<int8> samples;
        m_Data->GetData(samples);
        s3eSoundChannelPlay(chanID, (int16*) samples.begin(), (int16*) samples.end()-(int16*)samples.begin(), (looped ? -1 : 1), 0);
    }
    break;

    case PCM_16BIT_MONO:
    {
        // Get an array containing the data
        CIwArray<int16> samples;
        m_Data->GetData(samples);
        s3eSoundChannelPlay(chanID, (int16*)samples.begin(), (int16*)samples.end()-(int16*)samples.begin(), (looped ? -1 : 1), 0);
    }
    break;

    case ADPCM_MONO:
    {
        CIwArray<int16> samples;
        m_Data->GetData(samples);
        s3eSoundChannelSetInt(chanID, S3E_CHANNEL_USERVAR, (intptr_t)m_Data);
        s3eSoundChannelPlay(chanID, (int16*)samples.begin(), (int16*)samples.end()-(int16*)samples.begin(), (looped ? -1 : 1), 0);
    }
    break;
    }

    // Set callbacks

    return pInst;
}
//-----------------------------------------------------------------------------
bool    CIwSoundSpec::ParseAttribute(CIwTextParserITX* pParser, const char* pAttrName)
{
    IW_CALLSTACK("CIwSoundSpec::ParseAttribute")

#ifndef IW_DEBUG
    IwAssertMsg(SOUND, false, ("Project not built with IW_DEBUG"));
    return false;
#else
    if (!strcmp(pAttrName, "data"))
    {
        // Locate CIwSoundData resource
        char line[80];
        pParser->ReadString(line, 80);
        m_Data = (CIwSoundData*)IwGetResManager()->GetResNamed(line, IW_SOUND_RESTYPE_DATA);
        IwAssertMsg(SOUND, m_Data, ("Could not find CIwSoundData named %s", line));
    }
    else
    if (!strcmp(pAttrName, "vol"))
    {
        iwfixed v;
        pParser->ReadFixed(&v);
        m_VolMin = m_VolMax = (iwsfixed)v;
    }
    else
    if (!strcmp(pAttrName, "volMin"))
    {
        iwfixed v;
        pParser->ReadFixed(&v);
        m_VolMin =  (iwsfixed)v;
    }
    else
    if (!strcmp(pAttrName, "volMax"))
    {
        iwfixed v;
        pParser->ReadFixed(&v);
        m_VolMax =  (iwsfixed)v;
    }
    else
    if (!strcmp(pAttrName, "pitch"))
    {
        iwfixed v;
        pParser->ReadFixed(&v);
        m_PitchMin = m_PitchMax = (iwsfixed)v;
    }
    else
    if (!strcmp(pAttrName, "pitchMin"))
    {
        iwfixed v;
        pParser->ReadFixed(&v);
        m_PitchMin =  (iwsfixed)v;
    }
    else
    if (!strcmp(pAttrName, "pitchMax"))
    {
        iwfixed v;
        pParser->ReadFixed(&v);
        m_PitchMax =  (iwsfixed)v;
    }
    else
    if (!strcmp(pAttrName, "loop"))
    {
        bool b;
        pParser->ReadBool(&b);
        if (b == true)
            m_Flags |= LOOPED_F;
        else
            m_Flags &= ~LOOPED_F;
    }
    else
    if (!strcmp(pAttrName, "group"))
    {
        // Locate CIwSoundGroup resource
        char line[80];
        pParser->ReadString(line, 80);
        CIwSoundGroup* pGroup = IwSafeCast<CIwSoundGroup*>
            (IwGetResManager()->GetResNamed(line, IW_SOUND_RESTYPE_GROUP));
        IwAssertMsgTrap(SOUND, pGroup,
            ("Could not find CIwSoundGroup named %s", line), return true);
        SetGroup(pGroup);
    }
    else
        return CIwManaged::ParseAttribute(pParser, pAttrName);

#endif
    return true;
}
//-----------------------------------------------------------------------------
void    CIwSoundSpec::ParseClose(CIwTextParserITX* pParser)
{
    IW_CALLSTACK("CIwSoundSpec::ParseClose")

#ifndef IW_DEBUG
    IwAssertMsg(SOUND, false, ("Project not built with IW_DEBUG"));
#else
    // There must be a current resource group
    IwAssertMsgTrap(SOUND, IwGetResManager()->GetCurrentGroup(), ("No current resource group - don't know what to do with created object"), return);

    // Add to current resource group
    IwGetResManager()->GetCurrentGroup()->AddRes(IW_SOUND_RESTYPE_SPEC, this);
#endif
}
//-----------------------------------------------------------------------------
void    CIwSoundSpec::Trace()
{
    // Output header info
#ifdef IW_DEBUG
    IwTrace(SOUND, ("\"%s\"", DebugGetName()));
#endif
    IwTrace(SOUND, ("Hash: %u Vol: %d->%d Pitch: %d->%d Pan: %d->%d",
                    m_Hash, m_VolMin, m_VolMax, m_PitchMin, m_PitchMax, m_PanMin, m_PanMax));

    // Output an ascii graph representing the sample
    const int32 cColumns = 80;      // Width of graph
    const int32 cRows = 32;         // Height of graph

    // Symbols used to draw the graph - Should look something like this:
    //  .:'|
    //  ||||
    //  --------
    //      ||||
    //      ':!|
    // This gives us four units per row of the graph.
    const char cSymbolsPos[] = { ' ', '.', ':', '\'', '|' };
    const char cSymbolsNeg[] = { '|', '!', ':', '\'', ' ' };

    char lineBuffer[cColumns + 1];  // Allow space for terminator
    uint32 sampleSize = m_Data->m_SampleCount;

    int16 heightUnit;           // Sample value represented by each quarter row.
    CIwArray<int16> samples16;
    CIwArray<int8> samples8;

    switch (m_Data->m_Format)
    {
        case PCM_8BIT_MONO:
            heightUnit = 0x100 / (cRows * 4);
            m_Data->GetData(samples8);
            IwTrace(SOUND, ("Format: PCM 8-bit mono"));
            break;
        case PCM_16BIT_MONO:
            heightUnit = 0x10000 / (cRows * 4);
            m_Data->GetData(samples16);
            IwTrace(SOUND, ("Format: PCM 16-bit mono"));
            break;
        default:
            // Unsupported format
            IwTrace(SOUND, ("Format: Unsupported"));
            return;
    }

    // Draw the graph
    for (int32 y = cRows / 2 - 1; y >= (-cRows / 2); y--)   // Counting cRows
    {
        int16 sampleMin = (int16)(y * 4);
        const char* symbols = (y >= 0) ? cSymbolsPos : cSymbolsNeg;

        int32 x = 0;

        for (x = 0; x < cColumns; x++)
        {
            int16 sampleUnits;
            if (m_Data->m_Format == PCM_16BIT_MONO)
                sampleUnits = samples16[(x * sampleSize) / cColumns] / heightUnit;
            else
                sampleUnits = samples8[(x * sampleSize) / cColumns] / heightUnit;

            int16 remainder = sampleUnits - sampleMin;

            if (sampleMin >= sampleUnits) remainder = 0;
            else if (remainder > 4) remainder = 4;

            lineBuffer[x] = symbols[remainder];
        }
        // Start and terminate string and output
        lineBuffer[x] = '\0';

        #ifdef IW_USE_TRACING
        int16 axisVal = (y >= 0) ? (sampleMin + 4) * heightUnit : sampleMin * heightUnit;
        IwTrace(SOUND, ("%6d %s", axisVal, lineBuffer));
        #endif

        if (y == 0)
        {
            // axis line
            memset(lineBuffer, '-', cColumns);
            lineBuffer[0] = '0';
            lineBuffer[cColumns] = '\0';
            IwTrace(SOUND, ("%6d %s", 0, lineBuffer));
        }
    }
}
