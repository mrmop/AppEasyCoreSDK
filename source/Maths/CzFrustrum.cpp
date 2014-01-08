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

#include "CzFrustrum.h"
#include "CzMath.h"

void CzFrustrum::UpdateFrustum(CzMatrix4* pProjection, CzMatrix4 *pModel)
{
	float*	proj = (float *)pProjection->m;
	float*	modl = (float *)pModel->m;
	float	clip[16];
	float	t;

	// Combine the two matrices (multiply projection by modelview)
	clip[0] = modl[0] * proj[0] + modl[1] * proj[4] + modl[2] * proj[8] + modl[3] * proj[12];
	clip[1] = modl[0] * proj[1] + modl[1] * proj[5] + modl[2] * proj[9] + modl[3] * proj[13];
	clip[2] = modl[0] * proj[2] + modl[1] * proj[6] + modl[2] * proj[10] + modl[3] * proj[14];
	clip[3] = modl[0] * proj[3] + modl[1] * proj[7] + modl[2] * proj[11] + modl[3] * proj[15];
	
	clip[4] = modl[4] * proj[0] + modl[5] * proj[4] + modl[6] * proj[8] + modl[7] * proj[12];
	clip[5] = modl[4] * proj[1] + modl[5] * proj[5] + modl[6] * proj[9] + modl[7] * proj[13];
	clip[6] = modl[4] * proj[2] + modl[5] * proj[6] + modl[6] * proj[10] + modl[7] * proj[14];
	clip[7] = modl[4] * proj[3] + modl[5] * proj[7] + modl[6] * proj[11] + modl[7] * proj[15];
	
	clip[8] = modl[8] * proj[0] + modl[9] * proj[4] + modl[10] * proj[8] + modl[11] * proj[12];
	clip[9] = modl[8] * proj[1] + modl[9] * proj[5] + modl[10] * proj[9] + modl[11] * proj[13];
	clip[10] = modl[8] * proj[2] + modl[9] * proj[6] + modl[10] * proj[10] + modl[11] * proj[14];
	clip[11] = modl[8] * proj[3] + modl[9] * proj[7] + modl[10] * proj[11] + modl[11] * proj[15];
	
	clip[12] = modl[12] * proj[0] + modl[13] * proj[4] + modl[14] * proj[8] + modl[15] * proj[12];
	clip[13] = modl[12] * proj[1] + modl[13] * proj[5] + modl[14] * proj[9] + modl[15] * proj[13];
	clip[14] = modl[12] * proj[2] + modl[13] * proj[6] + modl[14] * proj[10] + modl[15] * proj[14];
	clip[15] = modl[12] * proj[3] + modl[13] * proj[7] + modl[14] * proj[11] + modl[15] * proj[15];
/*CHardware::D_printf("##### MODEL ######\n");
CHardware::D_printf("%2.2f, %2.2f, %2.2f, %2.2f\n", modl[0], modl[1], modl[2], modl[3]);
CHardware::D_printf("%2.2f, %2.2f, %2.2f, %2.2f\n", modl[4], modl[5], modl[6], modl[7]);
CHardware::D_printf("%2.2f, %2.2f, %2.2f, %2.2f\n", modl[8], modl[9], modl[10], modl[11]);
CHardware::D_printf("%2.2f, %2.2f, %2.2f, %2.2f\n", modl[12], modl[13], modl[14], modl[15]);
CHardware::D_printf("##### CLIP ######\n");
CHardware::D_printf("%2.2f, %2.2f, %2.2f, %2.2f\n", clip[0], clip[1], clip[2], clip[3]);
CHardware::D_printf("%2.2f, %2.2f, %2.2f, %2.2f\n", clip[4], clip[5], clip[6], clip[7]);
CHardware::D_printf("%2.2f, %2.2f, %2.2f, %2.2f\n", clip[8], clip[9], clip[10], clip[11]);
CHardware::D_printf("%2.2f, %2.2f, %2.2f, %2.2f\n", clip[12], clip[13], clip[14], clip[15]);
CHardware::D_printf("##### PROJECTION ######\n");
CHardware::D_printf("%2.2f, %2.2f, %2.2f, %2.2f\n", proj[0], proj[1], proj[2], proj[3]);
CHardware::D_printf("%2.2f, %2.2f, %2.2f, %2.2f\n", proj[4], proj[5], proj[6], proj[7]);
CHardware::D_printf("%2.2f, %2.2f, %2.2f, %2.2f\n", proj[8], proj[9], proj[10], proj[11]);
CHardware::D_printf("%2.2f, %2.2f, %2.2f, %2.2f\n", proj[12], proj[13], proj[14], proj[15]);*/
	// Extract the numbers for the RIGHT plane
	m_aFrustrum[0][0] = clip[ 3] - clip[ 0];
	m_aFrustrum[0][1] = clip[ 7] - clip[ 4];
	m_aFrustrum[0][2] = clip[11] - clip[ 8];
	m_aFrustrum[0][3] = clip[15] - clip[12];
	
	// Normalize the result
	t = CzMath::sqrt((float)(m_aFrustrum[0][0] * m_aFrustrum[0][0] + m_aFrustrum[0][1] * m_aFrustrum[0][1] + m_aFrustrum[0][2] * m_aFrustrum[0][2]));
	m_aFrustrum[0][0] /= t;
	m_aFrustrum[0][1] /= t;
	m_aFrustrum[0][2] /= t;
	m_aFrustrum[0][3] /= t;
	
	// Extract the numbers for the LEFT plane
	m_aFrustrum[1][0] = clip[ 3] + clip[ 0];
	m_aFrustrum[1][1] = clip[ 7] + clip[ 4];
	m_aFrustrum[1][2] = clip[11] + clip[ 8];
	m_aFrustrum[1][3] = clip[15] + clip[12];
	
	// Normalize the result
	t = CzMath::sqrt((float)(m_aFrustrum[1][0] * m_aFrustrum[1][0] + m_aFrustrum[1][1] * m_aFrustrum[1][1] + m_aFrustrum[1][2] * m_aFrustrum[1][2]));
	m_aFrustrum[1][0] /= t;
	m_aFrustrum[1][1] /= t;
	m_aFrustrum[1][2] /= t;
	m_aFrustrum[1][3] /= t;
	
	// Extract the BOTTOM plane
	m_aFrustrum[2][0] = clip[ 3] + clip[ 1];
	m_aFrustrum[2][1] = clip[ 7] + clip[ 5];
	m_aFrustrum[2][2] = clip[11] + clip[ 9];
	m_aFrustrum[2][3] = clip[15] + clip[13];
	
	// Normalize the result
	t = CzMath::sqrt((float)(m_aFrustrum[2][0] * m_aFrustrum[2][0] + m_aFrustrum[2][1] * m_aFrustrum[2][1] + m_aFrustrum[2][2] * m_aFrustrum[2][2]));
	m_aFrustrum[2][0] /= t;
	m_aFrustrum[2][1] /= t;
	m_aFrustrum[2][2] /= t;
	m_aFrustrum[2][3] /= t;
	
	// Extract the TOP plane
	m_aFrustrum[3][0] = clip[ 3] - clip[ 1];
	m_aFrustrum[3][1] = clip[ 7] - clip[ 5];
	m_aFrustrum[3][2] = clip[11] - clip[ 9];
	m_aFrustrum[3][3] = clip[15] - clip[13];
	
	// Normalize the result
	t = CzMath::sqrt((float)(m_aFrustrum[3][0] * m_aFrustrum[3][0] + m_aFrustrum[3][1] * m_aFrustrum[3][1] + m_aFrustrum[3][2] * m_aFrustrum[3][2]));
	m_aFrustrum[3][0] /= t;
	m_aFrustrum[3][1] /= t;
	m_aFrustrum[3][2] /= t;
	m_aFrustrum[3][3] /= t;
	
	// Extract the NEAR plane
	m_aFrustrum[4][0] = clip[ 3] + clip[ 2];
	m_aFrustrum[4][1] = clip[ 7] + clip[ 6];
	m_aFrustrum[4][2] = clip[11] + clip[10];
	m_aFrustrum[4][3] = clip[15] + clip[14];
	
	// Normalize the result
	t = CzMath::sqrt((float)(m_aFrustrum[4][0] * m_aFrustrum[4][0] + m_aFrustrum[4][1] * m_aFrustrum[4][1] + m_aFrustrum[4][2] * m_aFrustrum[4][2]));
	m_aFrustrum[4][0] /= t;
	m_aFrustrum[4][1] /= t;
	m_aFrustrum[4][2] /= t;
	m_aFrustrum[4][3] /= t;
	
	// Extract the FAR plane
	m_aFrustrum[5][0] = clip[ 3] - clip[ 2];
	m_aFrustrum[5][1] = clip[ 7] - clip[ 6];
	m_aFrustrum[5][2] = clip[11] - clip[10];
	m_aFrustrum[5][3] = clip[15] - clip[14];
	
	// Normalize the result
	t = CzMath::sqrt((float)(m_aFrustrum[5][0] * m_aFrustrum[5][0] + m_aFrustrum[5][1] * m_aFrustrum[5][1] + m_aFrustrum[5][2] * m_aFrustrum[5][2]));
	m_aFrustrum[5][0] /= t;
	m_aFrustrum[5][1] /= t;
	m_aFrustrum[5][2] /= t;
	m_aFrustrum[5][3] /= t;
}

