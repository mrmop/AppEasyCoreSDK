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


#include <string.h>
#include "CzMatrix4.h"
#include "CzMath.h"
#include "CzVec3.h"

#define CZA0	0	// C0
#define CZA1	1
#define CZA2	2
#define CZA3	3
#define CZA4	0	// C1
#define CZA5	1
#define CZA6	2
#define CZA7	3
#define CZA8	0	// C2
#define CZA9	1
#define CZA10	2
#define CZA11	3
#define CZA12	0	// C3
#define CZA13	1
#define CZA14	2
#define CZA15	3

#define CZB0	0
#define CZB1	0
#define CZB2	0
#define CZB3	0
#define CZB4	1
#define CZB5	1
#define CZB6	1
#define CZB7	1
#define CZB8	2
#define CZB9	2
#define CZB10	2
#define CZB11	2
#define CZB12	3
#define CZB13	3
#define CZB14	3
#define CZB15	3


CzMatrix4::CzMatrix4()
{
	Clear();
}	
CzMatrix4::CzMatrix4(CzMatrix4* m0)
{
	Copy(m0);
}

void CzMatrix4::Clear()
{
	memset(&m, 0, sizeof(m));
	m[CZA0][CZB0] = 1.0f;
	m[CZA5][CZB5] = 1.0f;
	m[CZA10][CZB10] = 1.0f;
	m[CZA15][CZB15] = 1.0f;
}

void CzMatrix4::Copy(CzMatrix4* m0)
{
	memcpy(&m, &m0->m, sizeof(m));
}

void CzMatrix4::Invert()
{
	float tx = m[CZA3][CZB3];
	float ty = m[CZA7][CZB7];
	float tz = m[CZA11][CZB11];

	Transpose();

	m[CZA3][CZB3] = -tx;
	m[CZA7][CZB7] = -ty;
	m[CZA11][CZB11] = -tz;
}

void CzMatrix4::Transpose()
{
	CzMatrix4 m0 = *this;

	m[CZA0][CZB0] = m0.m[CZA0][CZB0];
	m[CZA1][CZB1] = m0.m[CZA4][CZB4];
	m[CZA2][CZB2] = m0.m[CZA8][CZB8];
	m[CZA4][CZB4] = m0.m[CZA1][CZB1];
	m[CZA5][CZB5] = m0.m[CZA5][CZB5];
	m[CZA6][CZB6] = m0.m[CZA9][CZB9];
	m[CZA8][CZB8] = m0.m[CZA2][CZB2];
	m[CZA9][CZB9] = m0.m[CZA6][CZB6];
	m[CZA10][CZB10] = m0.m[CZA10][CZB10];
}

void CzMatrix4::Transpose4x4()
{
	CzMatrix4 m0 = *this;

	m[CZA0][CZB0] = m0.m[CZA0][CZB0];
	m[CZA1][CZB1] = m0.m[CZA4][CZB4];
	m[CZA2][CZB2] = m0.m[CZA8][CZB8];
	m[CZA3][CZB3] = m0.m[CZA12][CZB12];
	m[CZA4][CZB4] = m0.m[CZA1][CZB1];
	m[CZA5][CZB5] = m0.m[CZA5][CZB5];
	m[CZA6][CZB6] = m0.m[CZA9][CZB9];
	m[CZA7][CZB7] = m0.m[CZA13][CZB13];
	m[CZA8][CZB8] = m0.m[CZA2][CZB2];
	m[CZA9][CZB9] = m0.m[CZA6][CZB6];
	m[CZA10][CZB10] = m0.m[CZA10][CZB10];
	m[CZA11][CZB11] = m0.m[CZA14][CZB14];
	m[CZA12][CZB12] = m0.m[CZA3][CZB3];
	m[CZA13][CZB13] = m0.m[CZA7][CZB7];
	m[CZA14][CZB14] = m0.m[CZA11][CZB11];
	m[CZA15][CZB15] = m0.m[CZA15][CZB15];
}

