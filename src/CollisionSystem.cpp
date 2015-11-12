#include "CollisionSystem.h"
#include <EntityManager.h>
#include <CollisionComponent.h>
#include <TransformComponent.h>

void CollisionSystem::Update(EntityManager* pManager, float dt, float tt) {
	std::vector<GameEntity> collidibles = pManager->EntitiesWithComponents<CollisionComponent>();
	std::vector<GameEntity> toDestroy = std::vector<GameEntity>();

	for (GameEntity ge : collidibles) {
		pManager->GetComponent<CollisionComponent>(ge)->collider.SetPosition(pManager->GetComponent<TransformComponent>(ge)->transform.GetTranslation());
		pManager->GetComponent<CollisionComponent>(ge)->collider.IsColliding(false);
	}

	GameEntity e1;
	GameEntity e2;

	for (int i = 0; i < collidibles.size(); i++) {
		for (int j = 0; j < collidibles.size(); j++) {
			if (i != j) {
				e1 = collidibles[i];
				e2 = collidibles[j];

				if (pManager->GetComponent<CollisionComponent>(e1)->collider.CollidesWith(pManager->GetComponent<CollisionComponent>(e2)->collider)) {
					pManager->GetComponent<CollisionComponent>(e1)->collider.IsColliding(true);
					pManager->GetComponent<CollisionComponent>(e1)->collider.IsColliding(true);

					if ((e1.GetTag() == "Projectile" && e2.GetTag() == "Asteroid") ||
						(e1.GetTag() == "Asteroid" && e2.GetTag() == "Projectile")) {

						toDestroy.push_back(e1);
						toDestroy.push_back(e2);
					}
				}
			}
		}
	}

	for (int i = 0; i < toDestroy.size(); i++) {
		pManager->Destroy(toDestroy[i]);
	}
}
