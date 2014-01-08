/*
  Copyright 2005 Pocketeers Limited.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Pocketeers Limited. They may not be 
  disclosed to third parties or copied or duplicated in any form,in 
  whole or in part, without the prior written consent of Pocketeers 
  Limited
*/

/*---------------------------------------------------------------------------
 *
 * $Workfile: CzMatrix3.cpp $
 *
 *---------------------------------------------------------------------------
 */


#include <string.h>

#include "CzMatrix3.h"
#include "CzMath.h"
#include "CzVec2.h"

#define CZA0	0
#define CZA1	1
#define CZA2	2
#define CZA3	0
#define CZA4	1
#define CZA5	2
#define CZA6	0
#define CZA7	1
#define CZA8	2

#define CZB0	0
#define CZB1	0
#define CZB2	0
#define CZB3	1
#define CZB4	1
#define CZB5	1
#define CZB6	2
#define CZB7	2
#define CZB8	2


CzMatrix3::CzMatrix3()
{
	Clear();
}	
CzMatrix3::CzMatrix3(CzMatrix3* m0)
{
	Copy(m0);
}

void CzMatrix3::Clear()
{
	memset(&m, 0, sizeof(m));
	m[CZA0][CZB0] = 1.0f;
	m[CZA4][CZB4] = 1.0f;
	m[CZA8][CZB8] = 1.0f;
}

void CzMatrix3::Copy(CzMatrix3* m0)
{
	memcpy(&m, &m0->m, sizeof(m));
}

void CzMatrix3::Invert()
{
	float tx = m[CZA2][CZB2];
	float ty = m[CZA5][CZB5];

	Transpose();

	m[CZA2][CZB2] = -tx;
	m[CZA5][CZB5] = -ty;
}

void CzMatrix3::Transpose()
{
	CzMatrix3 m0 = *this;

	m[CZA0][CZB0] = m0.m[CZA0][CZB0];
	m[CZA1][CZB1] = m0.m[CZA3][CZB3];
	m[CZA3][CZB3] = m0.m[CZA1][CZB1];
	m[CZA4][CZB4] = m0.m[CZA4][CZB4];
	m[CZA6][CZB6] = m0.m[CZA2][CZB2];
	m[CZA7][CZB7] = m0.m[CZA5][CZB5];
}

void CzMatrix3::Identity()
{
	Clear();
	m[CZA0][CZB0] = 1.0f;
	m[CZA4][CZB4] = 1.0f;
	m[CZA8][CZB8] = 1.0f;
}

void CzMatrix3::Translate(float x, float y)
{
	Clear();
	m[CZA0][CZB0] = 1.0f;
	m[CZA4][CZB4] = 1.0f;
	m[CZA2][CZB2] = x;
	m[CZA5][CZB5] = y;
}

void CzMatrix3::Translate(CzVec2* v)
{
	Translate(v->x, v->y);
}

void CzMatrix3::Scale(float x, float y)
{
	Clear();
	m[CZA0][CZB0] = x;
	m[CZA4][CZB4] = y;
}

void CzMatrix3::ScaleRotation(float x, float y)
{
	m[CZA0][CZB0] *= x;
	m[CZA1][CZB1] *= x;
	m[CZA4][CZB4] *= y;
	m[CZA3][CZB3] *= y;
}

void CzMatrix3::Scale(CzVec2* v)
{
	Scale(v->x, v->y);
}

void CzMatrix3::TranslateSet(float x, float y)
{
	m[CZA2][CZB2] = x;
	m[CZA5][CZB5] = y;
}

void CzMatrix3::TranslateSet(CzVec2* v)
{
	m[CZA2][CZB2] = v->x;
	m[CZA5][CZB5] = v->y;
}

void CzMatrix3::Rotate(float angle)
{
	angle = CzMath::DegToRad(angle);
	float c = CzMath::cos(angle);
	float s = CzMath::sin(angle);

	Clear();
	m[CZA0][CZB0] = c;
	m[CZA1][CZB1] = -s;
	m[CZA3][CZB3] = s;
	m[CZA4][CZB4] = c;
	m[CZA8][CZB8] = 1.0f;
}