void CzMatrix4::ConvertGL(CzMatrix4& m0)
{
	m0.m[CZA0][CZB0] = m[CZA0][CZB0];
	m0.m[CZA1][CZB1] = m[CZA4][CZB4];
	m0.m[CZA2][CZB2] = m[CZA8][CZB8];
	m0.m[CZA3][CZB3] = m[CZA12][CZB12];
	m0.m[CZA4][CZB4] = m[CZA1][CZB1];
	m0.m[CZA5][CZB5] = m[CZA5][CZB5];
	m0.m[CZA6][CZB6] = m[CZA9][CZB9];
	m0.m[CZA7][CZB7] = m[CZA13][CZB13];
	m0.m[CZA8][CZB8] = m[CZA2][CZB2];
	m0.m[CZA9][CZB9] = m[CZA6][CZB6];
	m0.m[CZA10][CZB10] = m[CZA10][CZB10];
	m0.m[CZA11][CZB11] = m[CZA14][CZB14];
	m0.m[CZA12][CZB12] = m[CZA3][CZB3];
	m0.m[CZA13][CZB13] = m[CZA7][CZB7];
	m0.m[CZA14][CZB14] = m[CZA11][CZB11];
	m0.m[CZA15][CZB15] = m[CZA15][CZB15];
}

void CzMatrix4::ConvertFromGL(float* m0)
{
	m[CZA0][CZB0] = *(m0++);
	m[CZA1][CZB1] = *(m0++);
	m[CZA2][CZB2] = *(m0++);
	m[CZA3][CZB3] = *(m0++);
	m[CZA4][CZB4] = *(m0++);
	m[CZA5][CZB5] = *(m0++);
	m[CZA6][CZB6] = *(m0++);
	m[CZA7][CZB7] = *(m0++);
	m[CZA8][CZB8] = *(m0++);
	m[CZA9][CZB9] = *(m0++);
	m[CZA10][CZB10] = *(m0++);
	m[CZA11][CZB11] = *(m0++);
	m[CZA12][CZB12] = *(m0++);
	m[CZA13][CZB13] = *(m0++);
	m[CZA14][CZB14] = *(m0++);
	m[CZA15][CZB15] = *(m0++);
}

void CzMatrix4::Identity()
{
	Clear();
	m[CZA0][CZB0] = 1.0f;
	m[CZA5][CZB5] = 1.0f;
	m[CZA10][CZB10] = 1.0f;
	m[CZA15][CZB15] = 1.0f;
}

void CzMatrix4::UVN(CzVec3* u, CzVec3* v, CzVec3* n)
{
	Clear();
	m[CZA0][CZB0] = u->x;
	m[CZA1][CZB1] = u->y;
	m[CZA2][CZB2] = u->z;
	m[CZA4][CZB4] = v->x;
	m[CZA5][CZB5] = v->y;
	m[CZA6][CZB6] = v->z;
	m[CZA8][CZB8] = n->x;
	m[CZA9][CZB9] = n->y;
	m[CZA10][CZB10] = n->z;		
	m[CZA15][CZB15] = 1.0f;
}

void CzMatrix4::Translate(float x, float y, float z)
{
	Clear();
	m[CZA3][CZB3] = x;
	m[CZA7][CZB7] = y;
	m[CZA11][CZB11] = z;
}

void CzMatrix4::Translate(CzVec3* v)
{
	Translate(v->x, v->y, v->z);
}

void CzMatrix4::Scale(float x, float y, float z)
{
	Clear();
	m[CZA0][CZB0] = x;
	m[CZA5][CZB5] = y;
	m[CZA10][CZB10] = z;
	m[CZA15][CZB15] = 1.0f;
}

void CzMatrix4::Scale(CzVec3* v)
{
	Scale(v->x, v->y, v->z);
}

void CzMatrix4::TranslateSet(float x, float y, float z)
{
	m[CZA3][CZB3] = x;
	m[CZA7][CZB7] = y;
	m[CZA11][CZB11] = z;
}

void CzMatrix4::TranslateSet(CzVec3* v)
{
	m[CZA3][CZB3] = v->x;
	m[CZA7][CZB7] = v->y;
	m[CZA11][CZB11] = v->z;
}

void CzMatrix4::RotateX(float angle)
{
	angle = CzMath::DegToRad(angle);
	float c = CzMath::cos(angle);
	float s = CzMath::sin(angle);

	Clear();
	m[CZA0][CZB0] = 1.0f;
	m[CZA5][CZB5] = c;
	m[CZA6][CZB6] = -s;
	m[CZA9][CZB9] = s;
	m[CZA10][CZB10] = c;
	m[CZA15][CZB15] = 1.0f;
}

