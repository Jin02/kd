#include "KdTree.h"
#include <algorithm>

const float kMaxBoxLength = 1000000.0f;

static float3 getVertex(const void* vertices, uint32 vertexIndex, uint32 stride)
{
	const float3* position = reinterpret_cast<const float3*>(reinterpret_cast<const char*>(vertices) + (vertexIndex * stride));
	return *position;
}

static void float3Min(float3& source, const float3& rhs)
{
	source.x = std::min(source.x, rhs.x);
	source.y = std::min(source.y, rhs.y);
	source.z = std::min(source.z, rhs.z);
}

static void float3Max(float3& source, const float3& rhs)
{
	source.x = std::max(source.x, rhs.x);
	source.y = std::max(source.y, rhs.y);
	source.z = std::max(source.z, rhs.z);
}

void KdTree::buildBoundBox(float3& out_bbMin, float3& out_bbMax, const std::vector<RawKdNodeData>& nodeArray, uint32 beginIndex, uint32 endIndex)
{
	if (beginIndex == endIndex)
	{
		out_bbMin = float3(0.0f, 0.0f, 0.0f);
		out_bbMax = float3(0.0f, 0.0f, 0.0f);
		return;
	}

	out_bbMin = float3(kMaxBoxLength, kMaxBoxLength, kMaxBoxLength);
	out_bbMax = -out_bbMin;

	for (uint32 i = beginIndex; i < endIndex; ++i)
	{
		float3Min(out_bbMin, nodeArray[i]._bbMin);
		float3Max(out_bbMax, nodeArray[i]._bbMax);
	}
}

uint32 KdTree::buildInternal(std::vector<RawKdNodeData>& nodeArray, const uint32 beginIndex, const uint32 endIndex)
{
	const uint32 count = endIndex - beginIndex;
	if (1 == count)
	{
		return beginIndex;
	}

	float3 bbMin, bbMax;
	buildBoundBox(bbMin, bbMax, nodeArray, beginIndex, endIndex);

	uint32 midIndex = (endIndex - 1);
	{
		float3 extensts = bbMax - bbMin;

		uint32 dominantAxisIndex = 0xffffffff;

		if (extensts.x >= extensts.y && extensts.x >= extensts.z)		dominantAxisIndex = 0;
		else if (extensts.y >= extensts.x && extensts.y >= extensts.z)	dominantAxisIndex = 1;
		else if (extensts.z >= extensts.x && extensts.z >= extensts.y)	dominantAxisIndex = 2;
		assert(0xffffffff != dominantAxisIndex);

		std::sort(nodeArray.begin() + beginIndex, nodeArray.begin() + endIndex,
			[axisIndex = dominantAxisIndex](const RawKdNodeData& lhs, const RawKdNodeData& rhs)
			{
				return lhs._center[axisIndex] < rhs._center[axisIndex];
			});

		const float splitPos = (bbMin[dominantAxisIndex] + bbMax[dominantAxisIndex]) * 0.5f;
		for (uint32 i = beginIndex + 1; i < endIndex; ++i)
		{
			if (splitPos <= nodeArray[i]._center[dominantAxisIndex])
			{
				midIndex = i;
				break;
			}
		}
	}
	
	RawKdNodeData newNode;
	newNode._leftNodeIndex = buildInternal(nodeArray, beginIndex, midIndex);
	newNode._rightNodeIndex = buildInternal(nodeArray, midIndex, endIndex);

	auto computeSurfaceArea = [](const RawKdNodeData& nodeData)
	{
		const float3 extents = (nodeData._bbMax - nodeData._bbMin);
		return (extents.x * extents.y + extents.y * extents.z + extents.x * extents.z) * 2.0f;
	};

	const float leftNodeSurfaceArea = computeSurfaceArea(nodeArray[newNode._leftNodeIndex]);
	const float rightNodeSurfaceArea = computeSurfaceArea(nodeArray[newNode._rightNodeIndex]);

	// Note(jinpark) : left 먼저 접근할거라서 left node는 arae가 더 큰 노드로 설정함.
	if (leftNodeSurfaceArea < rightNodeSurfaceArea)
	{
		std::swap(newNode._leftNodeIndex, newNode._rightNodeIndex);
	}

	newNode._bbMin = bbMin;
	newNode._bbMax = bbMax;
	newNode._center = (bbMin + bbMax) * 0.5f;
	newNode._primitiveIndex = 0xffffffff;
	
	const uint32 newNodeIndex = static_cast<uint32>(nodeArray.size());
	nodeArray[newNode._leftNodeIndex]._parentNodeIndex = newNodeIndex;
	nodeArray[newNode._rightNodeIndex]._parentNodeIndex = newNodeIndex;
	nodeArray.push_back(newNode);

	return newNodeIndex;
}

static void buildNodeOrderInternal(std::vector<RawKdNodeData>& nodeArray, uint32 nodeIndex, uint32 nodeNextIndex, uint32& order)
{
	RawKdNodeData& node = nodeArray[nodeIndex];
	
	node._orderIndex	= order++;
	node._nextNodeIndex = nodeNextIndex;

	if (0xffffffff != node._leftNodeIndex)
	{
		buildNodeOrderInternal(nodeArray, node._leftNodeIndex, node._rightNodeIndex, order);
	}
	
	if (0xffffffff != node._rightNodeIndex)
	{
		buildNodeOrderInternal(nodeArray, node._rightNodeIndex, node._nextNodeIndex, order);
	}
}

void KdTree::buildNodeOrder(std::vector<RawKdNodeData>& nodeArray, const uint32 rootNodeIndex)
{
	uint32 order = 0;
	buildNodeOrderInternal(nodeArray, rootNodeIndex, 0xffffffff, order);
}

