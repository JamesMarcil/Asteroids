#include "ScriptSystem.h"

#include "EntityManager.h" // Need to to include this to access EntityManager::* functions.

// Components
#include "TransformComponent.h"
#include "ScriptComponent.h"

// DirectX
#include <DirectXMath.h>

void ScriptSystem::Update(EntityManager* pManager, float dt, float tt )
{
    using namespace DirectX;

    for(auto& entity : pManager->EntitiesWithComponents<ScriptComponent>())
    {
        ScriptComponent* script = pManager->GetComponent<ScriptComponent>(entity);
		script->Update(entity, dt, tt);
    }
}
