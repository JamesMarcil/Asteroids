#include <AttackSystem.h>

#include <EntityManager.h>
#include <InputManager.h>
#include <ResourceManager.h>
#include <TransformComponent.h>
#include <RenderComponent.h>
#include <InputComponent.h>
#include <PhysicsComponent.h>
#include <AttackComponent.h>
#include <CollisionComponent.h>

AttackSystem::~AttackSystem() {
	delete projectileCollider; 
	projectileCollider = 0;
}

void AttackSystem::Update(EntityManager* pManager, float dt, float tt) {
	if (InputManager::Instance()->IsMousePressed(MouseButton::LMB)) {
		FireProjectile(pManager);
	}
}

void AttackSystem::FireProjectile(EntityManager* pManager) {
	ResourceManager* rManager = ResourceManager::Instance();
	GameEntity player = pManager->EntitiesWithComponents<InputComponent, AttackComponent>()[0];

	if (projectileCollider == nullptr) {
		projectileCollider = new CollisionSphere(*rManager->GetMesh("Helix"), DirectX::XMFLOAT3(0,0,0), 0.1f);
	}

	GameEntity projectile = pManager->Create("Projectile");
	pManager->AddComponent<RenderComponent>(projectile, rManager->GetMaterial("default"), rManager->GetMesh("Helix"));
	pManager->AddComponent<TransformComponent>(projectile, pManager->GetComponent<TransformComponent>(player)->transform.GetTranslation());
	pManager->GetComponent<TransformComponent>(projectile)->transform.SetScale(0.1f);
	pManager->GetComponent<TransformComponent>(projectile)->transform.Rotate(3.14159f / 2, 0, 0);
	pManager->AddComponent<CollisionComponent>(projectile, projectileCollider->GetRadius(), pManager->GetComponent<TransformComponent>(player)->transform.GetTranslation());
	pManager->AddComponent<PhysicsComponent>(projectile, DirectX::XMFLOAT3(0, 0, 10), DirectX::XMFLOAT3(0, 0, 5));
}