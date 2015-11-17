#include "OctTree.h"

OctTree::OctTree() {
	maxEntities = 0;
	partitionedEntities = std::vector<std::vector<GameEntity>>();
}

OctTree::OctTree(int mE)
	: maxEntities(mE)
{
	partitionedEntities = std::vector<std::vector<GameEntity>>();
}

void OctTree::Update(std::vector<GameEntity> entities, Octant initial) {
	partitionedEntities.clear();
}

int OctTree::GetMaxEntities() {
	return maxEntities;
}

std::vector<std::vector<GameEntity>> OctTree::GetPartitionedEntities() {
	return partitionedEntities;
}

void OctTree::PartitionSpace(std::vector<GameEntity> entities, Octant o) {

}