void CzMatrix4::RotateY(float angle)
{
	angle = CzMath::DegToRad(angle);
	float c = CzMath::cos(angle);
	float s = CzMath::sin(angle);

	Clear();
	m[CZA0][CZB0] = c;
	m[CZA2][CZB2] = s;
	m[CZA5][CZB5] = 1.0f;
	m[CZA8][CZB8] = -s;
	m[CZA10][CZB10] = c;
	m[CZA15][CZB15] = 1.0f;
}

void CzMatrix4::RotateZ(float angle)
{
	angle = CzMath::DegToRad(angle);
	float c = CzMath::cos(angle);
	float s = CzMath::sin(angle);

	Clear();
	m[CZA0][CZB0] = c;
	m[CZA1][CZB1] = -s;
	m[CZA4][CZB4] = s;
	m[CZA5][CZB5] = c;
	m[CZA10][CZB10] = 1.0f;
	m[CZA15][CZB15] = 1.0f;
}

void CzMatrix4::Rotate(float x, float y, float z)
{
	CzMatrix4 mx;
	CzMatrix4 my;
	CzMatrix4 mz;

	mx.RotateX(x);
	my.RotateY(y);
	mz.RotateZ(z);

	Multiply(&mx, &my);
	Multiply(&mz);
}

void CzMatrix4::Rotate(CzVec3* pAngles)
{
	Rotate(pAngles->x, pAngles->y, pAngles->z);
}

void CzMatrix4::RotateYXZ(CzVec3* pAngles)
{
	RotateYXZ(pAngles->x, pAngles->y, pAngles->z);
}

void CzMatrix4::RotateYXZ(float x, float y, float z)
{
	CzMatrix4 mx;
	CzMatrix4 my;
	CzMatrix4 mz;

	mx.RotateX(x);
	my.RotateY(y);
	mz.RotateZ(z);

	Multiply(&mx, &mz);
	Multiply(&my);
}

void CzMatrix4::Quat(float x, float y, float z, float w)
{
	float 	xs, ys, zs;
	float	wx, wy, wz;
	float	xx, xy, xz;
	float	yy, yz, zz;

	float	s = 2.0f / (x * x + y * y + z * z + w * w);

	xs = x * s;		ys = y * s;		zs = z * s;
	wx = w * xs;	wy = w * ys;	wz = w * zs;
	xx = x * xs;	xy = x * ys;	xz = x * zs;
	yy = y * ys;	yz = y * zs;	zz = z * zs;

	Clear();

	m[CZA0][CZB0] = 1.0f - (yy + zz);
	m[CZA1][CZB1] = xy - wz;
	m[CZA2][CZB2] = xz + wy;

	m[CZA4][CZB4] = xy + wz;
	m[CZA5][CZB5] = 1.0f - (xx + zz);
	m[CZA6][CZB6] = yz - wx;

	m[CZA8][CZB8] = xz - wy;
	m[CZA9][CZB9] = yz + wx;
	m[CZA10][CZB10] = 1.0f - (xx + yy);

	m[CZA3][CZB3] = 0;
	m[CZA7][CZB7] = 0;
	m[CZA11][CZB11] = 0;
}

void CzMatrix4::LookAt(float eye_x, float eye_y, float eye_z, float lookat_x, float lookat_y, float lookat_z, float up_x, float up_y, float up_z, CzVec3* trans)
{
	CzVec3 u;
	CzVec3 v;
	CzVec3 n;
	CzVec3 up(up_x, up_y, up_z);

	n.x = eye_x - lookat_x;
	n.y = eye_y - lookat_y;
	n.z = eye_z - lookat_z;
	n.Normalise();
	u = up; u.Cross(n);
	u.Normalise();
	v = n; v.Cross(u);
	UVN(&u, &v, &n);		
	//		TranslateSet(-eye_x, -eye_y, -eye_z);
}

void CzMatrix4::Multiply(CzMatrix4* m1)
{
	Multiply(this, m1);
}

void CzMatrix4::MultiplyPost(CzMatrix4* m1)
{
	Multiply(m1, this);
}

