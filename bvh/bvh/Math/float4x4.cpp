#include "float4x4.h"

#include "float2.h"
#include "float3.h"
#include "float4.h"
#include "quat.h"

#include <math.h>
#include <float.h>
#include <memory>

float4x4::float4x4(	float __11, float __12, float __13, float __14,
				float __21, float __22, float __23, float __24,
				float __31, float __32, float __33, float __34,
				float __41, float __42, float __43, float __44	)
	: 	_11(__11), _12(__12), _13(__13), _14(__14),
		_21(__21), _22(__22), _23(__23), _24(__24),
		_31(__31), _32(__32), _33(__33), _34(__34),
		_41(__41), _42(__42), _43(__43), _44(__44)
{
}

bool float4x4::operator != (const float4x4& mat) const
{
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
		{
			float f = abs(_m[i][j] - mat._m[i][j]);

			if (f > FLT_EPSILON)
				return true;
		}

	return false;
}

bool float4x4::operator == (const float4x4& mat) const
{
	return !(operator!=(mat));
}

float4x4& float4x4::operator+= (const float4x4& mat)
{
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			_m[i][j] += mat._m[i][j];

	return *this;
}

float4x4& float4x4::operator-= (const float4x4& mat)
{
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			_m[i][j] -= mat._m[i][j];

	return *this;
}

float4x4& float4x4::operator*= (const float4x4& mat)
{
	(*this) = Multiply((*this), mat);
	return (*this);
}

float4x4& float4x4::operator*= (float f)
{
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
			_m[i][j] *= f;

	return (*this);
}

float4x4& float4x4::operator/= (float f)
{
	return operator*=(1.0f / f);
}

const float4x4 float4x4::operator+ (const float4x4& mat) const
{
	float4x4 res(*this);

	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
			res._m[i][j] += mat._m[i][j];

	return res;
}

const float4x4 float4x4::operator- (const float4x4& mat) const
{
	float4x4 res(*this);

	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
			res._m[i][j] -= mat._m[i][j];

	return res;
}

const float4x4 float4x4::operator* (const float4x4& mat)
{
	return Multiply((*this), mat);
}

const float4x4 float4x4::operator* (float f) const
{
	float4x4 res(*this);

	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
			res._m[i][j] *= f;

	return res;
}

const float4x4 float4x4::operator* (const float4x4& mat) const
{
	return 	float4x4::Multiply((*this), mat);
}

const float4x4 float4x4::operator/ (float f) const
{
	float4x4 res(*this);

	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
			res._m[i][j] /= f;

	return res;
}

const float2 float4x4::operator* (const float2& v) const
{
	float4 res(v.x, v.y, 1.0f, 1.0f);
	res = (*this) * res;

	return float2(res.x, res.y);
}

const float3 float4x4::operator* (const float3& v) const
{
	float4 res(v.x, v.y, v.z, 1.0f);
	res = (*this) * res;

	return float3(res.x, res.y, res.z);
}

const float4 float4x4::operator* (const float4& v) const
{
	float4 res(
		v.x * _m[0][0] + v.y * _m[1][0] + v.z * _m[2][0] + v.w * _m[3][0],
		v.x * _m[0][1] + v.y * _m[1][1] + v.z * _m[2][1] + v.w * _m[3][1],
		v.x * _m[0][2] + v.y * _m[1][2] + v.z * _m[2][2] + v.w * _m[3][2],
		v.x * _m[0][3] + v.y * _m[1][3] + v.z * _m[2][3] + v.w * _m[3][3]
	);

	return res;
}

float4x4 float4x4::Transpose(const float4x4& mat)
{
	return float4x4 (	mat._11, mat._21, mat._31, mat._41,
					mat._12, mat._22, mat._32, mat._42,
					mat._13, mat._23, mat._33, mat._43,
					mat._14, mat._24, mat._34, mat._44	);
}

void float4x4::Set(	float _11, float _12, float _13, float _14,
					float _21, float _22, float _23, float _24,
					float _31, float _32, float _33, float _34,
					float _41, float _42, float _43, float _44
)
{
	_m[0][0] = _11; _m[0][1] = _12; _m[0][2] = _13; _m[0][3] = _14;
	_m[1][0] = _21; _m[1][1] = _22; _m[1][2] = _23; _m[1][3] = _24;
	_m[2][0] = _31; _m[2][1] = _32; _m[2][2] = _33; _m[2][3] = _34;
	_m[3][0] = _41; _m[3][1] = _42; _m[3][2] = _43; _m[3][3] = _44;
}

float4x4 float4x4::Multiply(const float4x4& lhs, const float4x4& rhs)
{
	float4x4 res;
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			float f = 0.0f;
			for (int k = 0; k < 4; ++k)
			{
				f += lhs._m[i][k] * rhs._m[k][j];
			}

			res._m[i][j] = f;
		}
	}

	return res;
}

