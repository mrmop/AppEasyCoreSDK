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

#if (!defined__CZ_MATH_H__)
#define __CZ_MATH_H__

#include "CzVec2.h"
#include "CzVec3.h"

#if (!defined PI)
	#define PI 3.1415926535f
#endif

#if (!defined NULL)
	#define NULL	0
#endif

class CzMath
{
private:
public:

private:
public:
	// Angle operations
	static float	AngleDiff(float a1, float a2);

	// Misc
	static float	abs(float n);
	static int		abs(int n);

	// Trig
	static float	sqrt(float n);
	static float	sin(float n);
	static float	cos(float n);
	static float	tan(float n);
	static float	acos(float n);
	static float	atan(float n);
	static float	atan2(float n1, float n2);
	static float	RadToDeg(float n);
	static float	DegToRad(float n);

	// Quaternions
	static void		QuatSLERP(float *pQ1, float *pQ2, float *pQOut, float t);

	// Random numbers
	static void		SeedRand(unsigned int seed);
	static int		Rand();
	static float	Rand(float range);

	// Point Routines
	static int		PointClosestToPerimeter(CzVec3& point, CzVec3* v, int nNumLines, CzVec3& out);
	static float	PointDistanceToPlane(CzVec3& point, CzVec3& pop, CzVec3& normal);
	static void		PointProjectOntoPlane(CzVec3& point, CzVec3& pop, CzVec3& normal, CzVec3& out);
	static bool		PointInsidePolygon(CzVec3& point, CzVec3* poly, int nNumPoints);
	static float	PointIntersectWedge(CzVec3& wedge_base_pos, CzVec3& wedge_size, int type, CzVec3& pos);

	// Ray / Line Routines
	static float	LineIntersectPlane(CzVec3& line_start, CzVec3& line_end, CzVec3& pop, CzVec3& normal, CzVec3& out);
	static float	RayIntersectSphere(CzVec3& ray_pos, CzVec3& ray_dir, CzVec3& sphere_pos, float radius, CzVec3& out);
	static float	LineIntersectCappedCylinder(CzVec3& line_start, CzVec3& line_end, CzVec3& cyl_start, CzVec3& cyl_end, float radius, CzVec3& out);

	// Sphere Routines
	static float	SphereIntersectPlane(CzVec3& sphere_pos, CzVec3& sphere_vel, CzVec3& pop, CzVec3& normal, CzVec3& out, float radius);
	static bool		SphereIntersectCuboidTest(CzVec3& sphere_pos, float sphere_size, CzVec3& box_pos, CzVec3& box_size);

	// Cuboid Routines
	static int		CuboidIntersectTest(CzVec3& box1_pos, CzVec3& box1_size, CzVec3& box2_pos, CzVec3& box2_size);

	// Miscellaneous
	static CzVec3	CalcSlideVelocity(CzVec3& original_pos, CzVec3& original_vel, CzVec3& pop, CzVec3& normal, CzVec3& cp);

	// 2D routines
	static int		PointInsidePolygon2D(CzVec3& point, CzVec3* poly, int nNumPoints);
};

#endif	// __CZ_MATH_H__
