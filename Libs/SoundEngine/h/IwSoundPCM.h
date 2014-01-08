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
 * Support for PCM encoded samples
 */
#ifndef IW_SOUND_PCM_H
#define IW_SOUND_PCM_H

#include "s3eSound.h"
#include "IwSoundSpec.h"
#include "s3eTypes.h"

// Forward declarations
class CIwSoundInst;

//-------------------------------------------------------------------------
// CIwChannelPCM Declaration
//-------------------------------------------------------------------------
//! Channel for playing PCM encoded samples
//! Templated on sample type
template <class SAMPLE>
class CIwChannelPCM
{
    //---------------------------------------------------------------------
    // Public API
    //---------------------------------------------------------------------
public:
    // Members
    SAMPLE *                pos;
    SAMPLE *                loop_pos;
    SAMPLE *                end_pos;
    int32                   offset;

    int                     m_ChannelID;    //!< ID of Segundo sound channel this is overriding
    CIwSoundInst*           m_Instance;     //!< Pointer to CIwSoundInst that initiated this sound

    //! Constructor
    CIwChannelPCM();

    //! Mix this channel's sound into a target
    int GenerateAudio(s3eSoundGenAudioInfo* pInfo);

    //! The rate-conversion function
    int GenerateAudioFast(int16* aTarget, int aLength, int relative_rate, int volume, int mix, int* endOfSample);

    //Static Callback
    static int GenerateAudioCB(s3eSoundGenAudioInfo* pSystemData, CIwChannelPCM* pUserData)
    {
        return pUserData->GenerateAudio(pSystemData);
    }


    static INLINE_MEMBER int16 clip_to_int16( int32 sval )
    {
        enum{ minval = -32768, // = int32(~0x7fff) = int16(0x8000)
              maxval =  32767, // = int32( 0x7fff) = int16(0x7fff)
              allbits = 0xffff };

        if ((sval-minval) & ~allbits ) // quick overflow test, the addition moves valid range to 0-allbits
        {
            // we overflowed.
            if (sval > maxval)
                sval = maxval;
            else
            if (sval < minval)
                sval = minval;
            else
            {
                IwAssert(SOUND, false); // one of the two clips should have happened.
            }
        }

        IwAssert(SOUND, sval >= minval);
        IwAssert(SOUND, sval <= maxval);

        return (int16)sval;
    }

};// CIwChannelPCM

//! Declare the different implementations
typedef CIwChannelPCM<int16> CIwChannelPCM16;
typedef CIwChannelPCM<int8> CIwChannelPCM8;

//-------------------------------------------------------------------------
// CIwChannelPCM Definition
//-------------------------------------------------------------------------
//! This needs to be done in the header file to support the templating

