#include "GameState.h"

// STD
#include <ctime>
#include <cstdlib>

// Managers
#include "EntityManager.h"
#include "InputManager.h"
#include "ResourceManager.h"
#include "CameraManager.h"
// State
#include "StateMachine.h"
#include "GameStates.h"

// Components
#include "TransformComponent.h"
#include "RenderComponent.h"
#include "PhysicsComponent.h"
#include "InputComponent.h"

// Systems
#include "PhysicsSystem.h"
#include "RenderSystem.h"
#include "InputControllerSystem.h"

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
    if( !isInitialized )
    {
		CameraManager* camMan = CameraManager::Instance();
		camMan->RegisterCamera<Camera>("Main Camera", 0.0f, 0.0f, -5.0f);
		camMan->SetActiveCamera("Main Camera");

        ResourceManager* pManager = ResourceManager::Instance();

        /* Mesh Creation */
        pManager->RegisterMesh( "Sphere", "models/sphere.obj" );
        pManager->RegisterMesh( "Helix", "models/helix.obj" );
        pManager->RegisterMesh( "Cube", "models/cube.obj" );

        /* Shader Creation */
        pManager->RegisterShader<SimpleVertexShader>( "StandardVertex", L"VertexShader.cso" );
        pManager->RegisterShader<SimplePixelShader>( "StandardPixel", L"PixelShader.cso" );

		/* Texture Creation */
        pManager->RegisterTexture("Diffuse", L"textures/crate.png" );
        pManager->RegisterTexture("Rust", L"textures/rusty.jpg" );
        pManager->RegisterTexture("Rust_Spec", L"textures/rustySpec.png" );

		/* Material Creation */
		Material* defaultMat = new Material
        (
            static_cast<SimpleVertexShader*>(pManager->GetShader("StandardVertex")),
            static_cast<SimplePixelShader*>(pManager->GetShader("StandardPixel"))
        );
		defaultMat->AddTexture("diffuseTexture", "Diffuse");
		defaultMat->AddTexture("rustTexture", "Rust");
		defaultMat->AddTexture("specMapTexture", "Rust_Spec");
		pManager->RegisterMaterial("default", defaultMat);

        /* Sampler Creation */
        D3D11_SAMPLER_DESC samplerDesc;
        ZeroMemory(&samplerDesc, sizeof(samplerDesc));
        samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
        samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
        samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
        samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
        samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
        pManager->RegisterSamplerState( "trilinear", samplerDesc );

        EntityManager* pEntity = EntityManager::Instance();

        // Register Systems for demonstration.
        pEntity->AddSystem<PhysicsSystem>();
        pEntity->AddSystemWithPriority<RenderSystem, 0>();
		pEntity->AddSystemWithPriority<InputControllerSystem, 1>();

        // Generate 50 GameEntities for demonstration.
        srand(time(0));
		for (int i = 0; i < 15; ++i)
		{
			GameEntity e = pEntity->Create();
			XMFLOAT3 position = XMFLOAT3(rand() % 25 + 1, rand() % 25 + 1, rand() % 25 + 1);
			pEntity->AddComponent<TransformComponent>(e, position);
            pEntity->AddComponent<RenderComponent>(e, defaultMat, pManager->GetMesh("Sphere"));
            pEntity->AddComponent<PhysicsComponent>(e, XMVectorZero(), XMVectorSet(0.0f, -0.1f, 0.0f, 0.0f));
		}

		//Make Player
		GameEntity player = pEntity->Create();
		pEntity->AddComponent<TransformComponent>(player, XMFLOAT3(0, 0, 0));
		pEntity->AddComponent<RenderComponent>(player, defaultMat, pManager->GetMesh("Cube"));
		pEntity->AddComponent<PhysicsComponent>(player, XMVectorZero(), XMVectorSet(0, 0, 0, 0));
		pEntity->AddComponent<InputComponent>(player, 50.0f);
		PhysicsComponent* pPhysics = pEntity->GetComponent<PhysicsComponent>(player);
		pPhysics->drag = 0.95f;
		pPhysics->rotationalDrag = 0.85f;

        isInitialized = true;
    }

	currentEntity = 1;
}

#pragma endregion

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
    else if( pInput->IsKeyDown( '3' ) )
    {
        pState->GoToState( GameStates::EXIT );
    }
}

void GameState::Exit( void ) { /* Nothing to do. */ }
