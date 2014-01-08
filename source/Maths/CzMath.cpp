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

//#include <stdlib.h>
#include <math.h>
#include <stdlib.h>

#include "CzMath.h"
#include "CzVec2.h"
#include "CzVec3.h"

float CzMath::AngleDiff(float a1, float a2)
{
	if ((a2 >= a1) && (a2 < (a1 + 180.0f)))
		return a2 - a1;
	else
	if ((a1 >= a2) && (a1 < (a2 + 180.0f)))
		return -(a1 - a2);
	else
	if (a2 > (a1 + 180.0f))
		return -(a1 + 360.0f - a2);
	else
	if (a2 <= (a1 - 180.0f))
		return a2 + 360.0f - a1;

	return 0;
}

float CzMath::sqrt(float n)
{
	return sqrtf(n);
}

float CzMath::sin(float n)
{
	return sinf(n);
}

float CzMath::cos(float n)
{
	return cosf(n);
}

float CzMath::tan(float n)
{
	return tanf(n);
}

float CzMath::acos(float n)
{
	return acosf(n);
}

float CzMath::atan(float n)
{
	return atanf(n);
}

float CzMath::atan2(float n1, float n2)
{
	return atan2f(n1, n2);
}

float CzMath::DegToRad(float n)
{
	return (n * PI) / 180.0f;
}

float CzMath::RadToDeg(float n)
{
	return (n * 180.0f) / PI;
}

void CzMath::QuatSLERP(float *pQ1, float *pQ2, float *pQOut, float t)
{
	float	angle, dp;
	float	dq1, dq2;

	dp = pQ1[0] * pQ2[0] + pQ1[1] * pQ2[1] + pQ1[2] * pQ2[2] + pQ1[3] * pQ2[3];

	if (dp < 0)
	{
		// Quats are more than 90 degrees apart so invert 2nd to reduce spinning
		dp = -dp;
		dq2 = -1.0f;
	}
	else
	{
		dq2 = 1.0f;
	}

	if (dp <= 0.99999)
	{
		// slerp
		angle  = (float)acosf(dp);
		float st = (float)sinf(angle);
		dq1 = (float)sinf((1.0f - t) * angle) / st;
		dq2 *= (float)sinf(t * angle) / st;
	}
	else
	{
		// Diff between quats is very small so use lerp
		dq1 = 1.0f - t;
		dq2 *= t;
	}

	// Interpolate
	pQOut[0] = dq1 * pQ1[0] + dq2 * pQ2[0];
	pQOut[1] = dq1 * pQ1[1] + dq2 * pQ2[1];
	pQOut[2] = dq1 * pQ1[2] + dq2 * pQ2[2];
	pQOut[3] = dq1 * pQ1[3] + dq2 * pQ2[3];
}

int CzMath::Rand()
{
	return rand();
}

float CzMath::Rand(float range)
{
	return ((float)rand() * range) / (float)RAND_MAX;
}

void CzMath::SeedRand(unsigned int seed)
{
	srand(seed);
}

float CzMath::abs(float n)
{
	if (n < 0)
		return -n;

	return n;
}

int	CzMath::abs(int n)
{
	if (n < 0)
		return -n;

	return n;
}

// ================================
//
// Point Routines
//
// ================================
int CzMath::PointClosestToPerimeter(CzVec3& point, CzVec3* v, int nNumLines, CzVec3& out)
{
	float 	closest_d = 0.0f;
	int 	closest = 0;
	CzVec3 tmp_v;
	int		i1 = nNumLines - 1;
	int		i2 = 0;
	CzVec3 diff;
	float	d;
	
	for (int t = 0; t < nNumLines; t++)
	{
		// Get the point on the line that is closest to the test point
		point.PointClosestToLine(v[i1], v[i2], tmp_v);
	
		// Get distance between test point and closest point
		diff.x = point.x - tmp_v.x;
		diff.y = point.y - tmp_v.y;
		diff.z = point.z - tmp_v.z;
		d = diff.MagSquared();
		
		// If closer than previous then set as closest
		if (d < closest_d || t == 0)
		{
			closest_d = d;
			closest = t;
			out.x = tmp_v.x;
			out.y = tmp_v.y;
			out.z = tmp_v.z;
		}
		
		i1 = i2;
		i2++;
	}

	return closest;		
}

float CzMath::PointDistanceToPlane(CzVec3& point, CzVec3& pop, CzVec3& normal)
{
	float D = normal.Dot(pop);		// pop = Point On Plane
	float dist = point.Dot(normal) - D;
	
	return dist;
}

void CzMath::PointProjectOntoPlane(CzVec3& point, CzVec3& pop, CzVec3& normal, CzVec3& out)
{
	float D = normal.Dot(pop);		// pop = Point On Plane
	float t = normal.Dot(point) - D;
	
	out.x = point.x - (t * normal.x);
	out.y = point.y - (t * normal.y);
	out.z = point.z - (t * normal.z);
	
}

