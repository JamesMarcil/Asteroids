#ifndef AUTO_DESTRUCT_SCRIPT_H
#define AUTO_DESTRUCT_SCRIPT_H

#include "EntityManager.h"
#include "ScriptComponent.h"
#include "Transform.h"
#include "TransformComponent.h"
#include "GameEntity.h"

class AutoDestructScript : public IScript
{
private :
	float destroyAtZ;

public:
	AutoDestructScript(float destroyAtZ)
        : destroyAtZ(destroyAtZ)
    {
        /* Nothing to do. */
    }

	virtual ~AutoDestructScript(void) {}

	virtual void Update(GameEntity entity, float dt, float tt) override
    {
		EntityManager* pEntity = EntityManager::Instance();

		TransformComponent* pTransform = pEntity->GetComponent<TransformComponent>(entity);
		Transform& t = pTransform->transform;

		if (t.GetTranslation().z <= destroyAtZ)
        {
			pEntity->Destroy(entity);
			EventManager::Instance()->Fire("AsteroidDestroyed", nullptr);
		}
	}
};

#endif