bool CzFrustrum::CheckPoint(float x, float y, float z)
{
	int t;

	for (t = 0; t < 4; t++)
	{
		float d = m_aFrustrum[t][0] * x + m_aFrustrum[t][1] * y + m_aFrustrum[t][2] * z + m_aFrustrum[t][3];
		if (d < 0)
			return false;
	}
	return true;
}

bool CzFrustrum::CheckPointXZ(float x, float z)
{
	float d;
	
	// Only check against right, left and front frustrum planes

	d = m_aFrustrum[0][0] * x + m_aFrustrum[0][2] * z + m_aFrustrum[0][3];
	if (d < 0)
		return false;
	d = m_aFrustrum[1][0] * x + m_aFrustrum[1][2] * z + m_aFrustrum[1][3];
	if (d < 0)
		return false;
	d = m_aFrustrum[4][0] * x + m_aFrustrum[4][2] * z + m_aFrustrum[4][3];
	if (d < 0)
		return false;
		
	return true;
}

bool CzFrustrum::CheckSphere(float x, float y, float z, float radius)
{
	int t;

	for (t = 0; t < 5; t++)
//	for (t = 0; t < 2; t++)
	{
		float d = m_aFrustrum[t][0] * x + m_aFrustrum[t][1] * y + m_aFrustrum[t][2] * z + m_aFrustrum[t][3];
//System.out.println(vPos.x + " " + vPos.y + " " + vPos.z);
//System.out.println(d);
		if (d <= -radius)
			return false;
	}
	return true;
}

bool CzFrustrum::CheckSphereXZ(float x, float z, float radius)
{
	float d;

	// Only check against right, left and front frustrum planes
	
	d = m_aFrustrum[0][0] * x + m_aFrustrum[0][2] * z + m_aFrustrum[0][3];
	if (d <= -radius)
		return false;
	d = m_aFrustrum[1][0] * x + m_aFrustrum[1][2] * z + m_aFrustrum[1][3];
	if (d <= -radius)
		return false;
	d = m_aFrustrum[4][0] * x + m_aFrustrum[4][2] * z + m_aFrustrum[4][3];
	if (d <= -radius)
		return false;

	return true;
}

bool CzFrustrum::CheckBB(CzVec3* pBB)
{
	if (pBB == NULL)
		return false;
	
	for (int t = 0; t < 8; t++)
	{
		if (CheckPoint(pBB[t].x, pBB[t].y, pBB[t].z))
			return true;
	}
	
	return false;
}

