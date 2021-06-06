#include <iostream>

#include "KdTree.h"
#include "BasicGeometryGenerator.h"

int main()
{
	PrimitiveBuffer primitiveBuffer = BasicGeometryGenerator::CreateSphere(10.0f, 32, 32);

	std::vector<PackedKdNode> packedNodeArray;

	KdTree kdTree;
	kdTree.build(packedNodeArray, primitiveBuffer._vertexBuffer.data(), sizeof(float3), primitiveBuffer._indexBuffer.data(), primitiveBuffer._indexBuffer.size());

	{

	}

	return 0;
}