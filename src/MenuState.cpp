#include "MenuState.h"

// STD
#include <cmath>

// DirectX
#include <DirectXMath.h>

// Managers
#include "CameraManager.h"
#include "ResourceManager.h"
#include "EntityManager.h"
#include "EventManager.h"

// Systems
#include "ClearSystem.h"
#include "UIUpdateSystem.h"
#include "UIRenderSystem.h"
#include "SkyboxSystem.h"
#include "SwapSystem.h"
#include "PhysicsSystem.h"
#include "ScriptSystem.h"
#include "RenderSystem.h"

// State
#include "StateMachine.h"
#include "GameStates.h"

// Scripts
#include "AABBComponent.h"
#include "UITextComponent.h"
#include "ScriptComponent.h"
#include "ResetPositionScript.h"

#include "EntityFactory.h"

#include "StaticCamera.h"

using namespace DirectX;

#pragma region Destructor

MenuState::MenuState(void)
{
    m_pEvent = EventManager::Instance();
    m_pCamera = CameraManager::Instance();
    m_pEntity = EntityManager::Instance();
    m_pResource = ResourceManager::Instance();
    m_pState = StateMachine<GameStates>::Instance();
}

MenuState::~MenuState(void)
{
    /* Nothing to do. */
}

#pragma endregion

#pragma region IGameState Interface

void MenuState::Enter(void)
{
    m_pResource->ParseJSONFile("json/resources.json");

    m_pCamera->RegisterCamera<StaticCamera>("Menu_Camera", 0.0f, 0.0f, -5.0f);
    m_pCamera->SetActiveCamera("Menu_Camera");

    m_pEntity->AddSystem<UIUpdateSystem>();
    m_pEntity->AddSystem<PhysicsSystem>();
    m_pEntity->AddSystem<ScriptSystem>();
    m_pEntity->AddSystem<ClearSystem>();
    m_pEntity->AddSystem<RenderSystem>();
    m_pEntity->AddSystem<SkyboxSystem>();
    m_pEntity->AddSystem<UIRenderSystem>();
    m_pEntity->AddSystem<SwapSystem>();

    m_pEvent->Register("PlayClicked", this);
    m_pEvent->Register("WarpBegin", this);
    m_pEvent->Register("WarpEnd", this);
    m_pEvent->Register("OnResize", this);
    m_pEvent->Fire("WarpBegin", nullptr);

    float width = static_cast<float>(m_pResource->GetWindowWidth());
    float height = static_cast<float>(m_pResource->GetWindowHeight());

    title = EntityFactory::CreateTextField
        (
            L"Asteroids",
            "SpaceAge54",
            XMFLOAT2{0.15625f * width, 0.15625f * height},
            XMFLOAT4{1.0f, 1.0f, 1.0f, 1.0f}
        );

    button = EntityFactory::CreateButton
        (
            XMFLOAT2{0.0f, 0.0f},
            width,
            height,
            "PlayClicked",
            L"Click to Play",
            "SpaceAge48",
            XMFLOAT2{0.1f * width, 0.5f * height},
            XMFLOAT4{1.0f, 1.0f, 1.0f, 1.0f}
        );

    GameEntity light = EntityFactory::CreateDirectionalLight
        (
            XMFLOAT4{1.0f, 1.0f, 1.0f, 1.0f},   // Color
            XMFLOAT3{0.0f, 0.0f, 1.0f},         // Direction
            0.0f                                // Specular Exponent
        );

    SpawnAsteroids();
}

void MenuState::SpawnAsteroids(void)
{
    srand(static_cast<std::time_t>(0));
    int span = 10;
    for(int i = 0; i < 10; ++i)
    {
        XMFLOAT3 position = XMFLOAT3(static_cast<float>(rand()*rand() % (span * 2) - span), static_cast<float>(rand()*rand() % (span * 2) - span), i * 5.0f + 155.0f);
        XMFLOAT3 velocity{0.0f, 0.0f, -15.0f - (rand() % 15)};
        XMFLOAT3 acceleration{0.0f, 0.0f, 0.0f};
        XMFLOAT3 rotation{rand() * 3.0f, rand() * 3.0f, rand() * 3.0f};
        float scale = 1.0f + rand() * 0.0001f;

        GameEntity asteroid = EntityFactory::CreateAsteroid(position, velocity, acceleration, rotation, scale, i + 1);
        ScriptComponent* pScript = m_pEntity->AddComponent<ScriptComponent>(asteroid);
        pScript->AddScript<ResetPositionScript>(-5.0f, i * 5.0f + 155.0f);
    }
}

void MenuState::Update(float deltaTime, float totalTime)
{
    /* Nothing to do. */
}

void MenuState::Exit(void)
{
    m_pEntity->Clear();
    m_pEntity->ClearSystems();

    m_pEvent->UnRegister("WarpBegin", this);
    m_pEvent->UnRegister("WarpEnd", this);
    m_pEvent->UnRegister("PlayClicked", this);
    m_pEvent->UnRegister("OnResize", this);
}

#pragma endregion

void MenuState::EventRouter(const std::string& event, void* pData)
{
    if(event == "PlayClicked")
    {
        m_pState->GoToState(GameStates::GAME);
    }
    else if(event == "WarpEnd")
    {
        m_pEvent->Fire("WarpBegin", nullptr);
    }
    else if(event == "OnResize")
    {
        float width = static_cast<float>(m_pResource->GetWindowWidth());
        float height = static_cast<float>(m_pResource->GetWindowHeight());

        // Update the Title text.
        {
            UITextComponent* pText = m_pEntity->GetComponent<UITextComponent>(title);
            XMFLOAT2& position = pText->position;
            position.x = 0.15625f * width;
            position.y = 0.15625f * height;
        }

        // Update the Play button.
        {
            AABBComponent* pAABB = m_pEntity->GetComponent<AABBComponent>(button);
            pAABB->boundingBox = AABB{XMFLOAT2{0.5f * width, 0.5f * height}, 0.5f * width, 0.5f * height};

            UITextComponent* pText = m_pEntity->GetComponent<UITextComponent>(button);
            XMFLOAT2& position = pText->position;
            position.x = 0.1f * width;
            position.y = 0.5f * height;
        }
    }
}
