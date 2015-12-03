#include "EntityFactory.h"

// ECS
#include "EntityManager.h"
#include "ResourceManager.h"

// Components
#include "TransformComponent.h"
#include "RenderComponent.h"
#include "PhysicsComponent.h"
#include "InputComponent.h"
#include "LightComponent.h"
#include "ScriptComponent.h"
#include "AABBComponent.h"
#include "ButtonComponent.h"
#include "UIRenderComponent.h"
#include "UITextComponent.h"

// Scripts
#include "AutoDestructScript.h"

using namespace DirectX;

GameEntity EntityFactory::CreateTextField(std::wstring text, std::string spritefont_id, DirectX::XMFLOAT2 position, DirectX::XMFLOAT4 color)
{
    EntityManager* pEntity = EntityManager::Instance();
    ResourceManager* pResource = ResourceManager::Instance();

    GameEntity textfield = pEntity->Create();
    pEntity->AddComponent<UITextComponent>(textfield, text, spritefont_id, position, color);

    return textfield;
}

GameEntity EntityFactory::CreateButton(DirectX::XMFLOAT2 center, float width, float height, std::string material_id)
{
    EntityManager* pEntity = EntityManager::Instance();
    ResourceManager* pResource = ResourceManager::Instance();

    GameEntity button = pEntity->Create();
    pEntity->AddComponent<ButtonComponent>(button, true, "PlayClicked");
    pEntity->AddComponent<AABBComponent>(button, XMFLOAT2{center.x + 400.0f, center.y + 300.0f}, width / 2.0f, height / 2.0f); // TODO: Replace with half window width and half window height
    pEntity->AddComponent<UIRenderComponent>(button, center, width, height, pResource->GetMaterial(material_id));

    return button;
}

GameEntity EntityFactory::CreateAsteroid(DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 velocity, DirectX::XMFLOAT3 acceleration)
{
    EntityManager* pEntity = EntityManager::Instance();
    ResourceManager* pResource = ResourceManager::Instance();

    GameEntity asteroid = pEntity->Create();
    pEntity->AddComponent<TransformComponent>(asteroid, position);
    pEntity->AddComponent<RenderComponent>(asteroid, pResource->GetMaterial("default"), pResource->GetMesh("Sphere"));
    pEntity->AddComponent<PhysicsComponent>(asteroid, velocity, acceleration);
    ScriptComponent* script = pEntity->AddComponent<ScriptComponent>(asteroid);
    script->AddScript<AutoDestructScript>(-5.0f);

    return asteroid;
}

GameEntity EntityFactory::CreatePlayer(DirectX::XMFLOAT3 position)
{
    EntityManager* pEntity = EntityManager::Instance();
    ResourceManager* pResource = ResourceManager::Instance();

    GameEntity player = pEntity->Create();
    pEntity->AddComponent<RenderComponent>(player, pResource->GetMaterial("ship"), pResource->GetMesh("Ship"));
    pEntity->AddComponent<InputComponent>(player, 50.0f);
    TransformComponent* pTrans = pEntity->AddComponent<TransformComponent>(player, position);
    pTrans->transform.SetScale(.001f);
    PhysicsComponent* pPhysics = pEntity->AddComponent<PhysicsComponent>(player, XMVectorZero(), XMVectorZero());
    pPhysics->drag = 0.95f;
    pPhysics->rotationalDrag = 0.85f;

    return player;
}

GameEntity EntityFactory::CreateDirectionalLight(DirectX::XMFLOAT4 color, DirectX::XMFLOAT3 direction, float specularExponent)
{
    EntityManager* pEntity = EntityManager::Instance();

    GameEntity dirLight = pEntity->Create();
    pEntity->AddComponent<DirectionalLightComponent>
    (
        dirLight,
        color,
        direction,
        specularExponent
    );

    return dirLight;
}

GameEntity EntityFactory::CreatePointLight(DirectX::XMFLOAT4 color, DirectX::XMFLOAT3 position, float specularExponent)
{
    EntityManager* pEntity = EntityManager::Instance();

    GameEntity pointLight = pEntity->Create();
    pEntity->AddComponent<PointLightComponent>
    (
        pointLight,
        color,
        position,
        specularExponent
    );

    return pointLight;
}

GameEntity EntityFactory::CreateSpotlight(DirectX::XMFLOAT4 color, DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 direction, float specularExponent, float spotlightPower)
{
    EntityManager* pEntity = EntityManager::Instance();

    GameEntity spotlight = pEntity->Create();
    pEntity->AddComponent<SpotLightComponent>
    (
        spotlight,
        color,
        position,
        direction,
        specularExponent,
        spotlightPower
    );

    return spotlight;
}
