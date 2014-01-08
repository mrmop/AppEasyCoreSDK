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

#ifndef __CZ_FRUSTRUM_H__
#define __CZ_FRUSTRUM_H__

#include "CzVec3.h"
#include "CzMatrix4.h"

class CzFrustrum
{
public:
	float	m_aFrustrum[6][4];
	
public:	
	CzFrustrum()	{}

	void UpdateFrustum(CzMatrix4* pProjection, CzMatrix4 *pModel);
	bool CheckPoint(float x, float y, float z);
	bool CheckPointXZ(float x, float z);
	bool CheckSphere(float x, float y, float z, float radius);
	bool CheckSphereXZ(float x, float z, float radius);
	bool CheckBB(CzVec3* pBB);
};



#endif	// __CZ_FRUSTRUM_H__
