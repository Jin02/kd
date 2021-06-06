#include "quat.h"
#include "float3.h"
#include "float4x4.h"

#include <math.h>
#include <float.h>

quat::quat(float _x, float _y, float _z, float _w)
	: x(_x), y(_y), z(_z), w(_w)
{
}

const quat quat::operator + () const
{
	return (*this);
}
const quat quat::operator - () const
{
	return quat(-x, -y, -z, -w);
}

const quat quat::operator + (const quat& a) const
{
	return quat(x + a.x, y + a.y, z + a.z, w + a.w).Normalized();
}
const quat quat::operator - (const quat& a) const
{
	return quat(x - a.x, y - a.y, z - a.z, w - a.w).Normalized();
}
const quat quat::operator * (const quat& a) const
{
	return quat(	(w * a.x) + (x * a.w) + (y * a.z) - (z * a.y),
						(w * a.y) + (y * a.w) + (z * a.x) - (x * a.z),
						(w * a.z) + (z * a.w) + (x * a.y) - (y * a.x),
						(w * a.w) - (x * a.x) - (y * a.y) - (z * a.z)	);
}

const quat quat::operator * (float f) const
{
	return quat(x * f, y * f, z * f, w * f);
}

const quat quat::operator / (float f) const
{
	return quat(x / f, y / f, z / f, w / f);
}

bool quat::operator == (const quat& a) const
{
	quat q = (*this) - a;
	return (fabsf(q.x) < FLT_EPSILON) & (fabsf(q.y) < FLT_EPSILON) & (fabsf(q.z) < FLT_EPSILON) & (fabsf(q.w) < FLT_EPSILON);
}
bool quat::operator != (const quat& a) const
{
	return !operator==(a);
}

quat& quat::operator += (const quat& a)
{
	x += a.x;
	y += a.y;
	z += a.z;
	w += a.w;

	(*this) = Normalize(*this);
	return (*this);
}

quat& quat::operator -= (const quat& a)
{
	x -= a.x;
	y -= a.y;
	z -= a.z;
	w -= a.w;

	(*this) = Normalize(*this);
	return (*this);
}

quat& quat::operator *= (const quat& q)
{
	x = (w * q.x) + (x * q.w) + (y * q.z) - (z * q.y);
	y = (w * q.y) + (y * q.w) + (z * q.x) - (x * q.z);
	z = (w * q.z) + (z * q.w) + (x * q.y) - (y * q.x);
	w = (w * q.w) - (x * q.x) - (y * q.y) - (z * q.z);

	(*this) = Normalize(*this);
	return (*this);
}

quat quat::Inverse(const quat& in)
{
	float sq = (in.x * in.x) + (in.y + in.y) + (in.z * in.z) + (in.w * in.w);
	float inv_norm = 1.0f / sq;

	return quat(-in.x * inv_norm, -in.y * inv_norm, -in.z * inv_norm, -in.w * inv_norm);
}

quat quat::Normalize(const quat& in)
{
	float sq = (in.x * in.x) + (in.y * in.y) + (in.z * in.z) + (in.w * in.w);
	float dst = 1.0f / sqrtf(sq);

	return quat(in.x * dst, in.y * dst, in.z * dst, in.w * dst);
}

float quat::Dot(const quat& a, const quat& b)
{
	return (a.x * b.x) + (a.y * b.y) + (a.z * b.z) + (a.w * b.w);
}

quat quat::Slerp(const quat& from, const quat& to, float t)
{
	float fromWeight, toWeight;

	float diff = (from.x * to.x) + (from.y * to.y) + (from.z * to.z) + (from.w * to.w);

	if ((1.0f - fabsf(diff)) > FLT_EPSILON)
	{		
		float theta = acosf(fabsf(diff));
		float oneOverSinTheta = 1.0f / sinf(theta);

		fromWeight = sinf(theta * (1.0f - t)) * oneOverSinTheta;
		toWeight = sinf(theta * t) * oneOverSinTheta;

		if (diff < 0.0f)
			fromWeight *= -1.0f;
	}
	else
	{
		fromWeight = 1.0f - t;
		toWeight = t;
	}

	return quat(	(from.x * fromWeight) + (to.x  * toWeight),
						(from.y * fromWeight) + (to.y  * toWeight),
						(from.z * fromWeight) + (to.z  * toWeight),
						(from.w * fromWeight) + (to.w  * toWeight) ).Normalized();
}

