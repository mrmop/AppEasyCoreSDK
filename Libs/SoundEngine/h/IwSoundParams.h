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
 * Parameters to modify the default playing parameters for a sound.
 */
#ifndef IW_SOUNDPARAMS_H
#define IW_SOUNDPARAMS_H

// Includes
#include "IwManaged.h"
#include "IwGeomCore.h"

// Forward Declarations

//-----------------------------------------------------------------------------
// CIwSoundParams
//-----------------------------------------------------------------------------
//! Parameters to modify the default playing parameters for a sound.
class CIwSoundParams
{
public:
    CIwSoundParams();

    // Members
    iwsfixed    m_Vol;
    iwsfixed    m_Pan;
    iwsfixed    m_Pitch;
    uint16      m_Pad;
};

#endif
