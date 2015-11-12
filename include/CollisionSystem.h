#ifndef COLLISION_SYSTEM
#define COLLISION_SYSTEM

#include "System.h"
#include "ResourceManager.h"

class CollisionSystem : public System<CollisionSystem>
{
	virtual ~CollisionSystem() {}
	virtual void Update(EntityManager* pManager, float dt, float tt) override;
};

#endif

