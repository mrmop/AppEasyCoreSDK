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
#include "IwSoundADPCM.h"

#include "IwOptimiseSpeed.h"
#include "IwOptimiseARM.h"
#include "IwDebug.h"
#include "IwSoundData.h"
#include "IwSoundPCM.h"

#ifdef WRITE_FILE
FILE* op;
#endif

int CIwChannelADPCM::isInitialized = 0;

int CIwChannelADPCM::GenerateAudio(s3eSoundGenAudioInfo* pInfo)
{
    int16* aTarget = pInfo->m_Target;
    int aLength = pInfo->m_NumSamples;
    int mix = pInfo->m_Mix;

    if (!pos)
    {
        //First time GenerateAudio is called for this sample
        //Take the sample data from pInfo->m_OrigStart,pInfo->m_OrigNumSamples. This is what was passed to
        //s3eSoundChannelPlay
        pos = pInfo->m_OrigStart;
        end_pos = (pInfo->m_OrigStart + (pInfo->m_OrigNumSamples &~1));
        CIwSoundDataADPCM* pSData = (CIwSoundDataADPCM*)s3eSoundChannelGetInt(pInfo->m_Channel, S3E_CHANNEL_USERVAR);

        m_SamplesPerBlock = pSData->GetSamplesPerBlock();
        m_BlockAlign  = pSData->GetBlockAlignment();
        m_SamplesRemaining = pSData->GetSampleCount();
        m_LastBlock = 0;

#ifdef WRITE_FILE
        op = fopen ("c:\\op.raw", "Rwb");
#endif


        bres_accum = 0;
        count = 0;
        s_1 = 0             ;
        s_2 = 0;
        bufferstep = 0;
        index = 0;

    }

    int relative_rate = (s3eSoundChannelGetInt(pInfo->m_Channel, S3E_CHANNEL_RATE)*0x1000)/s3eSoundGetInt(S3E_SOUND_OUTPUT_FREQ);
    int volume = s3eSoundChannelGetInt(pInfo->m_Channel, S3E_CHANNEL_VOLUME);

    int endSample = 0;
    int ret = GenerateADPCMAudioFast(aTarget, aLength, relative_rate, volume, mix, &endSample);
    if (endSample)
    {
        pInfo->m_EndSample = S3E_TRUE;
        pos = NULL;
    }
    return ret;
}

static int stepsizeTable[89] = {
    7, 8, 9, 10, 11, 12, 13, 14, 16, 17,
    19, 21, 23, 25, 28, 31, 34, 37, 41, 45,
    50, 55, 60, 66, 73, 80, 88, 97, 107, 118,
    130, 143, 157, 173, 190, 209, 230, 253, 279, 307,
    337, 371, 408, 449, 494, 544, 598, 658, 724, 796,
    876, 963, 1060, 1166, 1282, 1411, 1552, 1707, 1878, 2066,
    2272, 2499, 2749, 3024, 3327, 3660, 4026, 4428, 4871, 5358,
    5894, 6484, 7132, 7845, 8630, 9493, 10442, 11487, 12635, 13899,
    15289, 16818, 18500, 20350, 22385, 24623, 27086, 29794, 32767
};


struct _statTable
{
    int indexTable[16];
    int vpdiffTable[89*8];
};

static _statTable sTable = {
    { -1, -1, -1, -1, 2, 4, 6, 8,
      -1, -1, -1, -1, 2, 4, 6, 8, },
    { 0 }
};


/* Intel ADPCM step variation table
static int indexTable[16] = {
    -1, -1, -1, -1, 2, 4, 6, 8,
    -1, -1, -1, -1, 2, 4, 6, 8,
};
*/

//-----------------------------------------------
void CIwChannelADPCM::Init()
{
    isInitialized = 1;
    int i,j;
//  int* dest = &sTable.vpdiffTable[0];

    for (j = 0; j < 89; j++)
    {
//      if(j <= 88)
        {
            for (i = 0; i < 8; i++)
            {
                sTable.vpdiffTable[j*8 + i] = ((stepsizeTable[j]*i) >> 2) + (stepsizeTable[j] >>3);
            }
        }
//      else
//      {
//          for (i = 0; i < 8; i++)
//          {
//              *dest++ = ((stepsizeTable[88]*i) + (stepsizeTable[88] >>1)) >> 2;
//          }
//      }
    }

}


