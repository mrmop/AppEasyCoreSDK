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

#ifndef __CZ_MATRIX3_H__
#define __CZ_MATRIX3_H__

#include "CzVec3.h"
#include "CzVec2.h"

class CzMatrix3
{
private:
public:
	float		m[3][3];
private:
public:
				CzMatrix3();
				CzMatrix3(CzMatrix3* m0);

	void		Clear();
	void		Copy(CzMatrix3* m0);
	void		Inverse();
	void		Invert();
	void		Transpose();
	void		Identity();
	void		Translate(float x, float y);
	void		Translate(CzVec2* pV);
	void		Translate(CzVec3* pV);
	void		TranslateSet(float x, float y);
	void		TranslateSet(CzVec2* pV);
	void		Scale(float x, float y);
	void		Scale(CzVec2 *v);
	void		Scale(CzVec3 *v);
	void		ScaleRotation(float x, float y);
	void		Rotate(float angle);
	void		Multiply(CzMatrix3* m0);
	void		MultiplyPost(CzMatrix3* m0);
	void		Multiply(CzMatrix3* m0, CzMatrix3* m1);
	void		Transform(float x, float y, CzVec2* ov);
	void		TransformSVec(float x, float y, CzSVec2* ov);
	void		Transform(CzVec2* iv, CzVec2* ov);
	CzVec2		Transform(CzVec2* iv);
	CzVec2		Transform(float x, float y);
	CzSVec2		TransformSVec(CzVec2* iv);
	CzSVec2		TransformSVec(float x, float y);
	void		TransformRotationOnly(float x, float y, CzVec2 *ov);
	void		TransformRotationOnly(CzVec2* iv, CzVec2* ov);
	void		TransformN(CzVec2* iv, CzVec2* ov, int nCount);
	void		TransformN(CzVec3* iv, CzVec3* ov, int nCount);
	void		TransformNRotationOnly(CzVec2* iv, CzVec2* ov, int nCount);
	void		TransformNPreTranslate(CzVec2* iv, CzVec2* ov, int nCount);
	float		TransformZ(float x, float y);
	float		TransformZRS(float x, float y);

	CzVec2		getTranslation();
	float		getX() const;
	float		getY() const;
	
};


#endif	// __CV_MATRIX3_H__
