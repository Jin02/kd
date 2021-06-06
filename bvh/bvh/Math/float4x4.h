#pragma once

#include "Common.h"
#include "Rect.h"
#include "float2.h"
#include "float3.h"
#include "float4.h"

class quat;

class float4x4
{
public:
	float4x4() = default;
	float4x4(float _11, float _12, float _13, float _14,
		float _21, float _22, float _23, float _24,
		float _31, float _32, float _33, float _34,
		float _41, float _42, float _43, float _44);

public:
	bool operator != (const float4x4& mat) const;
	bool operator == (const float4x4& mat) const;

	float4x4& operator+= (const float4x4& mat);
	float4x4& operator-= (const float4x4& mat);
	float4x4& operator*= (const float4x4& mat);
	float4x4& operator*= (float f);
	float4x4& operator/= (float f);

	const float4x4 operator+ (const float4x4& mat) const;
	const float4x4 operator- (const float4x4& mat) const;
	const float4x4 operator* (const float4x4& mat);
	const float4x4 operator* (float f) const;
	const float4x4 operator/ (float f) const;
	const float4x4 operator* (const float4x4& mat) const;

	const float2 operator* (const float2& v) const;
	const float3 operator* (const float3& v) const;
	const float4 operator* (const float4& v) const;

	friend float4x4	operator*(float lhs, const float4x4& rhs) { return rhs * lhs; }
	friend float4	operator*(const float4& lhs, const float4x4& rhs) { return rhs * lhs; }
	friend float3	operator*(const float3& lhs, const float4x4& rhs) { return rhs * lhs; }
	friend float4& operator*=(float4& lhs, const float4x4& rhs) { lhs = rhs * lhs; return lhs; }
	friend float3& operator*=(float3& lhs, const float4x4& rhs) { lhs = rhs * lhs; return lhs; }

public:
	static float4x4 RotateUsingQuaternion(const quat& q);
	static float4x4 PerspectiveFovLH(float aspect, float fovy, float zn, float zf);
	static float4x4 OrthoLH(float w, float h, float zn, float zf);
	static float4x4 RotateUsingAxis(const float3& v, float angle);
	static float4x4 ComputeViewMatrix(const float4x4& worldMatrix);
	static float4x4 ComputeViewportMatrix(const Rect<uint32>& rect);
	static float4x4 ComputeInvViewportMatrix(const Rect<uint32>& rect);
	static float4x4 MakeRotationMatrix(const float3& right, const float3& up, const float3& forward);
	static float4x4 LookAtDir(const float3& targetDir, const float3* upVec = nullptr);
	static float4x4 ComputeViewProjMatrix(const float3& eyePos, const float3& forward, const float3& up, const float4x4& projMat);

public:
	void Set(float _11, float _12, float _13, float _14,
		float _21, float _22, float _23, float _24,
		float _31, float _32, float _33, float _34,
		float _41, float _42, float _43, float _44);

	static float4x4 Multiply(const float4x4& lhs, const float4x4& rhs);
	static float4x4 Transpose(const float4x4& mat);
	static float4x4 Inverse(const float4x4& mat); 		//using cramer's rule
	static float4x4 Identity();

public:
	union
	{
		struct
		{
			float        _11, _12, _13, _14;
			float        _21, _22, _23, _24;
			float        _31, _32, _33, _34;
			float        _41, _42, _43, _44;
		};

		float _m[4][4] = { 0.0f, };
	};
};