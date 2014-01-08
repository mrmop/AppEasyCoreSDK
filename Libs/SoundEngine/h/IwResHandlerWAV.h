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
 * Resource handler for .wav files.
 */
#ifndef IW_RESHANDLERWAV_H
#define IW_RESHANDLERWAV_H

// Includes
#include "IwArray.h"
#include "IwResManager.h"

// Format tags
#define WAVE_FORMAT_UNKNOWN 0x0000
#define WAVE_FORMAT_PCM     0x0001
#define WAVE_FORMAT_ADPCM   0x0002

// Data structures
typedef struct IwRIFFHeader
{
    char    typeID[4];          //!< ID string - Expect "RIFF"
    uint32  length;             //!< Length of data after this field (i.e. including rest of this struct)
    char    subTypeID[4];       //!< Sub-type ID string - Expect "WAVE"
} IwRIFFHeader;

typedef struct IwRIFFChunkHeader
{
    char    typeID[4];          //!< ID string - Either "fmt " or "data"
    uint32  length;             //!< Length of data after this field
} IwRIFFChunkHeader;

typedef struct IwWAVEFormatChunk
{
    uint16  formatTag;          //!< Format category
    uint16  channels;           //!< Number of channels
    uint32  samplesPerSec;      //!< Sampling rate
    uint32  avgBytesPerSec;     //!< For buffer estimation
    uint16  blockAlign;         //!< Data block size
    uint16  bitsPerSample;      //!< Bits per sample - Should be 8 or 16
} IwWAVEFormatChunk;


typedef struct IwWAVEFormatChunkADPCM
{
    uint16  formatTag;          //!< Format category
    uint16  channels;           //!< Number of channels
    uint32  samplesPerSec;      //!< Sampling rate
    uint32  avgBytesPerSec;     //!< For buffer estimation
    uint16  blockAlign;         //!< Data block size
    uint16  bitsPerSample;      //!< Bits per sample - Should be 8 or 16
    uint16  cbSize;             //!< The size in bytes of the extra information in the extended WAVE 'fmt' header. This should be 2 for adpcm.
    uint16  samplesPerBlock;    //!< The number of samples in each adpcm block
} IwWAVEFormatChunkADPCM;




// Forward declarations
class CIwSoundData;
class CIwResource;

//-------------------------------------------------------------------------
// CIwResHandlerWAV
//-------------------------------------------------------------------------

//! Resource handler for .wav files.
class CIwResHandlerWAV : public CIwResHandler
{
public:
    IW_MANAGED_DECLARE(CIwResHandlerWAV);
    ;
    // Methods
    CIwResHandlerWAV();
    virtual CIwResource*    Build(const CIwStringL& pathname);

    // Members
    CIwStringL  m_Pathname;     //!< Full pathname of current file

    // Static members
    static  const char* g_FlagNames[];

private:
    // Helper functions to split up file reading. Return false if an error occurs.
    bool ReadChunkHeader(IwRIFFChunkHeader& header, s3eFile& file);
    bool ReadChunkFormat(const IwRIFFChunkHeader& header, CIwSoundData*& pData, s3eFile& file);
    bool ReadChunkData(const IwRIFFChunkHeader& header, CIwSoundData*& pData, s3eFile& file);
    bool ReadChunkFact(const IwRIFFChunkHeader& header, CIwSoundData*& pData, s3eFile& file);
};

#endif
