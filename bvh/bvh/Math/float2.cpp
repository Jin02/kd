#include "float2.h"
#include <float.h>
#include <math.h>
#include "Common.h"

float2::float2(float _x, float _y) : x(_x), y(_y)
{
}

const float2 float2::operator + () const
{
	return (*this);
}

const float2 float2::operator - () const
{
	return float2(-x, -y);
}

const float2 float2::operator - (const float2 &a) const
{
	return float2(x - a.x, y - a.y);
}

const float2 float2::operator + (const float2 &a) const
{
	return float2(a.x + x, a.y + y);
}

bool float2::operator == (const float2 b) const
{
	return (fabsf(x - b.x) < FLT_EPSILON) & (fabsf(y - b.y) < FLT_EPSILON);
}

bool float2::operator != (const float2 &b) const
{
	return !float2::operator==(b);
}

const float2 float2::operator * (float d) const
{
	return float2(x * d, y * d);
}

const float2 float2::operator * (const float2 &a) const
{
	return float2(a.x * x, a.y * y);
}

float float2::operator[](unsigned int idx) const
{
	return value[idx];
}

const float2 float2::operator / (float d) const
{
	return float2(x / d, y / d);
}

const float2 float2::Zero()
{
	return float2(0, 0);
}

const float2 float2::One()
{
	return float2(1, 1);
}

float float2::AnglebyPoint(const float2 &from, const float2 &to, bool radian)
{
	float x = to.x - from.x;
	float y = to.y - from.y;

	return radian ? atan2f(y, x) : RAD_TO_DEG(atan2f(y, x));
}

float float2::AnglebyDirect(const float2 &from, const float2 &to, bool radian)
{
	return radian ? acosf(float2::Dot(from, to)) : RAD_TO_DEG(acosf(float2::Dot(from, to)));
}

float float2::Distance(const float2 &a, const float2 &b)
{
	float sx = (b.x - a.x) * (b.x - a.x);
	float sy = (b.y - a.y) * (b.y - a.y);

	return sqrtf(sx + sy);
}

float float2::Dot(const float2 &a, const float2 &b)
{
	return (a.x * b.x) + (a.y * b.y);
}

const float2 float2::Lerp(const float2 &from, const float2 &to, float t)
{
	float2 a = to - from;
	return a * t;
}

const float2 float2::Min(const float2 &a, const float2 &b)
{
	float x = a.x > b.x ? b.x : a.x;
	float y = a.y > b.y ? b.y : a.y;

	return float2(x, y);
}

const float2 float2::Max(const float2 &a, const float2 &b)
{
	float x = a.x > b.x ? a.x : b.x;
	float y = a.y > b.y ? a.y : b.y;

	return float2(x, y);
}

float float2::Length(const float2 &a)
{
	return sqrtf((a.x * a.x) + (a.y + a.y));
}

const float2 float2::Normalize(const float2 &value)
{
	float v = float2::Length(value);
	return value / v;
}

float float2::SqrLength(const float2 &a)
{
	return (a.x * a.x) + (a.y + a.y);
}

const float2 float2::Normalized() const
{
	return float2::Normalize(*this);
}

void float2::Set(float newX, float newY)
{
	x = newX;
	y = newY;
}

float2& float2::operator += (const float2& a)
{
	x += a.x;
	y += a.y;

	return (*this);
}

float2& float2::operator -= (const float2& a)
{
	return operator+=(-a);
}

float2& float2::operator *= (const float2& a)
{
	x *= a.x;
	y *= a.y;

	return (*this);
}

float2& float2::operator *= (float f)
{
	x *= f;
	y *= f;

	return (*this);
}

float2& float2::operator /= (float f)
{
	return operator*=(1.0f / f);
}