void CzMatrix4::Multiply(CzMatrix4* m0, CzMatrix4* m1)
{
	float 	m0_0, m0_1, m0_2;
	float 	m00, m01, m02, m03;
	float 	m10, m11, m12, m13;
	float 	m20, m21, m22, m23;

	m00 = m1->m[CZA0][CZB0];
	m10 = m1->m[CZA4][CZB4];
	m20 = m1->m[CZA8][CZB8];
	m01 = m1->m[CZA1][CZB1];
	m11 = m1->m[CZA5][CZB5];
	m21 = m1->m[CZA9][CZB9];
	m02 = m1->m[CZA2][CZB2];
	m12 = m1->m[CZA6][CZB6];
	m22 = m1->m[CZA10][CZB10];
	m03 = m1->m[CZA3][CZB3];
	m13 = m1->m[CZA7][CZB7];
	m23 = m1->m[CZA11][CZB11];

	m0_0 = m0->m[CZA0][CZB0];
	m0_1 = m0->m[CZA1][CZB1];
	m0_2 = m0->m[CZA2][CZB2];
	m[CZA0][CZB0] = ((m0_0 * m00) + (m0_1 * m10) + (m0_2 * m20));
	m[CZA1][CZB1] = ((m0_0 * m01) + (m0_1 * m11) + (m0_2 * m21));
	m[CZA2][CZB2] = ((m0_0 * m02) + (m0_1 * m12) + (m0_2 * m22));
	m[CZA3][CZB3] = ((m0_0 * m03) + (m0_1 * m13) + (m0_2 * m23) + m0->m[CZA3][CZB3]);

	m0_0 = m0->m[CZA4][CZB4];
	m0_1 = m0->m[CZA5][CZB5];
	m0_2 = m0->m[CZA6][CZB6];
	m[CZA4][CZB4] = ((m0_0 * m00) + (m0_1 * m10) + (m0_2 * m20));
	m[CZA5][CZB5] = ((m0_0 * m01) + (m0_1 * m11) + (m0_2 * m21));
	m[CZA6][CZB6] = ((m0_0 * m02) + (m0_1 * m12) + (m0_2 * m22));
	m[CZA7][CZB7] = ((m0_0 * m03) + (m0_1 * m13) + (m0_2 * m23) + m0->m[CZA7][CZB7]);

	m0_0 = m0->m[CZA8][CZB8];
	m0_1 = m0->m[CZA9][CZB9];
	m0_2 = m0->m[CZA10][CZB10];
	m[CZA8][CZB8] = ((m0_0 * m00) + (m0_1 * m10) + (m0_2 * m20));
	m[CZA9][CZB9] = ((m0_0 * m01) + (m0_1 * m11) + (m0_2 * m21));
	m[CZA10][CZB10] = ((m0_0 * m02) + (m0_1 * m12) + (m0_2 * m22));
	m[CZA11][CZB11] = ((m0_0 * m03) + (m0_1 * m13) + (m0_2 * m23) + m0->m[CZA11][CZB11]);

	m[CZA15][CZB15] = 1.0f;
}

void CzMatrix4::Transform(float x, float y, float z, CzVec3* ov)
{
	ov->x = ((x * m[CZA0][CZB0]) + (y * m[CZA1][CZB1]) + (z * m[CZA2][CZB2])) + m[CZA3][CZB3];
	ov->y = ((x * m[CZA4][CZB4]) + (y * m[CZA5][CZB5]) + (z * m[CZA6][CZB6])) + m[CZA7][CZB7];
	ov->z = ((x * m[CZA8][CZB8]) + (y * m[CZA9][CZB9]) + (z * m[CZA10][CZB10])) + m[CZA11][CZB11];
}

CzVec3 CzMatrix4::TransformProject(float x, float y, float z, float d)
{
	CzVec3 ov;
	ov.z = ((x * m[CZA8][CZB8]) + (y * m[CZA9][CZB9]) + (z * m[CZA10][CZB10])) + m[CZA11][CZB11];
	float oz = d / (d + ov.z);
	ov.x = (((x * m[CZA0][CZB0]) + (y * m[CZA1][CZB1]) + (z * m[CZA2][CZB2])) + m[CZA3][CZB3]) * oz;
	ov.y = (((x * m[CZA4][CZB4]) + (y * m[CZA5][CZB5]) + (z * m[CZA6][CZB6])) + m[CZA7][CZB7]) * oz;

	return ov;
}