void KdTree::build(std::vector<PackedKdNode>& outPackedNodeArray, const void* vertices, uint32 stride, const uint32* indices, const uint32 indexCount)
{
	assert(0 == (indexCount % 3));
	const uint32 primitiveCount = indexCount / 3;
	
	std::vector<RawKdNodeData> rawNodeDataArray;

	// Note(jinpark) : 1 step - build primitive node
	for (uint32 primitiveIndex = 0; primitiveIndex < primitiveCount; ++primitiveIndex)
	{
		float3 boxMin = float3(kMaxBoxLength, kMaxBoxLength, kMaxBoxLength);
		float3 boxMax = -boxMin;

		float3 positions[] = {	getVertex(vertices, indices[primitiveIndex * 3 + 0], stride),
								getVertex(vertices, indices[primitiveIndex * 3 + 1], stride) ,
								getVertex(vertices, indices[primitiveIndex * 3 + 2], stride) };

		for (uint32 i = 0; i < 3; ++i)
		{
			float3Min(boxMin, positions[i]);
			float3Max(boxMax, positions[i]);
		}

		RawKdNodeData primitiveNode;
		primitiveNode._bbMin = boxMin;
		primitiveNode._bbMax = boxMax;
		primitiveNode._primitiveIndex = primitiveIndex;
		primitiveNode._center = (boxMin + boxMax) * 0.5f;
		primitiveNode._primitiveArea = float3::Cross(positions[2] - positions[0], positions[1] - positions[0]).Length() * 0.5f;
		rawNodeDataArray.push_back(primitiveNode);
	}

	const uint32 primitiveNodeCount = static_cast<uint32>(rawNodeDataArray.size());
	const uint32 rootNodeIndex = buildInternal(rawNodeDataArray, 0, primitiveNodeCount);

	// Note(jinpark) : 2 step - build order index
	buildNodeOrder(rawNodeDataArray, rootNodeIndex);

	// Note(jinpark) : 3 step - raw 데이터 가공, kdNode로 전환
	std::vector<KdNode> kdNodeArray;
	{
		kdNodeArray.resize(rawNodeDataArray.size());

		const uint32 rawNodeCount = rawNodeDataArray.size();
		for (uint32 rawNodeIndex = 0; rawNodeIndex < rawNodeCount; ++rawNodeIndex)
		{
			const RawKdNodeData& rawNode = rawNodeDataArray[rawNodeIndex];

			KdNode& newNode = kdNodeArray[rawNode._orderIndex];

			newNode._bbMin = rawNode._bbMin;
			newNode._bbMax = rawNode._bbMax;
			newNode._primitiveIndex = rawNode._primitiveIndex;

			newNode._nextNodeIndex = 0xffffffff;
			if (0xffffffff != rawNode._nextNodeIndex)
			{
				newNode._nextNodeIndex = rawNodeDataArray[rawNode._nextNodeIndex]._orderIndex;
			}
		}
	}

	std::vector<PackedKdNode> packedNodeArray;

	const uint32 kdNodeCount = kdNodeArray.size();
	for (uint32 kdNodeIndex = 0; kdNodeIndex < kdNodeCount; ++kdNodeIndex)
	{
		const KdNode& kdNode = kdNodeArray[kdNodeIndex];
		const bool isLeafNode = (0xffffffff != kdNode._primitiveIndex);
		if (true == isLeafNode)
		{
			float3 positions[] = {	getVertex(vertices, indices[kdNode._primitiveIndex * 3 + 0], stride),
									getVertex(vertices, indices[kdNode._primitiveIndex * 3 + 1], stride) ,
									getVertex(vertices, indices[kdNode._primitiveIndex * 3 + 2], stride) };

			PackedKdNode primitiveNode;

			// Note(jinpark) : leaf node면 primitive primitive므로 edge 데이터만 중점으로 담음
			float3 edge0 = positions[1] - positions[0];

			// Note(jinpark) : packed data 크기가 16byte, primitive position 0 담는 문제때문에 2배.
			uint32 primitiveIndex = kdNode._primitiveIndex + kdNodeCount * 2;

			primitiveNode._parameter0 = edge0;	// Note(jinpark) : edge가 아니라 position1 넘겨도 되지않나?
			primitiveNode._parameter1 = primitiveIndex;
			packedNodeArray.push_back(primitiveNode);

			float3 edge1 = positions[2] - positions[0];
			uint32 nextNodeIndex = kdNode._nextNodeIndex;
			primitiveNode._parameter0 = edge1;
			primitiveNode._parameter1 = nextNodeIndex;
			packedNodeArray.push_back(primitiveNode);
		}
		else
		{
			PackedKdNode newPackedKdNode;
			newPackedKdNode._parameter0 = kdNode._bbMin;
			newPackedKdNode._parameter1 = kdNode._primitiveIndex;
			packedNodeArray.push_back(newPackedKdNode);

			newPackedKdNode._parameter0 = kdNode._bbMax;
			newPackedKdNode._parameter1 = kdNode._nextNodeIndex;
			packedNodeArray.push_back(newPackedKdNode);
		}
	}

	for (uint32 primitiveIndex = 0; primitiveIndex < primitiveCount; ++primitiveIndex)
	{
		const float3 position0 = getVertex(vertices, indices[primitiveIndex * 3 + 0], stride);
		
		PackedKdNode packedData;
		packedData._parameter0 = position0;
		packedData._parameter1 = 0;
		packedNodeArray.push_back(packedData);
	}

	outPackedNodeArray = static_cast<std::vector<PackedKdNode>&&>(packedNodeArray);
}