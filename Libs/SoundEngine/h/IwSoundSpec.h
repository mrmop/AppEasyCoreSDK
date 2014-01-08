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
#ifndef IW_SOUNDSPEC_H
#define IW_SOUNDSPEC_H

// Includes
#include "IwGeomCore.h"     // for iwsfixed
#include "IwResource.h"

// Forward Declarations
class CIwSoundData;
class CIwSoundGroup;
class CIwSoundInst;
class CIwSoundParams;
class CIwTextParserITX;
struct s3eSoundEndSampleInfo;

//-----------------------------------------------------------------------------
// CIwSoundSpec
//-----------------------------------------------------------------------------
//! Sound description.
class CIwSoundSpec : public CIwResource
{
public:
    enum
    {
        LOOPED_F    = (1 << 0), //!< sound should be played as looped
    };

    IW_MANAGED_DECLARE(CIwSoundSpec);
    CIwSoundSpec();
    ~CIwSoundSpec();
    virtual void Serialise();
    virtual void Resolve();
    virtual bool ParseAttribute(CIwTextParserITX* pParser, const char* pAttrName);
    virtual void ParseClose(CIwTextParserITX* pParser);

    //! Play the sound
    CIwSoundInst* Play(const CIwSoundParams* pParams = NULL);

    //! Get the group
    inline CIwSoundGroup* GetGroup() const {return m_Group;}

    //! Set the group
    inline void SetGroup(CIwSoundGroup* pGroup) {m_Group = pGroup;}

    //! Get the audio data
    inline const CIwSoundData* GetData() const {return m_Data;}

    //! Set the audio data
    inline void SetData(const CIwSoundData* pData)
    {
        // TODO: ASSERT IF PLAYING
        m_Data = pData;
    }

	void	setLooped(bool looped)	{ if (looped) m_Flags |= LOOPED_F; else m_Flags &= (0xffff - LOOPED_F); }

    //! Is the sound played as looped?
    inline bool IsLooped() const {return (m_Flags & LOOPED_F) != 0;}

    inline iwsfixed GetVolMin() const { return m_VolMin; }
    inline iwsfixed GetVolMax() const { return m_VolMax; }
    inline iwsfixed GetPanMin() const { return m_PanMin; }
    inline iwsfixed GetPanMax() const { return m_PanMax; }
    inline iwsfixed GetPitchMin() const { return m_PitchMin; }
    inline iwsfixed GetPitchMax() const { return m_PitchMax; }

    //! An extreme way of viewing the stored sound data. Traces to the console.
    //! Useful for checking the values fall within a reasonable range.
    void        Trace();

    // Members
private:
    uint16      m_Flags;
    uint16      pad;
    iwsfixed    m_VolMin;   //!< min volume to play at
    iwsfixed    m_VolMax;   //!< min volume to play at
    iwsfixed    m_PanMin;   //!< min pan to play at
    iwsfixed    m_PanMax;   //!< min pan to play at
    iwsfixed    m_PitchMin; //!< min pitch to play at
    iwsfixed    m_PitchMax; //!< min pitch to play at

    const CIwSoundData* m_Data; //!< raw audio data
    CIwSoundGroup*  m_Group;    //!< group we are in, or NULL

    // Friends
    friend int _IwSoundSpec_EndSampleCB(s3eSoundEndSampleInfo* esInfo, void* userData);
};

int _IwSoundSpec_EndSampleCB(s3eSoundEndSampleInfo* esInfo, void* userData);

#endif
