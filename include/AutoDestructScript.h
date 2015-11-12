#pragma once
#include "ScriptComponent.h"
#include "GameEntity.h"
#include "EntityManager.h"
#include "Transform.h"
#include "TransformComponent.h"
#include <iostream>
class AutoDestructScript : public Script
{
private :
	float destroyAtZ;
  
public:
	AutoDestructScript(float destroyAtZ):Script() { this->destroyAtZ = destroyAtZ; }
	~AutoDestructScript() {}

	void update(GameEntity entity, float dt, float tt) {
		EntityManager* pEntity = EntityManager::Instance();

		TransformComponent* pTransform = pEntity->GetComponent<TransformComponent>(entity);
		Transform& t = pTransform->transform;
		if(t.GetTranslation().z <= destroyAtZ)
			pEntity->Destroy(entity);
	}
};