float4x4 float4x4::Identity()
{
	return float4x4(	1.0f, 0.0f, 0.0f, 0.0f,
					0.0f, 1.0f, 0.0f, 0.0f,
					0.0f, 0.0f, 1.0f, 0.0f,
					0.0f, 0.0f, 0.0f, 1.0f	);
}

float4x4 float4x4::Inverse(const float4x4& mat)
{
	float4x4 out;
	float4x4 src = float4x4::Transpose(mat);

	float tmpCofactors[12];
	/* calculate pairs for first 8 elements (cofactors) */
	tmpCofactors[0] = src._m[2][2] * src._m[3][3];
	tmpCofactors[1] = src._m[2][3] * src._m[3][2];
	tmpCofactors[2] = src._m[2][1] * src._m[3][3];
	tmpCofactors[3] = src._m[2][3] * src._m[3][1];
	tmpCofactors[4] = src._m[2][1] * src._m[3][2];
	tmpCofactors[5] = src._m[2][2] * src._m[3][1];
	tmpCofactors[6] = src._m[2][0] * src._m[3][3];
	tmpCofactors[7] = src._m[2][3] * src._m[3][0];
	tmpCofactors[8] = src._m[2][0] * src._m[3][2];
	tmpCofactors[9] = src._m[2][2] * src._m[3][0];
	tmpCofactors[10] = src._m[2][0] * src._m[3][1];
	tmpCofactors[11] = src._m[2][1] * src._m[3][0];

	/* calculate first 8 elements (cofactors) */
	out._m[0][0] = tmpCofactors[0] * src._m[1][1] + tmpCofactors[3] * src._m[1][2] + tmpCofactors[4] * src._m[1][3];
	out._m[0][0] -= tmpCofactors[1] * src._m[1][1] + tmpCofactors[2] * src._m[1][2] + tmpCofactors[5] * src._m[1][3];
	out._m[0][1] = tmpCofactors[1] * src._m[1][0] + tmpCofactors[6] * src._m[1][2] + tmpCofactors[9] * src._m[1][3];
	out._m[0][1] -= tmpCofactors[0] * src._m[1][0] + tmpCofactors[7] * src._m[1][2] + tmpCofactors[8] * src._m[1][3];
	out._m[0][2] = tmpCofactors[2] * src._m[1][0] + tmpCofactors[7] * src._m[1][1] + tmpCofactors[10] * src._m[1][3];
	out._m[0][2] -= tmpCofactors[3] * src._m[1][0] + tmpCofactors[6] * src._m[1][1] + tmpCofactors[11] * src._m[1][3];
	out._m[0][3] = tmpCofactors[5] * src._m[1][0] + tmpCofactors[8] * src._m[1][1] + tmpCofactors[11] * src._m[1][2];
	out._m[0][3] -= tmpCofactors[4] * src._m[1][0] + tmpCofactors[9] * src._m[1][1] + tmpCofactors[10] * src._m[1][2];
	out._m[1][0] = tmpCofactors[1] * src._m[0][1] + tmpCofactors[2] * src._m[0][2] + tmpCofactors[5] * src._m[0][3];
	out._m[1][0] -= tmpCofactors[0] * src._m[0][1] + tmpCofactors[3] * src._m[0][2] + tmpCofactors[4] * src._m[0][3];
	out._m[1][1] = tmpCofactors[0] * src._m[0][0] + tmpCofactors[7] * src._m[0][2] + tmpCofactors[8] * src._m[0][3];
	out._m[1][1] -= tmpCofactors[1] * src._m[0][0] + tmpCofactors[6] * src._m[0][2] + tmpCofactors[9] * src._m[0][3];
	out._m[1][2] = tmpCofactors[3] * src._m[0][0] + tmpCofactors[6] * src._m[0][1] + tmpCofactors[11] * src._m[0][3];
	out._m[1][2] -= tmpCofactors[2] * src._m[0][0] + tmpCofactors[7] * src._m[0][1] + tmpCofactors[10] * src._m[0][3];
	out._m[1][3] = tmpCofactors[4] * src._m[0][0] + tmpCofactors[9] * src._m[0][1] + tmpCofactors[10] * src._m[0][2];
	out._m[1][3] -= tmpCofactors[5] * src._m[0][0] + tmpCofactors[8] * src._m[0][1] + tmpCofactors[11] * src._m[0][2];

	/* calculate pairs for second 8 elements (cofactors) */
	tmpCofactors[0] = src._m[0][2] * src._m[1][3];
	tmpCofactors[1] = src._m[0][3] * src._m[1][2];
	tmpCofactors[2] = src._m[0][1] * src._m[1][3];
	tmpCofactors[3] = src._m[0][3] * src._m[1][1];
	tmpCofactors[4] = src._m[0][1] * src._m[1][2];
	tmpCofactors[5] = src._m[0][2] * src._m[1][1];
	tmpCofactors[6] = src._m[0][0] * src._m[1][3];
	tmpCofactors[7] = src._m[0][3] * src._m[1][0];
	tmpCofactors[8] = src._m[0][0] * src._m[1][2];
	tmpCofactors[9] = src._m[0][2] * src._m[1][0];
	tmpCofactors[10] = src._m[0][0] * src._m[1][1];
	tmpCofactors[11] = src._m[0][1] * src._m[1][0];

	/* calculate second 8 elements (cofactors) */
	out._m[2][0] = tmpCofactors[0] * src._m[3][1] + tmpCofactors[3] * src._m[3][2] + tmpCofactors[4] * src._m[3][3];
	out._m[2][0] -= tmpCofactors[1] * src._m[3][1] + tmpCofactors[2] * src._m[3][2] + tmpCofactors[5] * src._m[3][3];
	out._m[2][1] = tmpCofactors[1] * src._m[3][0] + tmpCofactors[6] * src._m[3][2] + tmpCofactors[9] * src._m[3][3];
	out._m[2][1] -= tmpCofactors[0] * src._m[3][0] + tmpCofactors[7] * src._m[3][2] + tmpCofactors[8] * src._m[3][3];
	out._m[2][2] = tmpCofactors[2] * src._m[3][0] + tmpCofactors[7] * src._m[3][1] + tmpCofactors[10] * src._m[3][3];
	out._m[2][2] -= tmpCofactors[3] * src._m[3][0] + tmpCofactors[6] * src._m[3][1] + tmpCofactors[11] * src._m[3][3];
	out._m[2][3] = tmpCofactors[5] * src._m[3][0] + tmpCofactors[8] * src._m[3][1] + tmpCofactors[11] * src._m[3][2];
	out._m[2][3] -= tmpCofactors[4] * src._m[3][0] + tmpCofactors[9] * src._m[3][1] + tmpCofactors[10] * src._m[3][2];
	out._m[3][0] = tmpCofactors[2] * src._m[2][2] + tmpCofactors[5] * src._m[2][3] + tmpCofactors[1] * src._m[2][1];
	out._m[3][0] -= tmpCofactors[4] * src._m[2][3] + tmpCofactors[0] * src._m[2][1] + tmpCofactors[3] * src._m[2][2];
	out._m[3][1] = tmpCofactors[8] * src._m[2][3] + tmpCofactors[0] * src._m[2][0] + tmpCofactors[7] * src._m[2][2];
	out._m[3][1] -= tmpCofactors[6] * src._m[2][2] + tmpCofactors[9] * src._m[2][3] + tmpCofactors[1] * src._m[2][0];
	out._m[3][2] = tmpCofactors[6] * src._m[2][1] + tmpCofactors[11] * src._m[2][3] + tmpCofactors[3] * src._m[2][0];
	out._m[3][2] -= tmpCofactors[10] * src._m[2][3] + tmpCofactors[2] * src._m[2][0] + tmpCofactors[7] * src._m[2][1];
	out._m[3][3] = tmpCofactors[10] * src._m[2][2] + tmpCofactors[4] * src._m[2][0] + tmpCofactors[9] * src._m[2][1];
	out._m[3][3] -= tmpCofactors[8] * src._m[2][1] + tmpCofactors[11] * src._m[2][2] + tmpCofactors[5] * src._m[2][0];

	/* calculate determinant */
	float det = src._m[0][0] * out._m[0][0] + src._m[0][1] * out._m[0][1] + src._m[0][2] * out._m[0][2] + src._m[0][3] * out._m[0][3];

	/* calculate matrix inverse */
	det = 1 / det;
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; ++j)
			out._m[i][j] *= det;

	return out;
}

