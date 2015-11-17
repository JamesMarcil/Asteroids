#include "CollisionSystem.h"

// Manager
#include "EntityManager.h"
#include "EventManager.h"

// Component
#include "CollisionComponent.h"
#include "TransformComponent.h"

void CollisionSystem::Update(EntityManager* pManager, float dt, float tt)
{
	std::vector<GameEntity> collidables = pManager->EntitiesWithComponents<CollisionComponent>();
	for (GameEntity ge : collidables)
    {
        CollisionComponent* pCollider = pManager->GetComponent<CollisionComponent>(ge);
        TransformComponent* pTransform = pManager->GetComponent<TransformComponent>(ge);

		pCollider->collider.SetPosition(pTransform->transform.GetTranslation());
		pCollider->collider.IsColliding(false);
	}

	std::vector<GameEntity> toDestroy;
	for (std::size_t i = 0; i < collidables.size(); i++)
    {
		for (std::size_t j =  0; j < collidables.size(); j++)
        {
            if(i == j)
            {
                continue;
            }

            GameEntity e1 = collidables[i];
            GameEntity e2 = collidables[j];
            CollisionComponent* pColliderOne = pManager->GetComponent<CollisionComponent>(e1);
            CollisionComponent* pColliderTwo = pManager->GetComponent<CollisionComponent>(e2);

            if (pColliderOne->collider.CollidesWith(pColliderTwo->collider))
            {
                pColliderOne->collider.IsColliding(true);
                pColliderTwo->collider.IsColliding(true);

                if ((e1.GetTag() == "Projectile" && e2.GetTag() == "Asteroid") || (e1.GetTag() == "Asteroid" && e2.GetTag() == "Projectile"))
                {
                    toDestroy.push_back(e1);
                    toDestroy.push_back(e2);
                }
            }
		}
	}

    EventManager* pEvent = EventManager::Instance();
    for(auto& entity : toDestroy)
    {
		pManager->Destroy(entity);

        std::string tag = entity.GetTag();
        if(tag == "Asteroid")
        {
            pEvent->Fire("AsteroidDestroyed", nullptr);
        }
    }
}