#if 0

//---------------------------------------------------------------------
int16 adpcm_decode_sample(
    uint8* &input,
    int16 valprev,
    int8& index,
    uint8& bufferstep
    )
{
    int sign;           /* Current adpcm sign bit */
    int delta;          /* Current adpcm output value */
    int step;           /* Stepsize */
    int vpdiff;         /* Current change to valprev */

    if (index < 0) index = 0;
    else if (index > 88) index = 88;
    step = stepsizeTable[index];

    /* Step 1 - get the delta value and compute next index */
    if (bufferstep) {
        delta = (*input++>>4) & 0xf;
    } else {
        delta = (*input) & 0xf;
    }
    bufferstep = !bufferstep;

    /* Step 2 - Find new index value (for later) */
    index += indexTable[delta];
    if (index < 0) index = 0;
    else if (index > 88) index = 88;

    /* Step 3 - Separate sign and magnitude */
    sign = delta & 8;
    delta = delta & 7;

    /* Step 4 - update output value */
#if 1
    vpdiff = step >> 1;
    if (delta & 4) vpdiff += (step << 2);
    if (delta & 2) vpdiff += (step << 1);
    if (delta & 1) vpdiff += step;
    vpdiff >>= 2;
#else
    vpdiff = ((delta*step) >> 2) + (step >> 3);
#endif

    int32 nextval;

    if (sign)
      nextval = valprev - vpdiff;
    else
      nextval = valprev + vpdiff;

    /* Step 5 - clamp output value */
    if (nextval > 32767)
      nextval = 32767;
    else if (nextval < -32768)
      nextval = -32768;

#ifdef WRITE_FILE
    if (op)
        fwrite(&nextval, 2, 1, op);
#endif

    /* Step 7 - Output value */
    return (int16)nextval;
}
#endif
//---------------------------------------------------------------------------------------------------
int CIwChannelADPCM::GenerateADPCMAudioFast(int16* aTarget, int aLength, int relative_rate, int volume, int mix, int* endOfSample)
{
    /*
    for (int i=0;i<aLength;i++)
    {
        *aTarget++ = GetNextSample();
        if (pos >= end_pos) {
            *endOfSample = 1;
            fclose(op);
            return i;
        }
    }
    return aLength;
    */


    int16*          aTarget_orig    = aTarget;
    uint8*          src             = (uint8 *)pos      ;
    int             _bres_accum     = bres_accum        ;
    int             _count          = count         ;
    int16           _s_1            = s_1               ;
    int16           _s_2            = s_2               ;
    int32           _index          = index;
    uint8           _bufferstep     = bufferstep;

        // "size" here is size of uncompressed data (i.e. 28 samples for one 16 byte input block)
        // "src"  where you get the data ((size/28) * 16 bytes)
        // "dest"  where you put the data (size*2 bytes, becsause 16 bit samples)
    if (!aLength)
        return mix;
    if (volume < 8)
    {
        int myLength = aLength;
        if (!mix)
        {
            while (myLength--) {
                *aTarget++ = 0;
            }
        }
        else
            aTarget += myLength;

        do {
            if (_count & 1)goto entry0;
            while (_count)
            {
                _count--;
                _bres_accum += 0x1000;
entry0:
                while (_bres_accum >= relative_rate)
                {
                    _bres_accum -= relative_rate;
                    if (!(--aLength))
                        goto myreturn;
                }
                _count--;
                src++;
                _bres_accum += 0x1000;
                while (_bres_accum >= relative_rate)
                {
                    _bres_accum -= relative_rate;
                    if (!(--aLength))
                        goto myreturn;
                }
            }
            if (src >= (uint8*)end_pos)
            {
                *endOfSample = 1;
                src=0;
                goto myreturn;
            }
            {
                adpcm_state* pState;
                if (!m_LastBlock)
                {
                    pState = (adpcm_state*)src;
                }
                else
                {
                    pState = (adpcm_state*)((uint8*)m_LastBlock+m_BlockAlign);
                }

                m_LastBlock = pState;
                _count = m_SamplesPerBlock-1;
                if ((uint8*)pState+m_BlockAlign > (uint8*)end_pos)
                {
                    _count = (((uint8*)end_pos - (uint8*)pState)-4)*2;
                }
                src = ((uint8*)pState) + 4;
                _s_2 = pState->valprev;
                _index = pState->index;
                _bufferstep = 0;
                m_SamplesRemaining -= m_SamplesPerBlock;
            }
        }while (1);
        goto myreturn;
    }


    if (mix)
    {
        do {
            if (_count)
            {

                register uint8 srcVal;
                if (_count & 1)
                {
                    srcVal = *(src);
                    goto entry;
                }
                do {
                    {
                        srcVal = *(src);
                        _count--;
                        IwAssert(SOUND, _index >=0 && _index <= 88);
                        int delta = srcVal & 0x7;
                        int vpdiff = sTable.vpdiffTable[_index*8 + delta];
                        int indexAdd = sTable.indexTable[delta];
                        int s;
                        if (srcVal & 8)
                            s = _s_2 - vpdiff;
                        else
                            s = _s_2 + vpdiff;

                        _index += indexAdd;
                        if (_index < 0) _index = 0;
                        else if (_index > 88) _index = 88;

                        _s_1 = _s_2;
                        _s_2 = CIwChannelPCM<uint16>::clip_to_int16(s);
                    }
                    _bres_accum += 0x1000;
entry:
                    while (_bres_accum >= relative_rate)
                    {
                        _bres_accum -= relative_rate;
                        int32 samp = (_s_1 - _s_2)*( _bres_accum);
                        int32 curntsamp = *aTarget;
                        samp = (_s_2 + (samp >>12));
                        samp *= volume;
                        aLength--;
                        *aTarget++ = (int16) CIwChannelPCM<uint16>::clip_to_int16((curntsamp + ((samp)>>8)));
                        if (!aLength)
                            goto myreturn;
                    }
                    IwAssert(SOUND, _index >=0 && _index <= 88);
                    srcVal >>= 4;
                    int delta = srcVal & 0x7;
                    int vpdiff = sTable.vpdiffTable[_index*8 + delta];
                    _count--;
                    int indexAdd = sTable.indexTable[delta];
                    src++;
                    int s;
                    if (srcVal & 8)
                        s = _s_2 - vpdiff;
                    else
                        s = _s_2 + vpdiff;

                    _index += indexAdd;

                    if (_index < 0) _index = 0;
                    else if (_index > 88) _index = 88;

                    _s_1 = _s_2;
                    _s_2 =  CIwChannelPCM<uint16>::clip_to_int16(s);
                    _bres_accum += 0x1000;
                    while (_bres_accum >= relative_rate)
                    {
                        _bres_accum -= relative_rate;
                        int32 samp = (_s_1 - _s_2)*( _bres_accum);
                        int32 curntsamp = *aTarget;
                        samp = (_s_2 + (samp >>12));
                        samp *= volume;
                        aLength--;
                        *aTarget++ = (int16) CIwChannelPCM<uint16>::clip_to_int16((curntsamp + ((samp)>>8)));
                        if (!aLength)
                            goto myreturn;
                    }

                }while(_count > 0);
            }

            if (src >= (uint8*)end_pos)
            {
                *endOfSample = 1;
                src=0;
                goto myreturn;
            }

            IwAssert(SOUND,_count == 0);
            {
                adpcm_state* pState;
                if (!m_LastBlock)
                {
                    pState = (adpcm_state*)src;
                }
                else
                {
                    pState = (adpcm_state*)((uint8*)m_LastBlock+m_BlockAlign);
                }

                m_LastBlock = pState;
                _count = m_SamplesPerBlock-1;
                if ((uint8*)pState+m_BlockAlign > (uint8*)end_pos)
                {
                    _count = (((uint8*)end_pos - (uint8*)pState)-4)*2;
                }
                src = ((uint8*)pState) + 4;
                _s_2 = pState->valprev;
                _index = pState->index;
                _bufferstep = 0;
                m_SamplesRemaining -= m_SamplesPerBlock;
            }
        }while (1);
    }
    else
    {
        do {
            if (_count)
            {
                uint8 srcVal;
                if (_count & 1)
                {
                    srcVal = *(src);
                    goto entry2;
                }
                do {
                    {
                        srcVal = *(src);
                        _count--;
                        IwAssert(SOUND, _index >=0 && _index <= 88);
                        int delta = srcVal & 0x7;
                        int vpdiff = sTable.vpdiffTable[_index*8 + delta];
                        int indexAdd = sTable.indexTable[delta];
                        int s;
                        if (srcVal & 8)
                            s = _s_2 - vpdiff;
                        else
                            s = _s_2 + vpdiff;

                        _index += indexAdd;

                        if (_index < 0) _index = 0;
                        else if (_index > 88) _index = 88;

                        _s_1 = _s_2;
                        _s_2 = CIwChannelPCM<uint16>::clip_to_int16(s);
                    }

                    _bres_accum += 0x1000;
entry2:
                    while (_bres_accum >= relative_rate)
                    {
                        int32 samp;
                        _bres_accum -= relative_rate;
                        samp = (_s_2 + (((_s_1 - _s_2)*( _bres_accum))>>12));
                        *aTarget++ = (int16) ((samp * volume)>>8);
                        aLength--;
                        if (!aLength)
                            goto myreturn;
                    }

                    IwAssert(SOUND, _index >=0 && _index <= 88);
                    srcVal >>= 4;
                    int delta = srcVal & 0x7;
                    int vpdiff = sTable.vpdiffTable[_index*8 + delta];
                    _count--;
                    int indexAdd = sTable.indexTable[delta];
                    src++;
                    int s;
                    if (srcVal & 8)
                        s = _s_2 - vpdiff;
                    else
                        s = _s_2 + vpdiff;

                    _index += indexAdd;

                    if (_index < 0) _index = 0;
                    else if (_index > 88) _index = 88;

                    _s_1 = _s_2;
                    _s_2 = CIwChannelPCM<uint16>::clip_to_int16(s);
                    _bres_accum += 0x1000;
                    while (_bres_accum >= relative_rate)
                    {
                        int32 samp;
                        _bres_accum -= relative_rate;
                        samp = (_s_2 + (((_s_1 - _s_2)*( _bres_accum))>>12));
                        *aTarget++ = (int16) ((samp * volume)>>8);
                        aLength--;
                        if (!aLength)
                            goto myreturn;
                    }

                }while(_count > 0);
            }

            if (src >= (uint8*)end_pos)
            {
#ifdef WRITE_FILE
                fclose(op);
#endif
                *endOfSample = 1;
//              while (aLength--) {
//                  *aTarget++ = 0;
//              }
                src=0;
                goto myreturn;
            }
            IwAssert(SOUND,_count == 0);
            {
                adpcm_state* pState;
                if (!m_LastBlock)
                {
                    pState = (adpcm_state*)src;
                }
                else
                {
                    pState = (adpcm_state*)((uint8*)m_LastBlock+m_BlockAlign);
                }

                m_LastBlock = pState;
                _count = m_SamplesPerBlock-1;
                if ((uint8*)pState+m_BlockAlign > (uint8*)end_pos)
                {
                    _count = (((uint8*)end_pos - (uint8*)pState)-4)*2;
                }
                src = ((uint8*)pState) + 4;
                _s_2 = pState->valprev;
                _index = pState->index;
                _bufferstep = 0;
                m_SamplesRemaining -= m_SamplesPerBlock;

#ifdef WRITE_FILE
                if (op)
                    fwrite(&_s_2, 2, 1, op);
#endif
            }
        }while (1);
    }
myreturn:

    pos             = (int16 *)src      ;
    bres_accum      = _bres_accum       ;
    count           = _count                ;
    s_1             = _s_1              ;
    s_2             = _s_2              ;
    bufferstep      = _bufferstep;
    index           = (int8)_index;

    return aTarget - aTarget_orig;
}
