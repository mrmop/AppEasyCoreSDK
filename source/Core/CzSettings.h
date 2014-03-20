// 
//
// AppEasy SDK - Cross Platform Multi-purpose Game and App Engine
//
// Developed by Matthew Hopwood of Pocketeers Limited - www.pocketeers.co.uk
//
// For updates, tutorials and more details check out www.appeasymobile.com
//
// This code is provided free of charge and without any warranty whatsoever. You must ensure that this whole notice is present in all files and derivatives, so the reader knows its origin.
// If you use this SDK in your product then please ensure that you credit AppEasy's usage appropriately. Please see www.appeasymobile.com for licensing details and support
//
//

#if !defined(_CZ_SETTINGS_H_)
#define _CZ_SETTINGS_H_

#include "CzUtil.h"
#include "CzString.h"

/**
 @addtogroup Core
 @{
 */


#define	CZ_SETTINGS	CzSettings::getInstance()

/**
 @class	CzSettings

 @brief	CzSettings is a settings class that controls various settings of the AppEasy engine.

 CzSettings::Init() is called from CzApp::Init(), which loads optional settings from the AppEasy.xml settings file.

 */

class CzSettings
{
public:
	CDEFINE_SINGLETON(CzSettings)

protected:
	// Properties
	bool		PreventPowerSaving;				///< Enable / disable power saving
	bool		DencryptSource;					///< Enable / disable source decryption
	CzString	FacebookAppID;					///< Facebook App ID

public:
	bool			getPreventPowerSaving() const				{ return PreventPowerSaving; }
	void			setPreventPowerSaving(bool enable)			{ PreventPowerSaving = enable; }
	const CzString&	getFacebookAppID() const					{ return FacebookAppID; }
	void			setFacebookAppID(const char* facebook_id)	{ FacebookAppID = facebook_id; }
	bool			getDencryptSource() const					{ return DencryptSource; }
	// Properties end

protected:

public:
	int		Init();
	void	Release();

};

/// @}

#endif // _CZ_SETTINGS_H_