bool CzMath::PointInsidePolygon(CzVec3& point, CzVec3* poly, int nNumPoints)
{
	double	theta = 0;
	CzVec3 v0;
	CzVec3 v1;
	int		i1 = 0;
	
	v1.x = point.x - poly[nNumPoints - 1].x;
	v1.y = point.y - poly[nNumPoints - 1].y;
	v1.z = point.z - poly[nNumPoints - 1].z;
	v1.Normalise();
	
	for (int t = 0; t < nNumPoints; t++)
	{
		v0.x = v1.x;
		v0.y = v1.y;
		v0.z = v1.z;
		v1.x = point.x - poly[i1].x;
		v1.y = point.y - poly[i1].y;
		v1.z = point.z - poly[i1].z;
		v1.Normalise();
		
		theta += acos(v0.Dot(v1));

		i1++;			
	}

	// If angles add up to close to 2PI then class as inside polygon
	theta = (2.0f * PI) - theta;
	if (theta < 0)
		theta = -theta;
	if (theta < 0.009f)
		return true;
	
	return false;
}

float CzMath::PointIntersectWedge(CzVec3& wedge_base_pos, CzVec3& wedge_size, int type, CzVec3& pos)
{
	switch (type)
	{
		case 0:
		{
			// North facing wedge (ramp)
			float dz = (pos.z - wedge_base_pos.z) / wedge_size.z;
			return (wedge_base_pos.y) - (dz * wedge_size.y);
			
		}
	}

	return 0;
}

// ====================================================
//
// Ray / Line Routines
//
// Notes:
//		ray_dir = normalised direction of ray
//		return value of -1 indicates no intersection
// ====================================================
float CzMath::LineIntersectPlane(CzVec3& line_start, CzVec3& line_end, CzVec3& pop, CzVec3& normal, CzVec3& out)
{
	float t = -1.0f;
	float D = normal.Dot(pop);
	CzVec3 line_dir = line_end - line_start;
	
	float num = D - normal.Dot(line_start);
	float den = normal.Dot(line_dir);
	
	// Line runs parallel to plane so no solution
	if (abs(den) < 0.0001f)
		return t;
		
	// Only computer intersection point if its on the line
	t = num / den;
	if (t >= 0.0f && t <= 1.0f)
	{
		out.x = line_start.x + t * line_dir.x;
		out.y = line_start.y + t * line_dir.y;
		out.z = line_start.z + t * line_dir.z;
	}
	
	return t;
}

float CzMath::RayIntersectSphere(CzVec3& ray_pos, CzVec3& ray_dir, CzVec3& sphere_pos, float radius, CzVec3& out)
{
	float t = -1.0f;
	CzVec3 m = ray_pos - sphere_pos;
	
	float b = m.Dot(ray_dir);
	float c = m.Dot(m) - radius * radius;
	
	if (b > 0.0f && c > 0.0f)
		return -1.0f;

	// If descriminate negative then ray doesnt touch sphere
	float disc = b * b - c;
	if (disc < 0.0f)
		return -1.0f;
		
	// Calculate smallest t of intersection
	t = -b - sqrt(disc);

	// If t < 0 then ray actually started inside sphere so clamp it
	if (t < 0.0f)
		t = 0.0f;
		
	out.x = ray_pos.x + t * ray_dir.x;
	out.y = ray_pos.y + t * ray_dir.y;
	out.z = ray_pos.z + t * ray_dir.z;
	
	return t;
}

float CzMath::LineIntersectCappedCylinder(CzVec3& line_start, CzVec3& line_end, CzVec3& cyl_start, CzVec3& cyl_end, float radius, CzVec3& out)
{
	float t = -1.0f;
	CzVec3 d = cyl_end - cyl_start;
	CzVec3 m = line_start - cyl_start;
	CzVec3 n = line_end - line_start;
	float	md = m.Dot(d);
	float	nd = n.Dot(d);
	float	dd = d.Dot(d);
	
	// Test if line outside endcaps
	if (md < 0.0f && (md + nd) < 0.0f)
		return t;
	if (md > dd && (md + nd) > dd)
		return t;
	
	float nn = n.Dot(n);
	float mn = m.Dot(n);
	float mm = m.Dot(m);
	float a = dd * nn - nd * nd;
	float k = mm - radius * radius;
	float c = dd * k - md * md;
	
	if (abs(a) < 0.0001f)
	{
		if (c > 0.0f)			// Line is outside cylinder
			return 0;
		
		if (md < 0.0f)
			t = -mn / nn;		// Intersect against cylinder start cap
		else
		if (md > dd)
			t = (md - mn) / nn;	// Intersect against cylinder end cap
		else
			t = 0.0f;			// Inside cylinder
			
		return t;
	}
	
	float b = dd * mn - nd * md;
	float discr = b * b - a * c;
	if (discr < 0.0f)
		return t;				// No intersection
	
	t = (-k - sqrt(discr)) / a;
	if ((t < 0.0f) | (t > 1.0f))
		return -1;				// Intersection outside of line segment
	
	if ((md + t * nd) < 0.0f)
	{
		// Intersection outside cylinder start cap
		if (nd <= 0.0f)
			return 0;			// Segment pointing away from cap
			
		t = -md / nd;
		
		if ((k + 2.0f * t * (mn + t * nn)) > 0)
			return -1;
	}
	else
	if ((md + t * nd) > dd)
	{
		// Intersection outside cylinder end cap
		if (nd >= 0.0f)
			return 0;			// Segment pointing away from cap
			
		t = (dd - md) / nd;

		if ((k + dd - 2.0f * md + t * (2.0f * (mn - nd) + t * nn)) > 0)
			return -1;
		
	}
	
	return t;
}

