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
#ifndef IW_SOUNDDATA_H
#define IW_SOUNDDATA_H

// Includes
#include "IwResource.h"
#include "IwSound.h"

// Forward Declarations

//-----------------------------------------------------------------------------
// CIwSoundData
//-----------------------------------------------------------------------------
//! Data for a sound
class CIwSoundData : public CIwResource
{
    //-------------------------------------------------------------------------
    // Public API
    //-------------------------------------------------------------------------
public:

    IW_MANAGED_DECLARE(CIwSoundData);

    CIwSoundData();
    CIwSoundData(IwSoundDataFormat format, uint32 pitch) ;

    ~CIwSoundData();

    //! Get/Set the number of samples in the data (this is independent of the buffer size)
    void SetSampleCount(uint32 nSamples) { m_SampleCount = nSamples; }
    uint32 GetSampleCount() { return m_SampleCount; }

    //! Get/Set the size of the buffer in bytes
    uint32 GetBufferSize() const;
    //! Setting the buffer size will also set the sample count
    void SetBufferSize(uint32 nBytes);

    //! Get properly typed access to the data
    void GetData(CIwArray<int16>& array) const  { array.Share((int16*)m_Samples, GetBufferSize()/2, GetBufferSize()/2); }
    void GetData(CIwArray<int8>& array) const   { array.Share(m_Samples, GetBufferSize(), GetBufferSize()); }

    // get the pitch
    uint32 GetRecPitch() const { return m_RecPitch; }

    //! Get the size of the buffer in bytes
    //! Convert unsigned data into signed data
    void SwitchDataSign();

    virtual void Serialise();

    //-------------------------------------------------------------------------
    // Private API
    //-------------------------------------------------------------------------
protected:
    uint32              m_BufferSize;       //!< Size of the data buffer (may not directly correspond to number of samples with compressed audio)
    uint32              m_SampleCount;      //!< Number of samples
    int8*               m_Samples;          //!< The raw sample data
    uint32              m_RecPitch;         //!< Pitch (Hz) this sample was recorded at
    IwSoundDataFormat   m_Format;           //!< Data format

    // Friends
    friend class CIwSoundSpec;
    friend class CIwResHandlerWAV;
    friend class CIwSoundWAV;
    friend class CIwSoundDataADPCM;
};


//-----------------------------------------------------------------------------
// CIwSoundDataADPCM
//-----------------------------------------------------------------------------
//! Stores additional data for ADPCM
class CIwSoundDataADPCM : public CIwSoundData
{
public:
    IW_MANAGED_DECLARE(CIwSoundDataADPCM);

    CIwSoundDataADPCM();

    CIwSoundDataADPCM(IwSoundDataFormat format, uint32 pitch, uint32 samplesPerBlock, uint32 blockAlign);

    virtual void Serialise();

    uint32  GetSamplesPerBlock() { return m_SamplesPerBlock; }

    uint32  GetBlockAlignment()  { return m_BlockAlign; }


protected:
    uint32              m_SamplesPerBlock;      //!< Number of samples in each adpcm block
    uint32              m_BlockAlign;           //!< Pitch between blocks

};

#endif