template <class SAMPLE>
CIwChannelPCM<SAMPLE>::CIwChannelPCM()
:   pos(NULL),
    end_pos(NULL),
    offset(0),
    m_Instance(NULL)
{
}
//-------------------------------------------------------------------------
// Member Functions
//-------------------------------------------------------------------------
template <class SAMPLE>
int CIwChannelPCM<SAMPLE>::GenerateAudio(s3eSoundGenAudioInfo* pInfo)
{
    int16* aTarget = pInfo->m_Target;
    int aLength = pInfo->m_NumSamples;
    int mix = pInfo->m_Mix;

    if (!pos)
    {
        //First time GenerateAudio is called for this sample
        //Take the sample data from pInfo->m_OrigStart,pInfo->m_OrigNumSamples. This is what was passed to
        //s3eSoundChannelPlay
        pos = (SAMPLE*)pInfo->m_OrigStart;
        end_pos = (SAMPLE*)(pInfo->m_OrigStart + pInfo->m_OrigNumSamples);
        offset = 0;
    }

    int relative_rate = (s3eSoundChannelGetInt(pInfo->m_Channel, S3E_CHANNEL_RATE)*0x1000)/s3eSoundGetInt(S3E_SOUND_OUTPUT_FREQ);
    int volume = s3eSoundChannelGetInt(pInfo->m_Channel, S3E_CHANNEL_VOLUME);

    if (relative_rate != 0x1000)
    {
        int endSample = 0;
        int ret = GenerateAudioFast(aTarget, aLength, relative_rate, volume, mix, &endSample);
        if (endSample)
        {
            pInfo->m_EndSample = S3E_TRUE;
            pos = NULL;
        }
        return ret;
    }
    do {
        IwAssert(SOUND, end_pos >= pos);
        IwAssert(SOUND, pos != NULL);

        int remain = end_pos - pos;
        int qty = MIN(remain, aLength);
        aLength -= qty;

        if (volume == 0)
        {
            if (!mix)
                memset(aTarget, 0, sizeof(*aTarget)*qty);
            pos += qty, aTarget += qty, qty=0;
        }
        else
        {
            if (mix)
                for (; qty; qty--, pos++, aTarget++)
                    *aTarget += (*pos * volume) >> 8;
            else
                for (; qty; qty--, pos++, aTarget++)
                    *aTarget  = (*pos * volume) >> 8;
        }

        if (pos == end_pos)
        {

            pos = NULL;
            pInfo->m_EndSample = 1;
            return pInfo->m_NumSamples - aLength;
        }
    } while (aLength);

    return pInfo->m_NumSamples;
}
//-------------------------------------------------------------------------
// Static API
//-------------------------------------------------------------------------
template <class SAMPLE>
int CIwChannelPCM<SAMPLE>::GenerateAudioFast(int16* aTarget, int aLength, int relative_rate, int volume, int mix, int* endOfSample)
{

    if (!aLength)
        return mix;
    {
    int16 * _origTarget    = aTarget;
    SAMPLE * _pos           = pos                       ;
//  SAMPLE * _end_pos       = end_pos                   ;
    int     _offset         = offset                    ;

    SAMPLE * endPosMinus1 = end_pos - 1;
    register uint32 mask = 0x0FFF;
    if (mix)
    {
checkfailedmix:
        while (_pos  == endPosMinus1)
        {
            if (aLength)
            {
                int32 a = _pos[0];
                int32 b = _pos[0];
                int32 curntsamp, samp;
                curntsamp = *aTarget;
                samp = a + ((b-a)*_offset >> 12);
                //*aTarget++ = (int16) (curntsamp + ((samp * volume)>>8));
                *aTarget++ = (int16) clip_to_int16(curntsamp + ((samp * volume)>>8));

                _offset += relative_rate;
                _pos += _offset>>12;
                _offset &= mask;
                aLength--;
            }
            else
                goto myreturn;
        }
        if (_pos > endPosMinus1)
        {
            if (1) //--num_loops == 0)
            {
                *endOfSample=1;
                goto myreturn;
            }
        }
        if (!aLength)
            goto myreturn;

        if (volume)
        {
            do{
            int a = _pos[0];
            int b = _pos[1];
            int curntsamp = *aTarget;
            int samp = a + ((b-a)*_offset >> 12);

            //*aTarget++ = (int16) (curntsamp + ((samp * volume)>>8));
            *aTarget++ = (int16) clip_to_int16(curntsamp + ((samp * volume)>>8));

            _offset += relative_rate;
            _pos += _offset>>12;
            _offset &= mask;
            aLength--;

            if (_pos >= endPosMinus1)
                goto checkfailedmix;

            }while (aLength);
        }
        else
        { // volume == 0, mix
            do{
                aTarget++;
                _offset += relative_rate;
                _pos += _offset>>12;
                _offset &= mask;
                aLength--;
                if (_pos >= endPosMinus1)
                    goto checkfailedmix;
            }while (aLength);
        }
    }
    else
    {
checkfailednomix:
        while (_pos  == endPosMinus1)
        {
            if (aLength)
            {
                int32 a = _pos[0];
                int32 b = _pos[0];
                int32 samp;
                samp = a + ((b-a)*_offset >> 12);

                *aTarget++ = (int16)  ((samp * volume)>>8);
                _offset += relative_rate;
                _pos += _offset>>12;
                _offset &= mask;
                aLength--;
            }
            else
                goto myreturn;
        }
        if (_pos > endPosMinus1)
        {
            *endOfSample=1;
            goto myreturn;
        }
        if (!aLength)
            goto myreturn;

        if (volume)
        {
            do{
                int a = _pos[0];
                int b = _pos[1];
                int samp = a + ((b-a)*_offset >> 12);

                *aTarget++ = (int16) ((samp * volume)>>8);

                _offset += relative_rate;
                _pos += _offset>>12;
                _offset &= mask;
                aLength--;

                if (_pos >= endPosMinus1)
                    goto checkfailednomix;

            }while (aLength);
        }
        else
        { // volume==0, no mix
            do{
                *aTarget++ = 0;
                _offset += relative_rate;
                _pos += _offset>>12;
                _offset &= mask;
                aLength--;
                if (_pos >= endPosMinus1)
                    goto checkfailednomix;
            }while (aLength);
        }
    }

myreturn:

    pos     =   _pos;
    offset  =   _offset;


    return aTarget-_origTarget;
    }
}
//-------------------------------------------------------------------------

#endif /* !IW_SOUND_PCM_H */
