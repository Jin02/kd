#pragma once

#include "Common/Common.h"
#include "float3.h"
#include <vector>

struct KdNode
{
	float3 _bbMin;
	float3 _bbMax;

	uint32 _primitiveIndex = 0xffffffff;
	uint32 _nextNodeIndex = 0xffffffff;
};

struct RawKdNodeData : public KdNode
{
	float3 _center;

	uint32 _orderIndex = 0xffffffff;
	uint32 _parentNodeIndex = 0xffffffff;

	uint32 _leftNodeIndex = 0xffffffff;
	uint32 _rightNodeIndex = 0xffffffff;

	float _primitiveArea = 0.0f;
	float _surfaceAreaLeft = 0.0f;
	float _surfaceAreaRight = 0.0f;
};

struct PackedKdNode
{
	float3	_parameter0;
	uint32	_parameter1;
};

class KdTree
{
public:
	void build(std::vector<PackedKdNode>& outPackedNodeArray, const void* vertices, uint32 stride, const uint32* indices, const uint32 indexCount);
	
private:
	uint32 buildInternal(std::vector<RawKdNodeData>& nodeArray, const uint32 beginIndex, const uint32 endIndex);
	void buildBoundBox(float3& out_bbMin, float3& out_bbMax, const std::vector<RawKdNodeData>& nodeArray, uint32 beginIndex, uint32 endIndex);
	void buildNodeOrder(std::vector<RawKdNodeData>& nodeArray, const uint32 rootNodeIndex);
	
private:
	std::vector<KdNode> _nodeArray;
};