void CzMatrix3::Multiply(CzMatrix3* m0)
{
	Multiply(this, m0);
}

void CzMatrix3::MultiplyPost(CzMatrix3* m0)
{
	Multiply(m0, this);
}

void CzMatrix3::Multiply(CzMatrix3* m0, CzMatrix3* m1)
{
	float 	m0_0, m0_1, m0_2;
	float 	m00, m01, m02;
	float 	m10, m11, m12;
	float 	m20, m21, m22;

	m00 = m1->m[CZA0][CZB0];
	m10 = m1->m[CZA3][CZB3];
	m20 = m1->m[CZA6][CZB6];
	m01 = m1->m[CZA1][CZB1];
	m11 = m1->m[CZA4][CZB4];
	m21 = m1->m[CZA7][CZB7];
	m02 = m1->m[CZA2][CZB2];
	m12 = m1->m[CZA5][CZB5];
	m22 = m1->m[CZA8][CZB8];

	m0_0 = m0->m[CZA0][CZB0];
	m0_1 = m0->m[CZA1][CZB1];
	m0_2 = m0->m[CZA2][CZB2];
	m[CZA0][CZB0] = ((m0_0 * m00) + (m0_1 * m10) + (m0_2 * m20));
	m[CZA1][CZB1] = ((m0_0 * m01) + (m0_1 * m11) + (m0_2 * m21));
	m[CZA2][CZB2] = ((m0_0 * m02) + (m0_1 * m12) + (m0_2 * m22));

	m0_0 = m0->m[CZA3][CZB3];
	m0_1 = m0->m[CZA4][CZB4];
	m0_2 = m0->m[CZA5][CZB5];
	m[CZA3][CZB3] = ((m0_0 * m00) + (m0_1 * m10) + (m0_2 * m20));
	m[CZA4][CZB4] = ((m0_0 * m01) + (m0_1 * m11) + (m0_2 * m21));
	m[CZA5][CZB5] = ((m0_0 * m02) + (m0_1 * m12) + (m0_2 * m22));

	m0_0 = m0->m[CZA6][CZB6];
	m0_1 = m0->m[CZA7][CZB7];
	m0_2 = m0->m[CZA8][CZB8];
	m[CZA6][CZB6] = ((m0_0 * m00) + (m0_1 * m10) + (m0_2 * m20));
	m[CZA7][CZB7] = ((m0_0 * m01) + (m0_1 * m11) + (m0_2 * m21));
	m[CZA8][CZB8] = ((m0_0 * m02) + (m0_1 * m12) + (m0_2 * m22));
}

void CzMatrix3::Transform(float x, float y, CzVec2* ov)
{
	ov->x = (x * m[CZA0][CZB0]) + (y * m[CZA1][CZB1]) + m[CZA2][CZB2];
	ov->y = (x * m[CZA3][CZB3]) + (y * m[CZA4][CZB4]) + m[CZA5][CZB5];
}

void CzMatrix3::TransformSVec(float x, float y, CzSVec2* ov)
{
	ov->x = (short)((x * m[CZA0][CZB0]) + (y * m[CZA1][CZB1]) + m[CZA2][CZB2]);
	ov->y = (short)((x * m[CZA3][CZB3]) + (y * m[CZA4][CZB4]) + m[CZA5][CZB5]);
}

void CzMatrix3::Transform(CzVec2* iv, CzVec2* ov)
{
	Transform(iv->x, iv->y, ov);
}

CzVec2 CzMatrix3::Transform(float x, float y)
{
	CzVec2 ov;
	Transform(x, y, &ov);
	return ov;
}

CzVec2 CzMatrix3::Transform(CzVec2* iv)
{
	CzVec2 ov;
	Transform(iv->x, iv->y, &ov);
	return ov;
}

