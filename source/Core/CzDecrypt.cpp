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

#include "CzDecrypt.h"

//
//
//
//
// CzDecrypt implementation
//
//
//
//

unsigned char* CzDecrypt::DecryptKey = (unsigned char*)"add_your_own_key_here!";
bool CzDecrypt::Enabled = false;

void CzDecrypt::Decrypt(unsigned char *buffer, int length, unsigned char* key, int key_length)
{
	char* old = (char*)buffer;
	int index = 0;
	for (int t = 0; t < length; t++)
	{
		*buffer = *buffer ^ key[index];
		index++;
		if (index >= key_length)
			index = 0;
		buffer++;
	}
}



