#include "GameState.h"

// STD
#include <ctime>
#include <cstdlib>

// Managers
#include "EntityManager.h"
#include "InputManager.h"
#include "ResourceManager.h"
#include "CameraManager.h"

// Events
#include "EventManager.h"
#include "IEventListener.h"

// State
#include "StateMachine.h"
#include "GameStates.h"

// Components
#include "TransformComponent.h"
#include "RenderComponent.h"
#include "PhysicsComponent.h"
#include "InputComponent.h"
#include "LightComponent.h"
#include "ScriptComponent.h"
#include <CollisionComponent.h>
#include <AttackComponent.h>

// Scripts
#include "AutoDestructScript.h"

// Systems
#include "PhysicsSystem.h"
#include "ScriptSystem.h"
#include "InputControllerSystem.h"
#include "ClearSystem.h"
#include "RenderSystem.h"
#include "SkyboxSystem.h"
#include "SwapSystem.h"
#include <CollisionSystem.h>
#include <AttackSystem.h>

// For the DirectX Math library
using namespace DirectX;

#pragma region Initialization

// --------------------------------------------------------
// Initializes the base class (including the window and D3D),
// sets up our geometry and loads the shaders (among other things)
// --------------------------------------------------------
void GameState::Enter( void )
{
    // Register resources with the ResourceManager
    if(!isInitialized)
    {
		CameraManager* camMan = CameraManager::Instance();
		camMan->RegisterCamera<Camera>("Main Camera", 0.0f, 0.0f, -5.0f);
		camMan->SetActiveCamera("Main Camera");

        // Parse "resources.json" for any resources.
        ResourceManager* pManager = ResourceManager::Instance();
        pManager->ParseJSONFile("json/resources.json");

        /* Sampler Creation */
        {
            D3D11_SAMPLER_DESC desc;
            ZeroMemory(&desc, sizeof(desc));
            desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
            desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
            desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
            desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
            desc.MaxLOD = D3D11_FLOAT32_MAX;

            pManager->RegisterSamplerState( "trilinear", desc);
        }

        // Create the ID3D11RasterizerState for the Skybox.
        {
            D3D11_RASTERIZER_DESC desc;
            ZeroMemory(&desc, sizeof(D3D11_RASTERIZER_DESC));
            desc.FillMode = D3D11_FILL_SOLID;
            desc.CullMode = D3D11_CULL_FRONT;
            desc.DepthClipEnable = true;

            pManager->RegisterRasterizerState("Skybox_Rasterizer", desc);
        }

        // Create the ID3D11DepthStencilState for the Skybox.
        {
            D3D11_DEPTH_STENCIL_DESC desc;
            ZeroMemory(&desc, sizeof(D3D11_DEPTH_STENCIL_DESC));
            desc.DepthEnable = true;
            desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
            desc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

            pManager->RegisterDepthStencilState("Skybox_DepthStencil", desc);
        }

		this->LoadCurrentLevel();
		EventManager* pEventManager = EventManager::Instance();
		pEventManager->Register("WarpEnd", this);
		pEventManager->Register("AsteroidDestroyed", this);

        // Register Systems.
        EntityManager* pEntity = EntityManager::Instance();
        pEntity->AddSystem<PhysicsSystem>();
		pEntity->AddSystem<InputControllerSystem>();
        pEntity->AddSystem<ClearSystem>();
        pEntity->AddSystem<RenderSystem>();
        pEntity->AddSystem<SkyboxSystem>();
		pEntity->AddSystem<SwapSystem>();
		pEntity->AddSystem<ScriptSystem>();
		pEntity->AddSystem<CollisionSystem>();
		pEntity->AddSystem<AttackSystem>();

        // Make a SpotLight
        {
            GameEntity e = pEntity->Create("Light");
            pEntity->AddComponent<SpotLightComponent>
            (
                e,                                          // Entity
                XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),           // Color
                XMFLOAT3(0.0f, 0.0f, -1.0f),                // Position
                XMFLOAT3(0.0f, 0.0f, 1.0f),                 // Direction
                30.0f,                                      // Specular Exponent
                1.0f                                        // SpotLight Power
            );
        }

		//Make Player
		GameEntity player = pEntity->Create("Player");
		pEntity->AddComponent<RenderComponent>(player, pManager->GetMaterial("ship"), pManager->GetMesh("Ship"));
        pEntity->AddComponent<InputComponent>(player, 50.0f);
		pEntity->AddComponent<CollisionComponent>(player, *pManager->GetMesh("Ship"), XMFLOAT3(0, 0, 0), 0.001f);
		pEntity->AddComponent<AttackComponent>(player, 5);
		TransformComponent* pTrans = pEntity->AddComponent<TransformComponent>(player, XMFLOAT3(0, 0, 0));
		pTrans->transform.SetScale(.001f);
		PhysicsComponent* pPhysics = pEntity->AddComponent<PhysicsComponent>(player, XMVectorZero(), XMVectorSet(0, 0, 0, 0));
		pPhysics->drag = 0.95f;
		pPhysics->rotationalDrag = 0.85f;

        isInitialized = true;
    }

	currentEntity = 1;
}

#pragma endregion

void GameState::LoadCurrentLevel()
{
	this->currentLevel++;
	EntityManager* pEntity = EntityManager::Instance();
	ResourceManager* pManager = ResourceManager::Instance();

	Material* defaultMat = pManager->GetMaterial("default");

	srand(time(0));
	int span = 2;
	int toAdd = 30 + currentLevel * 5;
	this->asteroids = toAdd;
	for (int i = 0; i < toAdd; ++i)
	{
		GameEntity e = pEntity->Create("Asteroid");
		XMFLOAT3 position = XMFLOAT3(rand() % (span * 2) - span, rand() % (span * 2) - span, i * 5 + 15);
		pEntity->AddComponent<TransformComponent>(e, position);
		pEntity->AddComponent<RenderComponent>(e, defaultMat, pManager->GetMesh("Sphere"));
		pEntity->AddComponent<PhysicsComponent>(e, XMVectorZero(), XMVectorSet(0.0f, 0.0f, -1.0f + currentLevel*-2.0f, 0.0f));
		ScriptComponent* script = pEntity->AddComponent<ScriptComponent>(e);
		script->AddScript(new AutoDestructScript(-5.0f));
		pEntity->AddComponent<CollisionComponent>(e, *pManager->GetMesh("Sphere"), position, 1.3f);
	}
}

// One Listener can route multiple events, check the name to route properly
void GameState::EventRouter(const std::string& name, void* data)
{
    // Finished warp, initialize new level.
	if (name == "WarpEnd")
    {
		LoadCurrentLevel();
    }
    // Called whenever an asteroid is shot or auto destructs.
    else if (name == "AsteroidDestroyed")
    {
		asteroids--;
		if (asteroids <= 0) {
			EventManager::Instance()->Fire("WarpEnd", nullptr); //TODO change to WarpStart
		}
	}
}



#pragma region Game Loop
// --------------------------------------------------------
// Update your game here - take input, move objects, etc.
// --------------------------------------------------------
void GameState::Update( float deltaTime, float totalTime )
{
	// Handle Input
    InputManager* pInput = InputManager::Instance();
    StateMachine<GameStates>* pState = StateMachine<GameStates>::Instance();

    if( pInput->IsKeyDown( '1' ) )
    {
        pState->GoToState( GameStates::MENU );
    }
	else if (pInput->IsKeyDown('3'))
	{
		pState->GoToState(GameStates::EXIT);
	}
}

void GameState::Exit( void ) { /* Nothing to do. */ }
#pragma endregion