float4x4 float4x4::RotateUsingQuaternion(const quat& q)
{	
	float qy2 = q.y * q.y;
	float qx2 = q.x * q.x;
	float qz2 = q.z * q.z;	
	
	return float4x4(
		1.0f - 2.0f * (qy2 + qz2),
		2.0f * (q.x * q.y - q.z * q.w),
		2.0f * (q.x * q.z + q.y * q.w),
		0.0f,

		2.0f * (q.x * q.y + q.z * q.w),		
		1.0f - 2.0f * (qx2 + qz2),
		2.0f * (q.y * q.z - q.x * q.w),
		0.0f,
				
		2.0f * (q.x * q.z - q.y * q.w),
		2.0f * (q.y * q.z + q.x * q.w),
		1.0f - 2.0f * (qx2 + qy2),
		0.0f,
		
		0.0f,
		0.0f,
		0.0f,
		1.0f	);
}

float4x4 float4x4::PerspectiveFovLH(float aspect, float fovy, float zn, float zf)
{
	float yScale = 1.0f / tanf(fovy / 2.0f);
	float xScale = yScale / aspect;

	return float4x4(	xScale,		0.0f,		0.0f,					0.0f,
					0.0f,		yScale,		0.0f,					0.0f,
					0.0f,		0.0f,		zf / (zf - zn),			1.0f,
					0.0f,		0.0f,		-zn * zf / (zf - zn),	0.0f	);
}

