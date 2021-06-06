#include "Plane.h"
#include <math.h>

Plane::Plane(float _a, float _b, float _c, float _d)
	: a(_a), b(_b), c(_c), d(_d)
{

}

float Plane::DistancePoint(const float3& p) const
{
	return ComputeDistanceWithPoint((*this), p);
}

Plane Plane::FromPoints(const float3& v1, const float3& v2, const float3& v3)
{
	float3 edge1	= v2 - v1;
	float3 edge2	= v3 - v1;
	float3 normal	= float3::Normalize( float3::Cross(edge1, edge2) );

	return FromPointNormal(v1, normal);
}

Plane Plane::FromPointNormal(const float3& point, const float3& normal)
{
	return Plane(normal.x, normal.y, normal.z, -float3::Dot(point, normal));
}

float Plane::DotCoord(const Plane& p, const float3& v)
{
	return DotNoraml(p, v) + p.d;
}

float Plane::DotNoraml(const Plane& p, const float3& v)
{
	return (p.a * v.x) + (p.b * v.y) + (p.c * v.z);				 
}

Plane Plane::Normalize(const Plane& p)
{
	float norm = sqrtf( (p.a * p.a) + (p.b * p.b) + (p.c * p.c) );
	return (norm != 0.0f) ? Plane( p.a / norm, p.b / norm, p.c / norm, p.d / norm ) : Plane(0, 0, 0, 0);
}

float Plane::ComputeDistanceWithPoint(const Plane& p, const float3& v)
{
	return fabsf( DotCoord(p, v) );
}

bool Plane::SameSide(const Plane& p, const float3& v)
{
	return ComputeDistanceWithPoint(p, v) == 0.0f;
}

Plane::Direction Plane::GetSide(const Plane& p, const float3& v)
{
	float dist = DotCoord(p, v);
	
	if( dist < 0.0f )	return Direction::BACK;
	else if( dist > 0.0f )	return Direction::FRONT;
	return Direction::SAME;
}

float Plane::GetDistancePoint(const float3& v) const
{
	return ComputeDistanceWithPoint(*this, v);
}

bool Plane::SameSide(const float3& v) const
{
	return SameSide(*this, v);
}

Plane::Direction Plane::GetSide(const float3& v) const
{
	return GetSide(*this, v);
}
