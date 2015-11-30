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
#include "PostEffectsSystem.h"

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

#pragma region post process set up

		ID3D11RenderTargetView* ppRTV;
		ID3D11ShaderResourceView* ppSRV;

		// Create post-process resources
		D3D11_TEXTURE2D_DESC textureDesc;
		ZeroMemory(&textureDesc, sizeof(textureDesc));
		textureDesc.Width = pManager->GetWindowWidth();
		textureDesc.Height = pManager->GetWindowHeight();
		textureDesc.ArraySize = 1;
		textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		textureDesc.CPUAccessFlags = 0;
		textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		textureDesc.MipLevels = 1;
		textureDesc.MiscFlags = 0;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.SampleDesc.Quality = 0;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;

		// create the texture that we will render into
		ID3D11Texture2D* ppTexture;
		pManager->GetDevice()->CreateTexture2D(&textureDesc, 0, &ppTexture);
		
		// Create the Render Target View
		D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
		ZeroMemory(&rtvDesc, sizeof(rtvDesc));
		rtvDesc.Format = textureDesc.Format;
		rtvDesc.Texture2D.MipSlice = 0;
		rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

		pManager->GetDevice()->CreateRenderTargetView(ppTexture, &rtvDesc, &ppRTV);

		// Create the Shader Resource View for the scene
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		ZeroMemory(&srvDesc, sizeof(srvDesc));
		srvDesc.Format = textureDesc.Format;
		srvDesc.Texture2D.MipLevels = 1;
		srvDesc.Texture2D.MostDetailedMip = 0;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;

		pManager->GetDevice()->CreateShaderResourceView(ppTexture, &srvDesc, &ppSRV);

		// register the scene texture as our postEffectTexture
		pManager->RegisterTexture("PostEffectTexture", ppSRV);

		// We can release the texture because the RenderTarget and Shader Resource View now handle it
		ppTexture->Release();

		// Register the post processing Render Target
		pManager->RegisterRenderTargetView("PostRTV", ppRTV);

#pragma endregion

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
		pEntity->AddSystem<PostEffectsSystem>();
		pEntity->AddSystem<SwapSystem>();
		pEntity->AddSystem<ScriptSystem>();

        // Make a SpotLight
        {
            GameEntity e = pEntity->Create();
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
		GameEntity player = pEntity->Create();
		pEntity->AddComponent<RenderComponent>(player, pManager->GetMaterial("ship"), pManager->GetMesh("Ship"));
        pEntity->AddComponent<InputComponent>(player, 50.0f);
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
		GameEntity e = pEntity->Create();
		XMFLOAT3 position = XMFLOAT3(rand() % (span * 2) - span, rand() % (span * 2) - span, i * 5 + 15);
		pEntity->AddComponent<TransformComponent>(e, position);
		pEntity->AddComponent<RenderComponent>(e, defaultMat, pManager->GetMesh("Sphere"));
		pEntity->AddComponent<PhysicsComponent>(e, XMVectorZero(), XMVectorSet(0.0f, 0.0f, -1.0f + currentLevel*-2.0f, 0.0f));
		ScriptComponent* script = pEntity->AddComponent<ScriptComponent>(e);
		script->AddScript<AutoDestructScript>(-5.0f);
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
