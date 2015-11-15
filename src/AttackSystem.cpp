#include "AttackSystem.h"

// Managers
#include "EntityManager.h"
#include "InputManager.h"
#include "ResourceManager.h"

// Component
#include "TransformComponent.h"
#include "RenderComponent.h"
#include "InputComponent.h"
#include "PhysicsComponent.h"
#include "AttackComponent.h"
#include "CollisionComponent.h"

// DirectX
#include <DirectXMath.h>

AttackSystem::AttackSystem(void)
{
    ResourceManager* pResource = ResourceManager::Instance();
    projectileCollider = CollisionSphere(*pResource->GetMesh("Helix"), DirectX::XMFLOAT3(0,0,0), 0.1f);
}

void AttackSystem::Update(EntityManager* pManager, float dt, float tt)
{
    InputManager* pInput = InputManager::Instance();
	if (pInput->IsMousePressed(MouseButton::LMB))
    {
		FireProjectile(pManager);
	}
}

void AttackSystem::FireProjectile(EntityManager* pEntity)
{
	ResourceManager* pResource = ResourceManager::Instance();

	GameEntity player = pEntity->EntitiesWithComponents<InputComponent, AttackComponent>()[0];
    TransformComponent* pPlayerTransform = pEntity->GetComponent<TransformComponent>(player);

	GameEntity projectile = pEntity->Create("Projectile");
	pEntity->AddComponent<RenderComponent>(projectile, pResource->GetMaterial("default"), pResource->GetMesh("Helix"));
	pEntity->AddComponent<CollisionComponent>(projectile, projectileCollider.GetRadius(), pPlayerTransform->transform.GetTranslation());
	pEntity->AddComponent<PhysicsComponent>(projectile, DirectX::XMFLOAT3(0, 0, 10), DirectX::XMFLOAT3(0, 0, 5));
	TransformComponent* pTransform = pEntity->AddComponent<TransformComponent>(projectile, pPlayerTransform->transform.GetTranslation());
	pTransform->transform.SetScale(0.1f);
	pTransform->transform.Rotate(3.14159f / 2, 0, 0);
}
