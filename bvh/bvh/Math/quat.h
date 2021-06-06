#pragma once


class float3;
class float4x4;

class quat
{
public:
	quat() = default;
	quat(float x, float y, float z, float w);

	const quat operator + () const;
	const quat operator - () const;
	const quat operator + (const quat& q) const;
	const quat operator - (const quat& q) const;
	const quat operator * (const quat& q) const;
	const quat operator * (float f) const;
	const quat operator / (float f) const;

	bool operator == (const quat& q) const;
	bool operator != (const quat& q) const;

	quat& operator += (const quat& a);
	quat& operator -= (const quat& a);
	quat& operator *= (const quat& a);

	static quat Inverse(const quat& in);
	static quat Normalize(const quat& in);
	static quat Slerp(const quat& from, const quat& to, float t);
	static quat Lerp(const quat& from, const quat& to, float t);
	static quat FromEuler(const float3& v);

	static quat FromRotationMatrix(const float4x4& m);
	static quat FromAxis(const float3& axis, float angle);
	static quat FromYawPitchRoll(float yaw, float pitch, float roll);

	static float Dot(const quat& a, const quat& b);

public:
	void Set(float x, float y, float z, float w);
	const quat Normalized();
	const quat Inversed();
	const float3 GetForward() const;
	const float3 GetRight() const;
	const float3 GetUp() const;

public:
	float	x = 0.0f,
		y = 0.0f,
		z = 0.0f,
		w = 0.0f;
};