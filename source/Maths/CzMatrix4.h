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

#ifndef __CZ_MATRIX4_H__
#define __CZ_MATRIX4_H__

#include "CzVec3.h"

class CzMatrix4
{
private:
public:
	float		m[4][4];
private:
public:
				CzMatrix4();
				CzMatrix4(CzMatrix4* m0);

	void		Clear();
	void		Copy(CzMatrix4* m0);
	void		Invert();
	void		Transpose();
	void		Transpose4x4();
	void		ConvertGL(CzMatrix4 &m0);
	void		ConvertFromGL(float* m0);
	void		Identity();
	void		UVN(CzVec3* pU, CzVec3* pV, CzVec3 *pN);
	void		Translate(float x, float y, float z);
	void		Translate(CzVec3* pV);
	void		TranslateSet(float x, float y, float z);
	void		TranslateSet(CzVec3* pV);
	void		Scale(float x, float y, float z);
	void		Scale(CzVec3 *v);
	void		Quat(float x, float y, float z, float w);
	void		AngleAxis(float axis_x, float axis_y, float axis_z, float angle);
	void		LookAt(float eye_x, float eye_y, float eye_z, float lookat_x, float lookat_y, float lookat_z, float up_x, float up_y, float up_z, CzVec3* trans);
	void		RotateX(float angle);
	void		RotateY(float angle);
	void		RotateZ(float angle);
	void		Rotate(float x, float y, float z);
	void		Rotate(CzVec3 *pAngles);
	void		RotateYXZ(CzVec3 *pAngles);
	void		RotateYXZ(float x, float y, float z);
	void		Multiply(CzMatrix4* m0);
	void		MultiplyPost(CzMatrix4* m0);
	void		Multiply(CzMatrix4* m0, CzMatrix4* m1);
	void		Transform(float x, float y, float z, CzVec3 *ov);
	CzVec3		TransformProject(float x, float y, float z, float d);
	void		Transform(CzVec3 *iv, CzVec3 *ov);
	CzVec3		Transform(float x, float y, float z);
	void		TransformRotationOnly(float x, float y, float z, CzVec3 *ov);
	void		TransformRotationOnly(CzVec3* iv, CzVec3* ov);
	void		TransformN(CzVec3* iv, CzVec3* ov, int nCount);
	void		TransformProjectN(CzVec3* iv, CzVec3* ov, int nCount, float d);
	void		TransformNRotationOnly(CzVec3* iv, CzVec3* ov, int nCount);
	void		TransformNPreTranslate(CzVec3* iv, CzVec3* ov, int nCount);
	float		TransformZ(float x, float y, float z);
	float		TransformZRS(float x, float y, float z);

	static void		Project(float x, float y, float z, CzVec3 *ov, float d);
	static CzVec3	Project(float x, float y, float z, float d);
	static void		ProjectN(CzVec3* iv, CzVec3* ov, int nCount, float d);

	CzVec3		getTranslation();
	float		getX() const;
	float		getY() const;
	float		getZ() const;

	void		OrthoProjection(float top, float bottom, float left, float right, float near_plane, float far_plane);
	void		PerspProjection(float fov, float aspect, float near_plane, float far_plane);
};


#endif	// __CZ_MATRIX4_H__
