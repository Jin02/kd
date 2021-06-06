#pragma once


class float2
{
public:
	float2() = default;
	float2(float x, float y);

public:
	const float2 operator + () const;
	const float2 operator - () const;

	const float2 operator - (const float2& a) const;
	const float2 operator + (const float2& a) const;
	bool operator == (const float2 b) const;
	bool operator != (const float2& b) const;
	const float2 operator * (float d) const;
	const float2 operator * (const float2& a) const;
	const float2 operator / (float d) const;

	float2& operator += (const float2& a);
	float2& operator -= (const float2& a);
	float2& operator *= (const float2& a);
	float2& operator *= (float f);
	float2& operator /= (float f);

	float operator[](unsigned int idx) const;

	friend float2 operator+(float lhs, const float2& rhs) { return float2(lhs + rhs.x, lhs + rhs.y); }
	friend float2 operator-(float lhs, const float2& rhs) { return float2(lhs - rhs.x, lhs - rhs.y); }
	friend float2 operator*(float lhs, const float2& rhs) { return float2(lhs * rhs.x, lhs * rhs.y); }
	friend float2 operator/(float lhs, const float2& rhs) { return float2(lhs / rhs.x, lhs / rhs.y); }

public:
	static const float2 Zero();
	static const float2 One();
	static float AnglebyPoint(const float2& from, const float2& to, bool radian = true);
	static float AnglebyDirect(const float2& from, const float2& to, bool radian = true);
	static float Distance(const float2& a, const float2& b);
	static float Dot(const float2& a, const float2& b);
	static const float2 Lerp(const float2& from, const float2& to, float t);
	static const float2 Min(const float2& a, const float2& b);
	static const float2 Max(const float2& a, const float2& b);
	static float Length(const float2& a);
	static const float2 Normalize(const float2& value);
	static float SqrLength(const float2& a);

public:
	const float2 Normalized() const;
	void Set(float newX, float newY);

public:
	union
	{
		struct
		{
			float x, y;
		};

		float value[2] = { 0.0f };
	};
};