quat quat::Lerp(const quat& from, const quat& to, float t)
{
	float diff = (from.x * to.x) + (from.y * to.y) + (from.z * to.z) + (from.w * to.w);

	float fromWeight = 1.0f - t;
	float toWeight = t;

	return quat(	(from.x * fromWeight) + (to.x * toWeight),
						(from.y * fromWeight) + (to.y * toWeight),
						(from.z * fromWeight) + (to.z * toWeight),
						(from.w * fromWeight) + (to.w * toWeight)	);
}

quat quat::FromEuler(const float3& v)
{
	return FromYawPitchRoll(v.y, v.x, v.z);
}

void quat::Set(float x, float y, float z, float w)
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}

quat quat::FromRotationMatrix(const float4x4& m)
{
	quat out;

	float s = 0.0f;
	float trace = m._m[0][0] + m._m[1][1] + m._m[2][2] + 1.0f;

	if (trace > 1.0f)
	{
		s = 2.0f * sqrtf(trace);
		out.x = (m._m[1][2] - m._m[2][1]) / s;
		out.y = (m._m[2][0] - m._m[0][2]) / s;
		out.z = (m._m[0][1] - m._m[1][0]) / s;
		out.w = 0.25f * s;
	}
	else
	{
		int i, maxi = 0;

		for (i = 1; i < 3; i++)
		{
			if (m._m[i][i] > m._m[maxi][maxi])
				maxi = i;
		}

		switch (maxi)
		{
		case 0:
			s = 2.0f * sqrtf(1.0f + m._m[0][0] - m._m[1][1] - m._m[2][2]);
			out.x = 0.25f * s;
			out.y = (m._m[0][1] + m._m[1][0]) / s;
			out.z = (m._m[0][2] + m._m[2][0]) / s;
			out.w = (m._m[1][2] - m._m[2][1]) / s;
			break;

		case 1:
			s = 2.0f * sqrtf(1.0f + m._m[1][1] - m._m[0][0] - m._m[2][2]);
			out.x = (m._m[0][1] + m._m[1][0]) / s;
			out.y = 0.25f * s;
			out.z = (m._m[1][2] + m._m[2][1]) / s;
			out.w = (m._m[2][0] - m._m[0][2]) / s;
			break;

		case 2:
			s = 2.0f * sqrtf(1.0f + m._m[2][2] - m._m[0][0] - m._m[1][1]);
			out.x = (m._m[0][2] + m._m[2][0]) / s;
			out.y = (m._m[1][2] + m._m[2][1]) / s;
			out.z = 0.25f * s;
			out.w = (m._m[0][1] - m._m[1][0]) / s;
			break;
		}
	}

	return out;
}

quat quat::FromYawPitchRoll(float yaw, float pitch, float roll)
{
	float syaw		= sinf(yaw / 2.0f);
	float cyaw		= cosf(yaw / 2.0f);
	float spitch	= sinf(pitch / 2.0f);
	float cpitch	= cosf(pitch / 2.0f);
	float sroll		= sinf(roll / 2.0f);
	float croll		= cosf(roll / 2.0f);

	return quat(	syaw * cpitch * sroll + cyaw * spitch * croll,
						syaw * cpitch * croll - cyaw * spitch * sroll,
						cyaw * cpitch * sroll - syaw * spitch * croll,
						cyaw * cpitch * croll + syaw * spitch * sroll	);
}

quat quat::FromAxis(const float3& axis, float angle)
{
	float3 temp = float3::Normalize(axis);

	return quat(	sinf(angle / 2.0f) * temp.x,
						sinf(angle / 2.0f) * temp.y,
						sinf(angle / 2.0f) * temp.z,
						cosf(angle / 2.0f)	);
}

const quat quat::Normalized()
{
	return quat::Normalize(*this);
}

const quat quat::Inversed()
{
	return Inverse(*this);
}

const float3 quat::GetForward() const
{
	return float3(	2.0f * (x * z - y * w),
					2.0f * (y *z + x * w),
					1.0f - 2.0f * (x * x + y * y)	);
}

const float3 quat::GetRight() const
{
	return float3(	1.0f - 2.0f * (y * y + z * z),
					2.0f * (x * y - z * w),
					2.0f * (x * z + y * w)	);
}

const float3 quat::GetUp() const
{
	return float3(	2.0f * (x *y + z * w),
					1.0f - 2.0f * (x * x + z * z),
					2.0f * (y *z - x * w)	);
}