CzSVec2 CzMatrix3::TransformSVec(CzVec2* iv)
{
	CzSVec2 ov;
	TransformSVec(iv->x, iv->y, &ov);
	return ov;
}

CzSVec2 CzMatrix3::TransformSVec(float x, float y)
{
	CzSVec2 ov;
	TransformSVec(x, y, &ov);
	return ov;
}

float CzMatrix3::TransformZ(float x, float y)
{
	return (x * m[CZA6][CZB6]) + (y * m[CZA7][CZB7]) + m[CZA8][CZB8];
}

float CzMatrix3::TransformZRS(float x, float y)
{
	return (x * m[CZA6][CZB6]) + (y * m[CZA7][CZB7]);
}

void CzMatrix3::TransformRotationOnly(float x, float y, CzVec2* ov)
{
	ov->x = (x * m[CZA0][CZB0]) + (y * m[CZA1][CZB1]);
	ov->y = (x * m[CZA3][CZB3]) + (y * m[CZA4][CZB4]);
}

void CzMatrix3::TransformRotationOnly(CzVec2* iv, CzVec2* ov)
{
	TransformRotationOnly(iv->x, iv->y, ov);
}

void CzMatrix3::TransformN(CzVec2* iv, CzVec2* ov, int nCount)
{
	float	m00 = m[CZA0][CZB0];
	float	m01 = m[CZA1][CZB1];
	float	m10 = m[CZA3][CZB3];
	float	m11 = m[CZA4][CZB4];
	float	tx = m[CZA2][CZB2];
	float	ty = m[CZA5][CZB5];

	while (nCount-- != 0)
	{
		float x = iv->x;
		float y = iv->y;

		ov->x = (x * m00) + (y * m01) + tx;
		ov->y = (x * m10) + (y * m11) + ty;
		
		iv++;
		ov++;
	}
}

void CzMatrix3::TransformNRotationOnly(CzVec2* iv, CzVec2* ov, int nCount)
{
	float	m00 = m[CZA0][CZB0];
	float	m01 = m[CZA1][CZB1];
//	float	m02 = m[CZA2][CZB2];
	float	m10 = m[CZA3][CZB3];
	float	m11 = m[CZA4][CZB4];
//	float	m12 = m[CZA5][CZB5];
//	float	m20 = m[CZA6][CZB6];
//	float	m21 = m[CZA7][CZB7];
//	float	m22 = m[CZA8][CZB8];

	while (nCount-- != 0)
	{
		float x = iv->x;
		float y = iv->y;

		ov->x = (x * m00) + (y * m01);
		ov->y = (x * m10) + (y * m11);

		iv++;
		ov++;
	}
}

void CzMatrix3::TransformNPreTranslate(CzVec2* iv, CzVec2* ov, int nCount)
{
	float	m00 = m[CZA0][CZB0];
	float	m01 = m[CZA1][CZB1];
//	float	m02 = m[CZA2][CZB2];
	float	m10 = m[CZA3][CZB3];
	float	m11 = m[CZA4][CZB4];
//	float	m12 = m[CZA5][CZB5];
//	float	m20 = m[CZA6][CZB6];
//	float	m21 = m[CZA7][CZB7];
//	float	m22 = m[CZA8][CZB8];
	float	tx =  m[CZA2][CZB2];
	float	ty =  m[CZA5][CZB5];

	while (nCount-- != 0)
	{
		float x = iv->x + tx;
		float y = iv->y + ty;
		
		ov->x = (x * m00) + (y * m01);
		ov->y = (x * m10) + (y * m11);

		iv++;
		ov++;
	}
}


CzVec2 CzMatrix3::getTranslation()
{
	return CzVec2(m[CZA2][CZB2], m[CZA5][CZB5]);
}

float CzMatrix3::getX() const
{
	return m[CZA2][CZB2];
}
float CzMatrix3::getY() const
{
	return m[CZA5][CZB5];
}

