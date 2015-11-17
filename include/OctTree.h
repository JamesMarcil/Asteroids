#ifndef OCT_TREE
#define OCT_TREE

#include "GameEntity.h"
#include <DirectXMath.h>
#include <vector>

struct Octant {
	DirectX::XMFLOAT3 position;
	float width;
	float height;
	float depth;

	Octant(DirectX::XMFLOAT3 p, float w, float h, float d, int mE)
		: position(p), width(w), height(h), depth(d)
	{}
};

class OctTree
{
private:
	int maxEntities;										  // Max number of entities per partition
	std::vector<std::vector<GameEntity>> partitionedEntities; // List holding sub-lists of entities in their respective partitions

public:
	OctTree();
	OctTree(int mE);

	void Update(std::vector<GameEntity> entities, Octant initial);

	/* Getters */
	int GetMaxEntities();
	std::vector<std::vector<GameEntity>> GetPartitionedEntities();

private:
	void PartitionSpace(std::vector<GameEntity> entities, Octant o);
};

#endif