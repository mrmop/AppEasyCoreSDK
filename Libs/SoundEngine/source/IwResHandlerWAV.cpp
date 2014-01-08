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
#include "IwResHandlerWAV.h"
#include "IwDebug.h"
#include "IwFile.h"
#include "IwHashString.h"
#include "IwRuntime.h"
#include "IwSound.h"
#include "IwSoundData.h"
#include "s3eFile.h"

//-------------------------------------------------------------------------
// CIwResHandlerWAV
//-------------------------------------------------------------------------
IW_MANAGED_IMPLEMENT(CIwResHandlerWAV)
CIwResHandlerWAV::CIwResHandlerWAV() : CIwResHandler("wav", IW_SOUND_RESTYPE_DATA)
{
    SetName("WAV");
}
//-------------------------------------------------------------------------
CIwResource*    CIwResHandlerWAV::Build(const CIwStringL& pathname)
{
    IW_CALLSTACK("CIwResHandlerWAV::Build")

#ifndef IW_DEBUG
    IwAssertMsg(GRAPHICS, false, ("Project not built with IW_DEBUG"));
    return NULL;
#else
    CIwSoundData* pData = NULL; // Object to return

    m_Pathname = pathname;

    // Open file
    s3eFile* pFile = IwFileOpenPrefixed(m_Pathname.c_str(), "rb");
    IwAssertMsg(SOUND, pFile, ("Could not load file %s", m_Pathname.c_str()));
    if (!pFile)
        return NULL;

    // Read RIFF header - Gives the file size and checks that this is a WAVE
    // file as expected
    IwRIFFHeader riffHeader;
    if ((s3eFileRead(&riffHeader, sizeof(IwRIFFHeader), 1, pFile) != 1)
        || (strncmp(riffHeader.typeID, "RIFF", 4) != 0)
        || (strncmp(riffHeader.subTypeID, "WAVE", 4) != 0))
    {
        IwAssertMsg(SOUND, false, ("Invalid header in %s (RIFF Header)", m_Pathname.c_str()));
        s3eFileClose(pFile);
        return NULL;
    }

    // Read in RIFF chunks until we reach the end of the file
    // Read the RIFF chunk header. This tells us what type of chunk follows.
    IwRIFFChunkHeader chunkHeader;
    bool readData = false;
    uint32 fileSize = s3eFileGetSize(pFile);

    while (ReadChunkHeader(chunkHeader, *(s3eFile*)pFile))
    {
        uint32 chunkStartPos = s3eFileTell(pFile);

        // Next action depends on chunk type. The order of this is important and we may fail
        // if an unexpected chunk type is found
        if (!strncmp(chunkHeader.typeID, "fmt ", 4))
        {
            // Read WAVE info chunk
            if (!ReadChunkFormat(chunkHeader, pData, *(s3eFile*)pFile))
            {
                s3eFileClose(pFile);
                return NULL;
            }
        }
        else if (!strncmp(chunkHeader.typeID, "data", 4))
        {
            if (!ReadChunkData(chunkHeader, pData, *(s3eFile*)pFile))
            {
                s3eFileClose(pFile);
                return NULL;
            }
            readData = true;
        }
        else if (!strncmp(chunkHeader.typeID, "fact", 4))
        {
            if (!ReadChunkFact(chunkHeader, pData, *(s3eFile*)pFile))
            {
                s3eFileClose(pFile);
                return NULL;
            }
        }
        else
        {
            // Unknown chunk type
            // Make a proper string from the chunk type info
            char typeID[5];
            strncpy(typeID, chunkHeader.typeID, 4);
            typeID[4] = 0;  // Terminate

            const char* g_IgnoreTypes = "LIST" //LIST is just copyright info etc.
                "DISP";  //DISP seems to be info about what package exported it

            IwAssertMsg(SOUND, strstr(g_IgnoreTypes, typeID), ("Unhandled chunk type '%s' in %s. Ignoring this data.", typeID, m_Pathname.c_str()));
        }

        // Exit if at end of file
        if (chunkStartPos + chunkHeader.length >= fileSize)
            break;

        // Move to next chunk
        s3eFileSeek(pFile, chunkStartPos + chunkHeader.length, S3E_FILESEEK_SET);
    }

    // Check that we have read the sample data
    IwAssertMsg(SOUND, readData, ("No data chunk read in %s", m_Pathname.c_str()));
    s3eFileClose(pFile);
    return pData;
#endif
}
//-------------------------------------------------------------------------
bool CIwResHandlerWAV::ReadChunkHeader(IwRIFFChunkHeader& header, s3eFile& file)
{
    return (s3eFileRead(&header, sizeof(IwRIFFChunkHeader), 1, &file) == 1);
}
//-------------------------------------------------------------------------
bool CIwResHandlerWAV::ReadChunkFormat(const IwRIFFChunkHeader& header, CIwSoundData*& pData, s3eFile& file)
{
    IwWAVEFormatChunkADPCM format;

    // Read data from file
    if ((header.length < sizeof(IwWAVEFormatChunk))
        || (s3eFileRead(&format, sizeof(IwWAVEFormatChunkADPCM), 1, &file) != 1))
    {
        IwAssertMsg(SOUND, false, ("Invalid format chunk in WAV file"));
        return false;
    }

    IwAssertMsg(SOUND, format.channels == 1, ("%s has more than 1 channel. IwSound is mono only", m_Pathname.c_str()));

    // Create object based on header info
    switch (format.formatTag)
    {
    default:    // Unsupported format - Exit without creating data
        IwAssertMsg(SOUND, false, ("Unsupported WAVE file format (%d)", format.formatTag));
        return false;
        break;

    case WAVE_FORMAT_PCM:   // Raw PCM data. Bits per sample can be 8 or 16
        // Bits per sample will have been read into pad bytes of format info
        switch (format.bitsPerSample)
        {
        case 8:     // 8 bps
            pData = new CIwSoundData(PCM_8BIT_MONO, format.samplesPerSec);
            break;

        case 16:    // 16 bps
            pData = new CIwSoundData(PCM_16BIT_MONO, format.samplesPerSec);
            break;

        default:    // Unhandled
            IwAssertMsg(SOUND, false, ("Unsupported bits-per-sample (%d)", format.bitsPerSample));
            return false;
            break;
        }
        break;

    case 17:
            // ADPCM compressed data
            pData = new CIwSoundDataADPCM(ADPCM_MONO, format.samplesPerSec,
                ((IwWAVEFormatChunkADPCM*) &format)->samplesPerBlock,
                    ((IwWAVEFormatChunkADPCM*) &format)->blockAlign);
            break;
    }

    return true;
}
//-------------------------------------------------------------------------
bool CIwResHandlerWAV::ReadChunkData(const IwRIFFChunkHeader& header, CIwSoundData*& pData, s3eFile& file)
{
    // Check that we have already read a format chunk and the object has been created
    if (!pData)
    {
        IwAssertMsg(SOUND, false, ("Data chunk encountered before format chunk in %s", m_Pathname.c_str()));
        return false;
    }

    // If the number of samples has already been set then check we're not changing it
#ifdef IW_USE_ASSERTIONS
    uint32 currentBufSiz = pData->GetBufferSize();


    IwAssertMsg(SOUND, (currentBufSiz ==  0) || (header.length == currentBufSiz),
                        ("Unexpected data size in %s", m_Pathname.c_str()));
#endif

    // Create the data array
    pData->SetBufferSize(header.length);

    // Read in the actual data. This can be read straight into the array independent of BPS
    if (s3eFileRead(&pData->m_Samples[0], 1, header.length, &file) != header.length)
    {
        IwAssertMsg(SOUND, false, ("Error reading WAVE file data from %s", m_Pathname.c_str()));
        return false;
    }

    // DJB - Our 8-bit samples seem to be saved as unsigned data. Don't why this is or a way
    // to distinguish signed from unsigned.
    // Transform all 8-bit samples for now
    // DP - wav files are 8-bit unsigned and 16-bit signed, thats just the (random) way they are
    if (pData->m_Format == PCM_8BIT_MONO)
        pData->SwitchDataSign();

    return true;
}
//-------------------------------------------------------------------------
bool CIwResHandlerWAV::ReadChunkFact(const IwRIFFChunkHeader& header, CIwSoundData*& pData, s3eFile& file)
{
    uint32 sampleCount;
    if (s3eFileRead(&sampleCount, sizeof(uint32), 1, &file) != 1)
    {
        IwAssertMsg(SOUND, false, ("Error reading WAVE file info from %s", m_Pathname.c_str()));
        return false;
    }

    pData->SetSampleCount(sampleCount);
    return true;
}
