#include "GameState.h"

// STD
#include <ctime>
#include <cstdlib>

// Managers
#include "EntityFactory.h"
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

// Systems
#include "PhysicsSystem.h"
#include "ScriptSystem.h"
#include "InputControllerSystem.h"
#include "ClearSystem.h"
#include "RenderSystem.h"
#include "SkyboxSystem.h"
#include "SwapSystem.h"
#include "UIRenderSystem.h"
#include "UIUpdateSystem.h"

#include "UITextComponent.h"

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

		this->LoadCurrentLevel();
		EventManager* pEventManager = EventManager::Instance();
		pEventManager->Register("WarpEnd", this);
		pEventManager->Register("AsteroidDestroyed", this);

        // Register Systems.
        EntityManager* pEntity = EntityManager::Instance();
        pEntity->AddSystem<PhysicsSystem>();
		pEntity->AddSystem<InputControllerSystem>();
        pEntity->AddSystem<UIUpdateSystem>();
        pEntity->AddSystem<ClearSystem>();
        pEntity->AddSystem<RenderSystem>();
        pEntity->AddSystem<SkyboxSystem>();
        pEntity->AddSystem<UIRenderSystem>();
		pEntity->AddSystem<SwapSystem>();
		pEntity->AddSystem<ScriptSystem>();

        // Make a SpotLight
        GameEntity spotlight = EntityFactory::CreateSpotlight
        (
            XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), // Color
            XMFLOAT3(0.0f, 0.0f, -1.0f),      // Position
            XMFLOAT3(0.0f, 0.0f, 1.0f),       // Direction
            30.0f,                            // Specular Exponent
            1.0f                              // SpotLight Power
        );

		//Make Player
		GameEntity player = EntityFactory::CreatePlayer(XMFLOAT3(0.0f, 0.0f, 0.0f));

        // Make Button.
        GameEntity button = EntityFactory::CreateButton(XMFLOAT2(0.0f, 0.0f), 100.0f, 100.0f, "UIMaterial");

        // Make Text.
        GameEntity text = pEntity->Create();
        pEntity->AddComponent<UITextComponent>(text, L"Test", "TimesNewRoman", XMFLOAT2{400.0f, 300.0f}, XMFLOAT4{0.0f, 0.0f, 0.0f, 1.0f});

        isInitialized = true;
    }
}

#pragma endregion

void GameState::LoadCurrentLevel()
{
	this->currentLevel++;
	srand(time(0));
	int span = 2;
	int toAdd = 30 + currentLevel * 5;
	this->asteroids = toAdd;
	for (int i = 0; i < toAdd; ++i)
	{
		XMFLOAT3 position = XMFLOAT3(rand() % (span * 2) - span, rand() % (span * 2) - span, i * 5 + 15);
        XMFLOAT3 velocity{0.0f, 0.0f, 0.0f};
        XMFLOAT3 acceleration{0.0f, 0.0f, -1.0f + currentLevel * -2.0f};

        EntityFactory::CreateAsteroid(position, velocity, acceleration);
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
