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

#include "CzUtil.h"
#include "CzSettings.h"
#include "CzXml.h"
#include "CzDecrypt.h"

CDECLARE_SINGLETON(CzSettings)

//
//
//
// CzSettings implementation
//
//
//
int CzSettings::Init()
{
	PreventPowerSaving = false;

	CzXmlParser*	xml = new CzXmlParser();
	if (xml->Parse("AppEasy.xml") == XmlErrorNone)
	{
		CzXmlNode* root = xml->getRoot()->getFirstNode();
		if (root != NULL)
		{
			// Get debug trace info level
			CzXmlNode* node = root->getFirstNamedNode(CZ_HASH("debug"));
			if (node != NULL)
			{
				CzXmlAttribute* attrib = node->getAttribute(CZ_HASH("level"));
				if (attrib != NULL)
				{
					g_CzShowTracing = attrib->getValueAsInt();
					CzDebug::Log(CZ_DEBUG_CHANNEL_INFO, "Debugging trace level set to ", CzString(g_CzShowTracing).c_str());
				}
			}
			// Get Facebook App ID
			node = root->getFirstNamedNode(CZ_HASH("facebook"));
			if (node != NULL)
			{
				CzXmlAttribute* attrib = node->getAttribute(CZ_HASH("app_id"));
				if (attrib != NULL)
				{
					FacebookAppID = attrib->getValue();
					CzDebug::Log(CZ_DEBUG_CHANNEL_INFO, "Got Facebook App ID");
				}
			}
			// Get source decryption
			node = root->getFirstNamedNode(CZ_HASH("decrypt"));
			if (node != NULL)
			{
				CzXmlAttribute* attrib = node->getAttribute(CZ_HASH("enable"));
				if (attrib != NULL)
				{
					CzDecrypt::Enabled = attrib->getValueAsBool();
					CzDebug::Log(CZ_DEBUG_CHANNEL_INFO, "Decrypt source set to ", CzString(CzDecrypt::Enabled).c_str());
				}
			}
		}
	}

	delete xml;

	return 1;
}

void CzSettings::Release()
{
}


