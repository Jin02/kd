#pragma once

class float4x4;

class float3
{
public:
	float3(void) = default;
	float3(float x, float y, float z);

public:
	const float3 operator + () const;
	const float3 operator - () const;

	const float3 operator - (const float3& a) const;
	const float3 operator + (const float3& a) const;
	bool operator == (const float3& b) const;
	bool operator != (const float3& b) const;
	const float3 operator * (float d) const;
	const float3 operator * (const float3& a) const;
	const float3 operator / (float d) const;

	float3& operator += (const float3& a);
	float3& operator -= (const float3& a);
	float3& operator *= (const float3& a);
	float3& operator *= (float f);
	float3& operator /= (float f);

	float& operator[](unsigned int idx);
	const float operator[](unsigned int idx) const;

	friend float3 operator+(float lhs, const float3& rhs) { return float3(lhs + rhs.x, lhs + rhs.y, lhs + rhs.z); }
	friend float3 operator-(float lhs, const float3& rhs) { return float3(lhs - rhs.x, lhs - rhs.y, lhs - rhs.z); }
	friend float3 operator*(float lhs, const float3& rhs) { return float3(lhs * rhs.x, lhs * rhs.y, lhs * rhs.z); }
	friend float3 operator/(float lhs, const float3& rhs) { return float3(lhs / rhs.x, lhs / rhs.y, lhs / rhs.z); }


public:
	static const float3 Forward();
	static const float3 Right();
	static const float3 One();
	static const float3 Up();
	static const float3 Zero();
	static const float3 Cross(const float3& a, const float3& b);
	static const float3 Lerp(const float3& from, const float3& to, float t);
	static const float3 Min(const float3& a, const float3& b);
	static const float3 Max(const float3& a, const float3& b);
	static const float3 Normalize(const float3& value);
	static const float3 Project(const float3& vector, const float3& onNormal);
	static const float3 Reflect(const float3& inDirection, const float3& inNormal);

	static float AnglebyDirect(const float3& from, const float3& to, bool radian = true);
	static float AnglebyPoint(const float3& from, const float3& to, bool radian = true);
	static float Distance(const float3& a, const float3& b);
	static float Dot(const float3& a, const float3& b);
	static float Length(const float3& a);
	static float SqrLegnth(const float3& a);

	static float3 TransformCoord(const float3& v, const float4x4& mat);
	static float3 TransformNormal(const float3& v, const float4x4& mat);
	static float3 FromRotationMatrix(const float4x4& rotMat);
	static float3 EulerNormalize(const float3& euler);

public:
	const float3 Normalized() const;

	float Length() const;
	float Dot(const float3& v) const;
	void Set(float newX, float newY, float newZ);

public:
	union
	{
		struct
		{
			float x, y, z;
		};

		float value[3] = { 0.0f, };
	};
};