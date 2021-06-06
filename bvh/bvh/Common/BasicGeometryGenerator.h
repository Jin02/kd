#pragma once

#include <functional>
#include <vector>

#include "Common.h"
#include "float2.h"
#include "float3.h"

struct PrimitiveBuffer
{
	std::vector<float3> _vertexBuffer;
	std::vector<uint32> _indexBuffer;
};

class BasicGeometryGenerator
{
public:
	static PrimitiveBuffer CreateBox(const float3& size);
	static PrimitiveBuffer CreateSphere(float radius, uint32 sliceCount, uint32 stackCount);
};