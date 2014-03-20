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

#if !defined(_CZ_DECRYPT_H_)
#define _CZ_DECRYPT_H_

#include "CzUtil.h"
#include "CzXoml.h"

/**
 @addtogroup Core
 @{
 */
 
/**
 @class	CzDecrypt

 @brief	handles source file decryption

 */

class CzDecrypt
{
public:
	static bool Enabled;
	static unsigned char* DecryptKey;
	static void Decrypt(unsigned char *buffer, int length, unsigned char* key, int key_length);
};

/// @}


#endif // _CZ_DECRYPT_H_