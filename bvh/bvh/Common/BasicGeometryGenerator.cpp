#include "BasicGeometryGenerator.h"
#include <vector>

PrimitiveBuffer BasicGeometryGenerator::CreateBox(const float3& size)
{
	const uint32 vtxCount = 24;

	std::vector<float3> vertexDatas;

	float halfWidth		= 0.5f * size.x;
	float halfHeight	= 0.5f * size.y;
	float halfDepth		= 0.5f * size.z;

	// Fill in the front face vertex data.
	vertexDatas.push_back(float3(-halfWidth, -halfHeight, -halfDepth));
	vertexDatas.push_back(float3(-halfWidth, +halfHeight, -halfDepth));
	vertexDatas.push_back(float3(+halfWidth, +halfHeight, -halfDepth));
	vertexDatas.push_back(float3(+halfWidth, -halfHeight, -halfDepth));

	// Fill in the back face vertex data.
	vertexDatas.push_back(float3(-halfWidth, -halfHeight, +halfDepth));
	vertexDatas.push_back(float3(+halfWidth, -halfHeight, +halfDepth));
	vertexDatas.push_back(float3(+halfWidth, +halfHeight, +halfDepth));
	vertexDatas.push_back(float3(-halfWidth, +halfHeight, +halfDepth));

	// Fill in the top face vertex data.
	vertexDatas.push_back(float3(-halfWidth, +halfHeight, -halfDepth));
	vertexDatas.push_back(float3(-halfWidth, +halfHeight, +halfDepth));
	vertexDatas.push_back(float3(+halfWidth, +halfHeight, +halfDepth));
	vertexDatas.push_back(float3(+halfWidth, +halfHeight, -halfDepth));

	// Fill in the bottom face vertex data.
	vertexDatas.push_back(float3(-halfWidth, -halfHeight, -halfDepth));
	vertexDatas.push_back(float3(+halfWidth, -halfHeight, -halfDepth));
	vertexDatas.push_back(float3(+halfWidth, -halfHeight, +halfDepth));
	vertexDatas.push_back(float3(-halfWidth, -halfHeight, +halfDepth));

	// Fill in the left face vertex data.
	vertexDatas.push_back(float3(-halfWidth, -halfHeight, +halfDepth));
	vertexDatas.push_back(float3(-halfWidth, +halfHeight, +halfDepth));
	vertexDatas.push_back(float3(-halfWidth, +halfHeight, -halfDepth));
	vertexDatas.push_back(float3(-halfWidth, -halfHeight, -halfDepth));

	// Fill in the right face vertex data.
	vertexDatas.push_back(float3(+halfWidth, -halfHeight, -halfDepth));
	vertexDatas.push_back(float3(+halfWidth, +halfHeight, -halfDepth));
	vertexDatas.push_back(float3(+halfWidth, +halfHeight, +halfDepth));
	vertexDatas.push_back(float3(+halfWidth, -halfHeight, +halfDepth));

	//
	// Create the indices.
	//

	std::vector<uint32> indices;
	indices.resize(36);

	// Fill in the front face index data
	indices[0] = 0; indices[1] = 1; indices[2] = 2;
	indices[3] = 0; indices[4] = 2; indices[5] = 3;

	// Fill in the back face index data
	indices[6] = 4; indices[7] = 5; indices[8] = 6;
	indices[9] = 4; indices[10] = 6; indices[11] = 7;

	// Fill in the top face index data
	indices[12] = 8; indices[13] = 9; indices[14] = 10;
	indices[15] = 8; indices[16] = 10; indices[17] = 11;

	// Fill in the bottom face index data
	indices[18] = 12; indices[19] = 13; indices[20] = 14;
	indices[21] = 12; indices[22] = 14; indices[23] = 15;

	// Fill in the left face index data
	indices[24] = 16; indices[25] = 17; indices[26] = 18;
	indices[27] = 16; indices[28] = 18; indices[29] = 19;

	// Fill in the right face index data
	indices[30] = 20; indices[31] = 21; indices[32] = 22;
	indices[33] = 20; indices[34] = 22; indices[35] = 23;

	PrimitiveBuffer primitiveBuffer;
	primitiveBuffer._vertexBuffer = static_cast<std::vector<float3>&&>(vertexDatas);
	primitiveBuffer._indexBuffer = static_cast<std::vector<uint32>&&>(indices);
	return primitiveBuffer;
}

PrimitiveBuffer BasicGeometryGenerator::CreateSphere(float radius, uint32 sliceCount, uint32 stackCount)
{
	std::vector<float3> vertexDatas;
	vertexDatas.push_back(float3(0.0f, radius, 0.0f));

	float phiStep = MATH_PI / float(stackCount);
	float thetaStep = 2.0f * MATH_PI / float(sliceCount);

	for (uint32 i = 1; i <= stackCount - 1; ++i)
	{
		float phi = float(i) * phiStep;

		for (uint32 j = 0; j <= sliceCount; ++j)
		{
			float theta = float(j) * thetaStep;

			float3 position(	radius * sinf(phi) * cosf(theta),
								radius * cosf(phi),
								radius * sinf(phi) * sinf(theta)	);

			vertexDatas.push_back(position);
		}
	}

	vertexDatas.push_back(float3(0.0f, -radius, 0.0f));

	std::vector<uint32> indices;
	for (uint32 i = 1; i <= sliceCount; ++i)
	{
		indices.push_back(0);
		indices.push_back(i + 1);
		indices.push_back(i);
	}

	uint32 baseIndex = 1;
	uint32 ringVertexCount = sliceCount + 1;
	for (uint32 i = 0; i < stackCount - 2; ++i)
	{
		for (uint32 j = 0; j < sliceCount; ++j)
		{
			indices.push_back(baseIndex + i * ringVertexCount + j);
			indices.push_back(baseIndex + i * ringVertexCount + j + 1);
			indices.push_back(baseIndex + (i + 1) * ringVertexCount + j);

			indices.push_back(baseIndex + (i + 1) * ringVertexCount + j);
			indices.push_back(baseIndex + i * ringVertexCount + j + 1);
			indices.push_back(baseIndex + (i + 1) * ringVertexCount + j + 1);
		}
	}

	uint32 southPoleIndex = static_cast<uint32>(vertexDatas.size()) - 1;
	baseIndex = southPoleIndex - ringVertexCount;

	for (uint32 i = 0; i < sliceCount; ++i)
	{
		indices.push_back(southPoleIndex);
		indices.push_back(baseIndex + i);
		indices.push_back(baseIndex + i + 1);
	}

	PrimitiveBuffer primitiveBuffer;
	primitiveBuffer._vertexBuffer = static_cast<std::vector<float3>&&>(vertexDatas);
	primitiveBuffer._indexBuffer = static_cast<std::vector<uint32>&&>(indices);
	return primitiveBuffer;
}