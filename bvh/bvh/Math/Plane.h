#pragma once

#include "float3.h"

class Plane
{
public:
	enum class Direction { FRONT, BACK, SAME };
	Plane() = default;
	Plane(float a, float b, float c, float d);

	const Plane		Normalized() const { return Plane::Normalize((*this)); }
	float			DistancePoint(const float3& p) const;
	float			GetDistancePoint(const float3& v) const;
	bool			SameSide(const float3& v) const;
	Direction		GetSide(const float3& v) const;

	static Plane		FromPoints(const float3& v1, const float3& v2, const float3& v3);
	static Plane		FromPointNormal(const float3& point, const float3& normal);
	static Plane		Normalize(const Plane& p);
	static float		DotCoord(const Plane& p, const float3& v);
	static float		DotNoraml(const Plane& p, const float3& v);

	static float		ComputeDistanceWithPoint(const Plane& p, const float3& v);
	static bool			SameSide(const Plane& p, const float3& v);
	static Direction	GetSide(const Plane& p, const float3& v);

public:
	float	a = 0.0f,
			b = 0.0f,
			c = 0.0f,
			d = 0.0f;
};
