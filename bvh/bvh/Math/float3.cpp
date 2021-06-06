#include "float3.h"
#include "float4x4.h"

#include "Common.h"
#include <assert.h>
#include <math.h>
#include <float.h>

float3::float3(float _x, float _y, float _z)
	: x(_x), y(_y), z(_z)
{
}

const float3 float3::operator + () const
{
	return (*this);
}

const float3 float3::operator - () const
{
	return float3(-x, -y, -z);
}

const float3 float3::operator - (const float3& a) const
{
	return float3(x - a.x, y - a.y, z - a.z);
}

const float3 float3::operator + (const float3& a) const
{
	return float3(a.x + x, a.y + y, a.z + z);
}

bool float3::operator == (const float3& b) const
{
	return (fabsf(x - b.x) < FLT_EPSILON) & (fabsf(y - b.y) < FLT_EPSILON) & (fabsf(z - b.z) < FLT_EPSILON);
}

bool float3::operator != (const float3& b) const
{
	return !float3::operator==(b);
}

const float3 float3::operator * (float d) const
{
	return float3(x * d, y * d, z * d);
}

const float3 float3::operator * (const float3& a) const
{
	return float3(a.x * x, a.y * y, a.z * z);
}

const float3 float3::operator / (float d) const
{
	return float3(x / d, y / d, z / d);
}

float& float3::operator[](unsigned int idx)
{
	return value[idx];
}

const float float3::operator[](unsigned int idx) const
{
	return value[idx];
}

float3& float3::operator += (const float3& a)
{
	x += a.x;
	y += a.y;
	z += a.z;

	return (*this);
}

float3& float3::operator -= (const float3& a)
{
	x -= a.x;
	y -= a.y;
	z -= a.z;

	return (*this);
}

float3& float3::operator *= (const float3& a)
{
	x *= a.x;
	y *= a.y;
	z *= a.z;

	return (*this);
}

float3& float3::operator *= (float f)
{
	x *= f; y *= f; z *= f;
	return (*this);
}

float3& float3::operator /= (float f)
{
	x /= f; y /= f; z /= f;
	return (*this);
}

const float3 float3::Forward()
{
	return float3(0, 0, 1);
}

const float3 float3::Right()
{
	return float3(1, 0, 0);
}

const float3 float3::One()
{
	return float3(1, 1, 1);
}

const float3 float3::Up()
{
	return float3(0, 1, 0);
}

const float3 float3::Zero()
{
	return float3(0, 0, 0);
}

float float3::AnglebyDirect(const float3& from, const float3& to, bool radian)
{
	float dot = float3::Dot(from, to);
	return radian ? acosf(dot) : RAD_TO_DEG(acosf(dot));
}

float float3::AnglebyPoint(const float3& from, const float3& to, bool radian)
{
	float sx = (from.x - to.x) * (from.x - to.x);
	float sy = (from.y - to.y) * (from.y - to.y);
	float z = (from.z - to.z);
	float distanceXtoY = sqrtf(sx + sy);

	return radian ? atan2f(z, distanceXtoY) : RAD_TO_DEG(atan2f(z, distanceXtoY));
}

const float3 float3::Cross(const float3& a, const float3& b)
{
	return float3(a.y * b.z - b.y * a.z, a.z * b.x - b.z * a.x, a.x * b.y - b.x * a.y);
}

float float3::Distance(const float3& a, const float3& b)
{
	float sx = (b.x - a.x) * (b.x - a.x);
	float sy = (b.y - a.y) * (b.y - a.y);
	float sz = (b.z - a.z) * (b.z - a.z);

	return sqrtf(sx + sy + sz);
}

float float3::Dot(const float3& a, const float3& b)
{
	return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
}

const float3 float3::Lerp(const float3& from, const float3& to, float t)
{
	float3 a = to - from;
	return (a * t) + from;
}

const float3 float3::Min(const float3& a, const float3& b)
{
	float x = a.x > b.x ? b.x : a.x;
	float y = a.y > b.y ? b.y : a.y;
	float z = a.z > b.z ? b.z : a.z;

	return float3(x, y, z);
}

const float3 float3::Max(const float3& a, const float3& b)
{
	float x = a.x > b.x ? a.x : b.x;
	float y = a.y > b.y ? a.y : b.y;
	float z = a.z > b.z ? a.z : b.z;

	return float3(x, y, z);
}

float float3::Length(const float3& a)
{
	return sqrtf((a.x * a.x) + (a.y * a.y) + (a.z * a.z));
}

const float3 float3::Normalize(const float3& value)
{
	float v = float3::Length(value);
	assert(v > 0.0f);
	return value / v;
}

