#ifndef COLLISION_COMPONENT_H
#define COLLISION_COMPONENT_H

#include <Component.h>
#include "CollisionSphere.h"

struct CollisionComponent : public Component<CollisionComponent>
{
	CollisionSphere collider;

	CollisionComponent(Mesh& m, DirectX::XMFLOAT3& p, float s)
		: collider(m, p, s)
	{

	}
	~CollisionComponent() {}
};

#endif