// ================================
//
// Sphere Routines
//
// ================================
float CzMath::SphereIntersectPlane(CzVec3& sphere_pos, CzVec3& sphere_vel, CzVec3& pop, CzVec3& normal, CzVec3& out, float radius)
{
	float t = -1.0f;
	float D = normal.Dot(pop);
	float dist = normal.Dot(sphere_pos) - D;
	
	// Sphere is overlapping the plane
	if (abs(dist) <= radius)
	{
		t = 0.0f;
		out.x = sphere_pos.x;
		out.y = sphere_pos.y;
		out.z = sphere_pos.z;
	}
	else
	{
		float denom = normal.Dot(sphere_vel);
		if (denom * dist >= 0.0f)
		{
			// No intersection as sphere is moving parallel to plane or away from plane
			return t;
		}
		else
		{
			if (dist < 0.0f)
				radius = -radius;
				
			t = (radius - dist) / denom;
			
			out.x = sphere_pos.x + (t * sphere_vel.x) - (radius * normal.x);
			out.y = sphere_pos.y + (t * sphere_vel.y) - (radius * normal.y);
			out.z = sphere_pos.z + (t * sphere_vel.z) - (radius * normal.z);
		}
	}

	return t;
}

bool CzMath::SphereIntersectCuboidTest(CzVec3& sphere_pos, float sphere_size, CzVec3& box_pos, CzVec3& box_size)
{
	float d = 0;
	
	float min_x = box_pos.x - (box_size.x / 2.0f);
	float max_x = box_pos.x + (box_size.x / 2.0f);
	float min_y = box_pos.y - (box_size.y / 2.0f);
	float max_y = box_pos.y + (box_size.y / 2.0f);
	float min_z = box_pos.z - (box_size.z / 2.0f);
	float max_z = box_pos.z + (box_size.z / 2.0f);
	float x = sphere_pos.x;
	float y = sphere_pos.y;
	float z = sphere_pos.z;
	
	// Calculate distance from the sphere centre to each point on the cube per axis
	if (x < min_x)
		d += (min_x - x) * (min_x - x);
	else
	if (x > max_x)
		d += (x - max_x) * (x - max_x);
		
	if (y < min_y)
		d += (min_y - y) * (min_y - y);
	else
	if (y > max_y)
		d += (y - max_y) * (y - max_y);
		
	if (z < min_z)
		d += (min_z - z) * (min_z - z);
	else
	if (z > max_z)
		d += (z - max_z) * (z - max_z);
		
	// Check to see if distanmce falls within spheres radius
	if (d < (sphere_size * sphere_size))	
	{
		return true;
	}
	
	return false;
}

// ================================
//
// Cuboid Routines
//
// ================================
int CzMath::CuboidIntersectTest(CzVec3& box1_pos, CzVec3& box1_size, CzVec3& box2_pos, CzVec3& box2_size)
{
	int axis = 0;
	
	float dx = abs(box1_pos.x - box2_pos.x);
	float dy = abs(box1_pos.y - box2_pos.y);
	float dz = abs(box1_pos.z - box2_pos.z);
	float sx = (box1_size.x + box2_size.x) / 2.0f;
	float sy = (box1_size.y + box2_size.y) / 2.0f;
	float sz = (box1_size.z + box2_size.z) / 2.0f;
	
	if (dx < sx)
		axis |= 1;
	if (dy < sy)
		axis |= 2;
	if (dz < sz)
		axis |= 4;
		
	return axis;	
}

// ================================
//
// Miscellaneous
//
// ================================
CzVec3 CzMath::CalcSlideVelocity(CzVec3& original_pos, CzVec3& original_vel, CzVec3& pop, CzVec3& normal, CzVec3& cp)
{
	CzVec3 new_vel;
	CzVec3 pp;
	CzVec3 np = original_pos + original_vel;
	PointProjectOntoPlane(np, pop, normal, pp);

	return pp - cp;		// cp = collision point
}

// ================================
//
// 2D routines
//
// ================================