void CzMatrix4::Project(float x, float y, float z, CzVec3* ov, float d)
{
	ov->z = z;
	float oz = d / (d + z);
	ov->x = x * oz;
	ov->y = y * oz;
}

void CzMatrix4::Transform(CzVec3* iv, CzVec3* ov)
{
	Transform(iv->x, iv->y, iv->z, ov);
}

CzVec3 CzMatrix4::Transform(float x, float y, float z)
{
	CzVec3 ov;
	Transform(x, y, z, &ov);
	return ov;
}

CzVec3 CzMatrix4::Project(float x, float y, float z, float d)
{
	CzVec3 ov;
	Project(x, y, z, &ov, d);
	return ov;
}

float CzMatrix4::TransformZ(float x, float y, float z)
{
	return ((x * m[CZA8][CZB8]) + (y * m[CZA9][CZB9]) + (z * m[CZA10][CZB10])) + m[CZA11][CZB11];
}

float CzMatrix4::TransformZRS(float x, float y, float z)
{
	return ((x * m[CZA8][CZB8]) + (y * m[CZA9][CZB9]) + (z * m[CZA10][CZB10]));
}

void CzMatrix4::TransformRotationOnly(float x, float y, float z, CzVec3* ov)
{
	ov->x = ((x * m[CZA0][CZB0]) + (y * m[CZA1][CZB1]) + (z * m[CZA2][CZB2]));
	ov->y = ((x * m[CZA4][CZB4]) + (y * m[CZA5][CZB5]) + (z * m[CZA6][CZB6]));
	ov->z = ((x * m[CZA8][CZB8]) + (y * m[CZA9][CZB9]) + (z * m[CZA10][CZB10]));
}

void CzMatrix4::TransformRotationOnly(CzVec3* iv, CzVec3* ov)
{
	TransformRotationOnly(iv->x, iv->y, iv->z, ov);
}

void CzMatrix4::TransformN(CzVec3* iv, CzVec3* ov, int nCount)
{
	float	m00 = m[CZA0][CZB0];
	float	m01 = m[CZA1][CZB1];
	float	m02 = m[CZA2][CZB2];
	float	m10 = m[CZA4][CZB4];
	float	m11 = m[CZA5][CZB5];
	float	m12 = m[CZA6][CZB6];
	float	m20 = m[CZA8][CZB8];
	float	m21 = m[CZA9][CZB9];
	float	m22 = m[CZA10][CZB10];
	float	tx = m[CZA3][CZB3];
	float	ty = m[CZA7][CZB7];
	float	tz = m[CZA11][CZB11];

	while (nCount-- != 0)
	{
		float x = iv->x;
		float y = iv->y;
		float z = iv->z;

		ov->x = ((x * m00) + (y * m01) + (z * m02)) + tx;
		ov->y = ((x * m10) + (y * m11) + (z * m12)) + ty;
		ov->z = ((x * m20) + (y * m21) + (z * m22)) + tz;
		
		iv++;
		ov++;
	}
}

void CzMatrix4::TransformProjectN(CzVec3* iv, CzVec3* ov, int nCount, float d)
{
	float	m00 = m[CZA0][CZB0];
	float	m01 = m[CZA1][CZB1];
	float	m02 = m[CZA2][CZB2];
	float	m10 = m[CZA4][CZB4];
	float	m11 = m[CZA5][CZB5];
	float	m12 = m[CZA6][CZB6];
	float	m20 = m[CZA8][CZB8];
	float	m21 = m[CZA9][CZB9];
	float	m22 = m[CZA10][CZB10];
	float	tx = m[CZA3][CZB3];
	float	ty = m[CZA7][CZB7];
	float	tz = m[CZA11][CZB11];

	while (nCount-- != 0)
	{
		float x = iv->x;
		float y = iv->y;
		float z = iv->z;

		ov->z = ((x * m20) + (y * m21) + (z * m22)) + tz;
		float oz = d / (d + ov->z);
		ov->x = (((x * m00) + (y * m01) + (z * m02)) + tx) * oz;
		ov->y = (((x * m10) + (y * m11) + (z * m12)) + ty) * oz;
		
		iv++;
		ov++;
	}
}

