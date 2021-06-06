#pragma once


class float4
{
public:
	float4() = default;
	float4(float x, float y, float z, float w);

public:
	const float4 operator + () const;
	const float4 operator - () const;

	const float4 operator - (const float4& a) const;
	const float4 operator + (const float4& a) const;
	bool operator == (const float4& b) const;
	bool operator != (const float4& b) const;
	const float4 operator * (float d) const;
	const float4 operator / (float d) const;

	float4& operator += (const float4& a);
	float4& operator -= (const float4& a);
	float4& operator *= (const float4& a);
	float4& operator *= (float f);
	float4& operator /= (float f);

	float operator[](unsigned int idx) const;

	friend float4 operator+(float lhs, const float4& rhs) { return float4(lhs + rhs.x, lhs + rhs.y, lhs + rhs.z, lhs + rhs.w); }
	friend float4 operator-(float lhs, const float4& rhs) { return float4(lhs - rhs.x, lhs - rhs.y, lhs - rhs.z, lhs - rhs.w); }
	friend float4 operator*(float lhs, const float4& rhs) { return float4(lhs * rhs.x, lhs * rhs.y, lhs * rhs.z, lhs * rhs.w); }
	friend float4 operator/(float lhs, const float4& rhs) { return float4(lhs / rhs.x, lhs / rhs.y, lhs / rhs.z, lhs / rhs.w); }

public:
	static float4 One();
	static float4 Zero();

public:
	static const float4 Lerp(const float4& from, const float4& to, float t);
	static const float4 Min(const float4& a, const float4& b);
	static const float4 Max(const float4& a, const float4& b);
	static const float4 Normalize(const float4& value);

	static float Distance(const float4& a, const float4& b);
	static float Dot(const float4& a, const float4& b);

	static float Legnth(const float4& a);
	static float SqrLegnth(const float4& a);

public:
	const float4 Normalized() const;
	void Set(float newX, float newY, float newZ, float newW);

public:
	union
	{
		struct
		{
			float x, y, z, w;
		};

		float value[4] = { 0.0f, };
	};
};