float4x4 float4x4::OrthoLH(float w, float h, float zn, float zf)
{
	return float4x4(	2.0f / w,	0.0f,		0.0f,				0.0f,
					0.0f,		2.0f / h,	0.0f,				0.0f,
					0.0f,		0.0f,		1.0f / (zf- zn),	0.0f,
					0.0f,		0.0f,		zn / (zn - zf),		1.0f	);
}

float4x4 float4x4::RotateUsingAxis(const float3& v, float angle)
{
	float sangle	= sinf(angle);
	float cangle	= cosf(angle);
	float cdiff		= 1.0f - cangle;
	float3 nv		= float3::Normalize(v);

	return float4x4(	cdiff * nv.x * nv.x + cangle,			cdiff * nv.y * nv.x + sangle * nv.z,	cdiff * nv.z * nv.x - sangle * nv.y,	0.0f,
					cdiff * nv.x * nv.y - sangle * nv.z,	cdiff * nv.y * nv.y + cangle,			cdiff * nv.z * nv.y + sangle * nv.x,	0.0f,
					cdiff * nv.x * nv.z + sangle * nv.y,	cdiff * nv.y * nv.z - sangle * nv.x,	cdiff * nv.z * nv.z + cangle,			0.0f,
					0.0f,									0.0f,									0.0f,									1.0f	);
}

float4x4 float4x4::ComputeViewMatrix(const float4x4& worldMatrix)
{
	float3 right		= float3(worldMatrix._11, worldMatrix._12, worldMatrix._13);
	float3 up			= float3(worldMatrix._21, worldMatrix._22, worldMatrix._23);
	float3 forward		= float3(worldMatrix._31, worldMatrix._32, worldMatrix._33);
	float3 worldPos	= float3(worldMatrix._41, worldMatrix._42, worldMatrix._43);

	float3 p			= float3(	-float3::Dot(right, worldPos),
									-float3::Dot(up, worldPos),
									-float3::Dot(forward, worldPos)	);

	return float4x4(
		right.x, up.x, forward.x, 0.0f,
		right.y, up.y, forward.y, 0.0f,
		right.z, up.z, forward.z, 0.0f,
		p.x, p.y, p.z, 1.0f
	);
}

float4x4 float4x4::ComputeViewportMatrix(const Rect<uint32>& rect)
{
	return float4x4(	float(rect.size.w) / 2.0f,			0.0f,								0.0f,	0.0f,
					0.0f,								-float(rect.size.h) / 2.0f,			0.0f,	0.0f,
					0.0f,								0.0f,								1.0f,	0.0f,
					float(rect.x + rect.size.w) / 2.0f,	float(rect.y + rect.size.h) / 2.0f,	0.0f,	1.0f	);
}

float4x4 float4x4::ComputeInvViewportMatrix(const Rect<uint32>& rect)
{
	float4x4 viewportMat = ComputeViewportMatrix(rect);
	return float4x4::Inverse(viewportMat);
}

inline float4x4 float4x4::MakeRotationMatrix(const float3& right, const float3& up, const float3& forward)
{
	return float4x4(	right.x,	right.y,	right.z,	0.0f,
					up.x,		up.y,		up.z,		0.0f,
					forward.x,	forward.y,	forward.z,	0.0f,
					0.0f,		0.0f,		0.0f,		1.0f	);
}

float4x4 float4x4::LookAtDir(const float3& targetDir, const float3 * upVec)
{
	float3 worldUp	= upVec ? *upVec : float3::Up();

	float3 forward	= targetDir.Normalized();
	float3 right	= float3::Cross(worldUp, forward);
	float3 up		= float3::Cross(forward, right);

	return float4x4::MakeRotationMatrix(right, up, forward);
}

float4x4 float4x4::ComputeViewProjMatrix(const float3& eyePos, const float3& forward, const float3& up, const float4x4& projMat)
{
	float4x4 view = float4x4::LookAtDir(forward, &up);

	view._41 = eyePos.x;
	view._42 = eyePos.y;
	view._43 = eyePos.z;
	view._44 = 1.0f;

	view = float4x4::ComputeViewMatrix(view);
	
	return view * projMat;
}
