#include "CollisionSystem.h"
#include <EntityManager.h>
#include <CollisionComponent.h>
#include <TransformComponent.h>

void CollisionSystem::Update(EntityManager* pManager, float dt, float tt) {
	std::vector<GameEntity> collidibles = pManager->EntitiesWithComponents<CollisionComponent>();

	for (GameEntity ge : collidibles) {
		pManager->GetComponent<CollisionComponent>(ge)->collider.SetPosition(pManager->GetComponent<TransformComponent>(ge)->transform.GetTranslation());
		pManager->GetComponent<CollisionComponent>(ge)->collider.IsColliding(false);
	}

	CollisionSphere* s1;
	CollisionSphere* s2;

	for (int i = 0; i < collidibles.size(); i++) {
		for (int j = 0; j < collidibles.size(); j++) {
			if (i != j) {
				s1 = &pManager->GetComponent<CollisionComponent>(collidibles[i])->collider;
				s2 = &pManager->GetComponent<CollisionComponent>(collidibles[j])->collider;

				if (s1->CollidesWith(*s2)) {
					s1->IsColliding(true);
					s2->IsColliding(true);
				}
			}
		}
	}
}