const float3 float3::Project(const float3& vector, const float3& onNormal)
{
	float dot = float3::Dot(vector, onNormal);
	float t = dot / float3::SqrLegnth(onNormal);
	return vector * t;
}

const float3 float3::Reflect(const float3& inDirection, const float3& inNormal)
{
	float dot = float3::Dot(inDirection, inNormal);

	float x = -2.0f * dot * inNormal.x;
	float y = -2.0f * dot * inNormal.y;
	float z = -2.0f * dot * inNormal.z;

	return inDirection - float3(x, y, z);
}

float float3::SqrLegnth(const float3& a)
{
	return (a.x * a.x) + (a.y + a.y) + (a.z * a.z);
}

const float3 float3::Normalized() const
{
	return float3::Normalize(*this);
}

float float3::Length() const
{
	return float3::Length((*this));
}

float float3::Dot(const float3& v) const
{
	return float3::Dot((*this), v);
}

void float3::Set(float newX, float newY, float newZ)
{
	x = newX;
	y = newY;
	z = newZ;
}

float3 float3::TransformCoord(const float3& v, const float4x4& mat)
{
	float3 src = v;
	float w = mat._m[0][3] * src.x + mat._m[1][3] * src.y + mat._m[2][3] * src.z + mat._m[3][3];

	if (w == 0.0f)
		return float3(0.0f, 0.0f, 0.0f);

	return float3(
	(mat._m[0][0] * src.x + mat._m[1][0] * src.y + mat._m[2][0] * src.z + mat._m[3][0]) / w,
	(mat._m[0][1] * src.x + mat._m[1][1] * src.y + mat._m[2][1] * src.z + mat._m[3][1]) / w,
	(mat._m[0][2] * src.x + mat._m[1][2] * src.y + mat._m[2][2] * src.z + mat._m[3][2]) / w );
}

float3 float3::TransformNormal(const float3& v, const float4x4& mat)
{
	float3 src = v;

	return float3(
	(mat._m[0][0] * src.x + mat._m[1][0] * src.y + mat._m[2][0] * src.z),
	(mat._m[0][1] * src.x + mat._m[1][1] * src.y + mat._m[2][1] * src.z),
	(mat._m[0][2] * src.x + mat._m[1][2] * src.y + mat._m[2][2] * src.z) );
}

//http://www.gamedev.net/topic/643623-how-do-i-get-the-euler-angle-from-a-matrix/
float3 float3::FromRotationMatrix(const float4x4& matrix)
{
	float3 dotx = float3(matrix._11, matrix._12, matrix._13);
	float3 doty = float3(matrix._21, matrix._22, matrix._23);
	float3 dotz = float3(matrix._31, matrix._32, matrix._33);

	float x = float3::Dot(dotx, dotx);

	auto Approximately = [](float a, float b) -> bool
	{
		return fabsf(a - b) < FLT_EPSILON;
	};

	if (Approximately(x, 1.0f) == false)
	{
		float invx = 1.0f / sqrtf(x);
		dotx = dotx*invx;
	}

	float y = float3::Dot(doty, doty);
	if (Approximately(y, 1.0f) == false)
	{
		float invy = 1.0f / sqrtf(y);
		doty = doty*invy;
	}

	float z = float3::Dot(dotz, dotz);
	if (Approximately(z, 1.0f) == false)
	{
		float invz = 1.0f / sqrtf(z);
		dotz = dotz*invz;
	}

	float thetaX = 0;
	float thetaY = 0;
	float thetaZ = 0;
	if (dotz.y < 1.0f)
	{
		if (dotz.y > -1.0f)
		{
			thetaX = asinf(-dotz.y);
			thetaY = atan2f(dotz.x, dotz.z);
			thetaZ = atan2f(dotx.y, doty.y);
		}
		else
		{
			thetaX = MATH_PI*0.5f;
			thetaY = -atan2f(-doty.x, dotx.x);
			thetaZ = 0;
		}
	}
	else
	{
		thetaX = -MATH_PI * 0.5f;
		thetaY = atan2f(-doty.x, dotx.x);
		thetaZ = 0;
	}

	return float3::EulerNormalize(float3(thetaX, thetaY, thetaZ));
}

float3 float3::EulerNormalize(const float3& euler)
{
	int x = (int)(euler.x / 360.0f);
	int y = (int)(euler.y / 360.0f);
	int z = (int)(euler.z / 360.0f);

	return float3(	euler.x - x * 360.0f + (int)(euler.x < 0) * 360.0f,
					euler.y - y * 360.0f + (int)(euler.y < 0) * 360.0f,
					euler.z - z * 360.0f + (int)(euler.z < 0) * 360.0f);
}