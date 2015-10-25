#include "PhysicsSystem.h"

#include "EntityManager.h" // Need to to include this to access EntityManager::* functions.

// Components
#include "TransformComponent.h"
#include "PhysicsComponent.h"

// DirectX
#include <DirectXMath.h>

void PhysicsSystem::Update(EntityManager* pManager, float dt, float tt )
{
    using namespace DirectX;

    for(auto& entity : pManager->EntitiesWithComponents<PhysicsComponent, TransformComponent>())
    {
        TransformComponent* pTransform = pManager->GetComponent<TransformComponent>(entity);
        Transform& t = pTransform->transform;

        PhysicsComponent* pPhysics = pManager->GetComponent<PhysicsComponent>(entity);

        XMVECTOR velocity = XMLoadFloat3(&pPhysics->velocity);
        XMVECTOR acceleration = XMLoadFloat3(&pPhysics->acceleration);

        velocity = XMVectorAdd(velocity, XMVectorScale(acceleration, dt));
        t.Translate(XMVectorScale(velocity, dt));

        XMStoreFloat3(&pPhysics->velocity, velocity);
        XMStoreFloat3(&pPhysics->acceleration, acceleration);
    }
}
