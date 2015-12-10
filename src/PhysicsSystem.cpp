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
        PhysicsComponent* pPhysics = pManager->GetComponent<PhysicsComponent>(entity);
        TransformComponent* pTransform = pManager->GetComponent<TransformComponent>(entity);
        Transform& t = pTransform->transform;

		// Translation
		float drag = (1.0f - pPhysics->drag);
		XMVECTOR velocity = XMLoadFloat3(&pPhysics->velocity);
		XMVECTOR acceleration = XMLoadFloat3(&pPhysics->acceleration);
        XMVECTOR velocityLoss = XMVectorScale(velocity, drag);

		velocity = XMVectorSubtract(velocity, XMVectorScale(velocityLoss, dt));
		velocity = XMVectorAdd(velocity, XMVectorScale(acceleration, dt));
		t.Translate(XMVectorScale(velocity, dt));

		// Rotation
		float rotDrag = (1.0f - pPhysics->rotationalDrag);
		XMVECTOR rotVelocity = XMLoadFloat3(&pPhysics->rotationalVelocity);
		XMVECTOR rotAcceleration = XMLoadFloat3(&pPhysics->rotationalAcceleration);
        XMVECTOR rotVelocityLoss = XMVectorScale(rotVelocity, rotDrag);

		rotVelocity = XMVectorSubtract(rotVelocity, XMVectorScale(rotVelocityLoss, dt));
		rotVelocity = XMVectorAdd(rotVelocity, XMVectorScale(rotAcceleration, dt));
		t.Rotate(XMVectorScale(rotVelocity, dt));

		// Saving
        XMStoreFloat3(&pPhysics->velocity, velocity);
		XMStoreFloat3(&pPhysics->acceleration, acceleration);
		XMStoreFloat3(&pPhysics->rotationalVelocity, rotVelocity);
		XMStoreFloat3(&pPhysics->rotationalAcceleration, rotAcceleration);
    }
}
