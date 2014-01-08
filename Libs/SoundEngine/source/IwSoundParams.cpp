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
#include "IwSoundParams.h"

// Forward Declarations

//-----------------------------------------------------------------------------
// CIwSoundParams
//-----------------------------------------------------------------------------
CIwSoundParams::CIwSoundParams()
{
    IW_CALLSTACK("CIwSoundParams::CIwSoundParams")

    m_Vol   = IW_GEOM_ONE;
    m_Pan   = 0;
    m_Pitch = IW_GEOM_ONE;
}