void CzMatrix4::TransformNRotationOnly(CzVec3* iv, CzVec3* ov, int nCount)
{
	float	m00 = m[CZA0][CZB0];
	float	m01 = m[CZA1][CZB1];
	float	m02 = m[CZA2][CZB2];
	float	m10 = m[CZA4][CZB4];
	float	m11 = m[CZA5][CZB5];
	float	m12 = m[CZA6][CZB6];
	float	m20 = m[CZA8][CZB8];
	float	m21 = m[CZA9][CZB9];
	float	m22 = m[CZA10][CZB10];

	while (nCount-- != 0)
	{
		float x = iv->x;
		float y = iv->y;
		float z = iv->z;

		ov->x = ((x * m00) + (y * m01) + (z * m02));
		ov->y = ((x * m10) + (y * m11) + (z * m12));
		ov->z = ((x * m20) + (y * m21) + (z * m22));

		iv++;
		ov++;
	}
}

void CzMatrix4::TransformNPreTranslate(CzVec3* iv, CzVec3* ov, int nCount)
{
	float	m00 = m[CZA0][CZB0];
	float	m01 = m[CZA1][CZB1];
	float	m02 = m[CZA2][CZB2];
	float	m10 = m[CZA4][CZB4];
	float	m11 = m[CZA5][CZB5];
	float	m12 = m[CZA6][CZB6];
	float	m20 = m[CZA8][CZB8];
	float	m21 = m[CZA9][CZB9];
	float	m22 = m[CZA10][CZB10];
	float	tx = m[CZA3][CZB3];
	float	ty = m[CZA7][CZB7];
	float	tz = m[CZA11][CZB11];

	while (nCount-- != 0)
	{
		float x = iv->x + tx;
		float y = iv->y + ty;
		float z = iv->z + tz;
		
		ov->x = ((x * m00) + (y * m01) + (z * m02));
		ov->y = ((x * m10) + (y * m11) + (z * m12));
		ov->z = ((x * m20) + (y * m21) + (z * m22));

		iv++;
		ov++;
	}
}
void CzMatrix4::ProjectN(CzVec3* iv, CzVec3* ov, int nCount, float d)
{
	while (nCount-- != 0)
	{
		float x = iv->x;
		float y = iv->y;
		float z = iv->z;

		float oz = d / (d + iv->z);
		ov->z = iv->z;
		ov->x = iv->x * oz;
		ov->y = iv->y * oz;
		
		iv++;
		ov++;
	}
}
CzVec3 CzMatrix4::getTranslation()
{
	return CzVec3(m[CZA3][CZB3], m[CZA7][CZB7], m[CZA11][CZB11]);
}

float CzMatrix4::getX() const
{
	return m[CZA3][CZB3];
}
float CzMatrix4::getY() const
{
	return m[CZA7][CZB7];
}
float CzMatrix4::getZ() const
{
	return m[CZA11][CZB11];
}



void CzMatrix4::OrthoProjection(float top, float bottom, float left, float right, float near_plane, float far_plane)
{
	Clear();
	m[CZA0][CZB0] = 2 / (right - left);
	m[CZA3][CZB3] = -((right + left) / (right - left));
	m[CZA5][CZB5] = 2 / (top - bottom);
	m[CZA7][CZB7] = -((top + bottom) / (top - bottom));
	m[CZA10][CZB10] = -2 / (far_plane - near_plane);
	m[CZA11][CZB11] = -((far_plane + near_plane) / (near_plane - far_plane));
	m[CZA15][CZB15] = 1;
}

void CzMatrix4::PerspProjection(float fov, float aspect, float near_plane, float far_plane)
{
	CzMatrix4 proj;
	float f = 1.0f / CzMath::tan(fov / 2.0f);

	Clear();
	m[CZA0][CZB0] = f / (aspect);
	m[CZA5][CZB5] = f;
	m[CZA10][CZB10] = -near_plane / (far_plane - near_plane);
	m[CZA11][CZB11] = -(far_plane * near_plane) / (far_plane - near_plane);
	m[CZA14][CZB14] = -1.0f;
}

