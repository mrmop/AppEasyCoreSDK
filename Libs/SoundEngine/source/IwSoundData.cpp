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
 * Audio data resource.
 */

#include "IwSoundData.h"

// Forward Declarations

//-----------------------------------------------------------------------------
// CIwSoundData
//-----------------------------------------------------------------------------
IW_CLASS_FACTORY(CIwSoundData);
IW_MANAGED_IMPLEMENT(CIwSoundData)
CIwSoundData::CIwSoundData()
:
  m_SampleCount(0),
  m_Samples(NULL),
  m_RecPitch(44100),
  m_Format(PCM_16BIT_MONO),
  m_BufferSize(0)
{
}
//-----------------------------------------------------------------------------
CIwSoundData::CIwSoundData(IwSoundDataFormat format, uint32 pitch)
:
  m_SampleCount(0),
  m_Samples(NULL),
  m_RecPitch(pitch),
  m_Format(format),
  m_BufferSize(0)
{
}
//-----------------------------------------------------------------------------
CIwSoundData::~CIwSoundData()
{
    delete [] m_Samples;
}
//-----------------------------------------------------------------------------
void CIwSoundData::SetBufferSize(uint32 nBytes)
{
    // Do nothing if not changing
    if (nBytes == m_BufferSize && m_Samples)
        return;

    if (m_Samples)
        delete m_Samples;

    // Create the buffer
    m_SampleCount = nBytes;

    switch (m_Format)
    {
    case ADPCM_MONO:
        m_SampleCount = nBytes*2;
        break;

    case PCM_8BIT_MONO:
        m_SampleCount = nBytes;
        break;
    default:                // Default to 16-bit
    case PCM_16BIT_MONO:
        m_SampleCount = nBytes/2;
        break;
    };

    m_BufferSize = nBytes;
    m_Samples = new int8[m_BufferSize];
}
//-----------------------------------------------------------------------------
uint32 CIwSoundData::GetBufferSize() const
{
    return m_BufferSize;
}
//-----------------------------------------------------------------------------
void CIwSoundData::SwitchDataSign()
{
    CIwArray<int8> array;
    GetData(array);
    for (uint32 i=0; i < array.size(); i++)
        array[i] += (int8) 0x80;
}
//-----------------------------------------------------------------------------
void CIwSoundData::Serialise()
{
    IW_CALLSTACK("CIwSoundData::Serialise")

    CIwManaged::Serialise();

    IwSerialiseUInt32(m_BufferSize);
    SetBufferSize(m_BufferSize);

    IwSerialiseUInt32(m_SampleCount);
    IwSerialiseInt8(*m_Samples, GetBufferSize());

    IwSerialiseUInt32(m_RecPitch);
    IwSerialiseUInt16((uint16&)m_Format);
}
// CIwSoundDataADPCM
//-----------------------------------------------------------------------------
IW_CLASS_FACTORY(CIwSoundDataADPCM);
//-----------------------------------------------------------------------------
IW_MANAGED_IMPLEMENT(CIwSoundDataADPCM)
CIwSoundDataADPCM::CIwSoundDataADPCM() :
    m_SamplesPerBlock(0),
    m_BlockAlign(0)
{
}
//-----------------------------------------------------------------------------
CIwSoundDataADPCM::CIwSoundDataADPCM(IwSoundDataFormat format, uint32 pitch, uint32 samplesPerBlock, uint32 blockAlign)
:
    CIwSoundData(format, pitch),
    m_SamplesPerBlock(samplesPerBlock),
    m_BlockAlign(blockAlign)
{
}
//-----------------------------------------------------------------------------
void CIwSoundDataADPCM::Serialise()
{
    CIwSoundData::Serialise();
    IwSerialiseUInt32(m_SamplesPerBlock);
    IwSerialiseUInt32(m_BlockAlign);

    // Convert ADPCM to PCM and attach an appropriate WAV header
    // Note: Very hacky way of checking for requirement to do this!
    if (g_IwSerialiseContext.read && s3eSoundGetInt(S3E_SOUND_AVAILABLE) == 123)
    {
        if (m_Format == ADPCM_MONO)
        {
            const int WAV_HEADER_SIZE = 44;

            int8* newData = new int8[GetSampleCount()*2 + WAV_HEADER_SIZE];

            uint8* wavHeader = (uint8*)newData;
            int16* samples = (int16*)(wavHeader + WAV_HEADER_SIZE);

            CIwChannelADPCM tempADPCM;
            s3eSoundGenAudioInfo tempInfo;

            tempInfo.m_Channel = 0;
            tempInfo.m_EndSample = S3E_FALSE;
            tempInfo.m_Mix = 0;
            tempInfo.m_NumSamples = GetSampleCount();
            tempInfo.m_OrigNumSamples = GetBufferSize()/2;
            tempInfo.m_OrigRepeat = 0;
            tempInfo.m_OrigStart = (int16*)m_Samples;
            tempInfo.m_Target = samples;

            s3eSoundChannelStop(tempInfo.m_Channel);

            s3eSoundChannelSetInt(tempInfo.m_Channel, S3E_CHANNEL_USERVAR, (intptr_t)this);
            s3eSoundChannelSetInt(tempInfo.m_Channel, S3E_CHANNEL_RATE, s3eSoundGetInt(S3E_SOUND_OUTPUT_FREQ));
            s3eSoundChannelSetInt(tempInfo.m_Channel, S3E_CHANNEL_VOLUME, S3E_SOUND_MAX_VOLUME);

            int i = tempADPCM.GenerateAudio(&tempInfo);

            // Contract buffer size to exact length (earlier estimate of size is inaccurate)
            int newSampleCount = i;
            int newBufferSize = i * 2 + WAV_HEADER_SIZE;

            // Build WAV header.
            {
                char buffer[WAV_HEADER_SIZE+1] = "RIFF\xa4\xfc\0\0WAVEfmt \x10\0\0\0\x01\0\x01\0\x40\x1f\0\0\x80\x3e\0\0\x02\0\x10\0\x64\x61\x74\x61\x80\xfc\0\0";
                int samplerate = m_RecPitch;
                int datasize = 2 * newSampleCount;

                memcpy(wavHeader, buffer, WAV_HEADER_SIZE);

                int size = datasize + 36;
                wavHeader[4] = (size    ) & 0xff;
                wavHeader[5] = (size>> 8) & 0xff;
                wavHeader[6] = (size>>16) & 0xff;
                wavHeader[7] = (size>>24) & 0xff;

                wavHeader[34] = 16; //bitspersample;
                wavHeader[22] = 1; //numchannels;

                wavHeader[24] = (samplerate    ) & 0xff;
                wavHeader[25] = (samplerate>> 8) & 0xff;
                wavHeader[26] = (samplerate>>16) & 0xff;
                wavHeader[27] = (samplerate>>24) & 0xff;

                wavHeader[40] = (datasize    ) & 0xff;
                wavHeader[41] = (datasize>> 8) & 0xff;
                wavHeader[42] = (datasize>>16) & 0xff;
                wavHeader[43] = (datasize>>24) & 0xff;

                #if 1 && defined IW_DEBUG && defined I3D_ARCH_X86
                    // For debug: write resulting WAV file out to disk
                    FILE * f = fopen("test.wav", "wb");
                    if (f)
                    {
                        fwrite(wavHeader, 1, newBufferSize, f);
                        fclose(f);
                    }
                #endif
            }

            // Delete old ADPCM data and change members to use new PCM data
            delete [] m_Samples;
            m_Samples = newData;

            m_BufferSize = newBufferSize;
            m_SampleCount = newSampleCount;
            m_Format = PCM_16BIT_MONO;
        }
    }
}
