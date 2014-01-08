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
#ifndef IW_SOUNDRWAV_H
#define IW_SOUNDRWAV_H

// Includes
#include "IwResHandlerWAV.h"

//-------------------------------------------------------------------------
// CIwSoundWAV
//-------------------------------------------------------------------------

//! Resource handler for .wav files.
class CIwSoundWAV
{
public:
    // Methods
    CIwSoundWAV();
    static CIwSoundData* Create(const CIwStringL& pathname, void* buffer = NULL, u_int file_size = 0);
    static CIwSoundData* Create(void *data, int data_pen);

private:
    // Helper functions to split up file reading. Return false if an error occurs.
    static bool ReadChunkHeader(IwRIFFChunkHeader& header, s3eFile& file);
    static bool ReadChunkFormat(const CIwStringL& pathname, const IwRIFFChunkHeader& header, CIwSoundData*& pData, s3eFile& file);
    static bool ReadChunkData(const CIwStringL& pathname, const IwRIFFChunkHeader& header, CIwSoundData*& pData, s3eFile& file);
    static bool ReadChunkFact(const CIwStringL& pathname, const IwRIFFChunkHeader& header, CIwSoundData*& pData, s3eFile& file);
};

#endif
