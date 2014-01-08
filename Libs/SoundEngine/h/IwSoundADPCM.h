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
 * Support for ADPCM compressed samples
 */
#ifndef IW_SOUNDADPCM_H
#define IW_SOUNDADPCM_H

// Includes
#include "s3eTypes.h"
#include "s3eSound.h"

#define AUDIO_FRAMESIZE 160

struct adpcm_state {
    short valprev;      /* Previous output value */
    unsigned char index;    /* Index into stepsize table */
    unsigned char bufferstep; /*was pad, but DP: I added this */
};

class CIwChannelADPCM
{
    public:
    int16 *     pos             ;
    int16 *     end_pos         ;
    int         bres_accum      ;
    int         count           ;
    int16       s_1             ;
    int16       pad             ;
    int16       s_2             ;
    int8        index;          /* Index into stepsize table */
    uint8       bufferstep;     /* which 4 bits of the current byte we want*/

    int32        m_SamplesPerBlock;
    int32        m_BlockAlign ;
    int32        m_SamplesRemaining;
    adpcm_state* m_LastBlock;

    int32       paused;



    uint8 m_FrameBuf[AUDIO_FRAMESIZE*8];

    CIwChannelADPCM()
    {
        pos = 0;
        if (isInitialized == 0)
            Init();
    }


    static int GenerateAudioCB(s3eSoundGenAudioInfo* pSystemData, CIwChannelADPCM* pUserData)
    {
        return pUserData->GenerateAudio(pSystemData);
    }

    int GenerateAudio(s3eSoundGenAudioInfo* pInfo);

    int GenerateADPCMAudioFast(int16* aTarget, int aLength, int relative_rate, int volume, int mix, int* endOfSample);

    int16   GetNextSample ();

    static  int isInitialized;
    static  void Init();
};

#endif /* !IW_SOUNDADPCM_H */
