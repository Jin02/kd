#include "float4.h"
#include "Common.h"
#include <assert.h>
#include <math.h>
#include <float.h>

float4::float4(float _x, float _y, float _z, float _w)
	: x(_x), y(_y), z(_z), w(_w)
{
}

const float4 float4::operator + () const
{
	return (*this);
}

const float4 float4::operator - () const
{
	return float4(-x, -y, -z, -w);
}

const float4 float4::operator - (const float4& a) const
{
	return float4(x - a.x, y - a.y, z - a.z, w - a.w);
}

const float4 float4::operator + (const float4& a) const
{
	return float4(	a.x + x,
					a.y + y,
					a.z + z,
					a.w + w);
}

bool float4::operator == (const float4& b) const
{
	return (fabsf(x - b.x) < FLT_EPSILON) & (fabsf(y - b.y) < FLT_EPSILON) & (fabsf(z - b.z) < FLT_EPSILON) & (fabsf(w - b.w) < FLT_EPSILON);
}

bool float4::operator != (const float4& b) const
{
	return !float4::operator==(b);
}

const float4 float4::operator * (float d) const
{
	return float4(x * d, y * d, z * d, w * d);
}

float4& float4::operator *= (float f)
{
	x *= f; y *= f; z *= f; w *= f;
	return (*this);
}

float4& float4::operator *= (const float4& a)
{
	x *= a.x; y *= a.y; z *= a.z; w *= a.w;
	return (*this);
}

float4& float4::operator -= (const float4& a)
{
	x -= a.x; y -= a.y; z -= a.z; w -= a.z;
	return (*this);
}

const float4 float4::operator / (float d) const
{
	return float4(x / d, y / d, z / d, w / d);
}

float float4::operator[](unsigned int idx) const
{
	return value[idx];
}

float4 float4::One()
{
	return float4(1, 1, 1, 1);
}

float4 float4::Zero()
{
	return float4(0, 0, 0, 0);
}

float float4::Distance(const float4& a, const float4& b)
{
	float sx = (b.x - a.x) * (b.x - a.x);
	float sy = (b.y - a.y) * (b.y - a.y);
	float sz = (b.z - a.z) * (b.z - a.z);
	float sw = (b.w - a.w) * (b.w - a.w);

	return sqrtf(sx + sy + sz + sw);
}

float float4::Dot(const float4& a, const float4& b)
{
	return (a.x * b.x) + (a.y * b.y) + (a.z * b.z) + (a.w * b.w);
}

const float4 float4::Lerp(const float4& from, const float4& to, float t)
{
	float4 a = to - from;
	return (a * t) + from;
}

const float4 float4::Min(const float4& a, const float4& b)
{
	float x = a.x > b.x ? b.x : a.x;
	float y = a.y > b.y ? b.y : a.y;
	float z = a.z > b.z ? b.z : a.z;
	float w = a.w > b.w ? b.w : a.w;

	return float4(x, y, z, w);
}

const float4 float4::Max(const float4& a, const float4& b)
{
	float x = a.x > b.x ? a.x : b.x;
	float y = a.y > b.y ? a.y : b.y;
	float z = a.z > b.z ? a.z : b.z;
	float w = a.w > b.w ? a.w : b.w;

	return float4(x, y, z, w);
}

float float4::Legnth(const float4& a)
{
	return sqrtf((a.x * a.x) + (a.y + a.y) + (a.z * a.z) + (a.w * a.w));
}

const float4 float4::Normalize(const float4& value)
{
	float v = float4::Legnth(value);
	return value / v;
}

float float4::SqrLegnth(const float4& a)
{
	return (a.x * a.x) + (a.y + a.y) + (a.z * a.z) + (a.w * a.w);
}

const float4 float4::Normalized() const
{
	return float4::Normalize(*this);
}

void float4::Set(float newX, float newY, float newZ, float newW)
{
	x = newX;
	y = newY;
	z = newZ;
	w = newW;
}

float4& float4::operator += (const float4& a)
{
	x += a.x; y += a.y; z += a.z; w += a.z;
	return (*this);
}

float4& float4::operator /= (float f)
{
	x /= f; y /= f; z /= f; w /= f;
	return (*this);
}