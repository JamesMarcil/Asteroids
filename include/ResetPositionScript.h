#ifndef RESET_POSITION_SCRIPT_H
#define RESET_POSITION_SCRIPT_H

// Script Interface
#include "IScript.h"

// ECS
#include "GameEntity.h"
#include "EntityManager.h"
#include "TransformComponent.h"

// DirectX
#include <DirectXMath.h>

class ResetPositionScript : public IScript
{
private:
    float resetAtZ;
    float valueToResetTo;

public:
    ResetPositionScript(float resetAtZ, float valueToResetTo)
        : resetAtZ(resetAtZ), valueToResetTo(valueToResetTo)
    {
        /* Nothing to do */
    }

    virtual ~ResetPositionScript(void) = default;

    virtual void Update(GameEntity entity, float dt, float tt) override
    {
        using namespace DirectX;

        EntityManager* pEntity = EntityManager::Instance();
        if(pEntity->HasComponent<TransformComponent>(entity))
        {
            TransformComponent* pTransform = pEntity->GetComponent<TransformComponent>(entity);
            Transform& t = pTransform->transform;
            XMFLOAT3 position = t.GetTranslation();

            if(position.z <= resetAtZ)
            {
                position.z = valueToResetTo;
                t.SetTranslation(position);
            }
        }
    }
};

#endif
