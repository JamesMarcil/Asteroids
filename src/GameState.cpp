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

// For the DirectX Math library
using namespace DirectX;

#pragma region Initialization

// --------------------------------------------------------
// Initializes the base class (including the window and D3D),
// sets up our geometry and loads the shaders (among other things)
// --------------------------------------------------------
void GameState::Enter(void)
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
        pEntity->AddSystemWithPriority<PhysicsSystem, 0>();
		pEntity->AddSystemWithPriority<InputControllerSystem, 1>();
        pEntity->AddSystemWithPriority<UIUpdateSystem, 2>();
        pEntity->AddSystemWithPriority<ClearSystem, 3>();
        pEntity->AddSystemWithPriority<RenderSystem, 4>();
        pEntity->AddSystemWithPriority<SkyboxSystem, 5>();
        pEntity->AddSystemWithPriority<UIRenderSystem, 6>();
		pEntity->AddSystemWithPriority<SwapSystem, 7>();
		pEntity->AddSystemWithPriority<ScriptSystem, 8>();

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

// --------------------------------------------------------
// Update your game here - take input, move objects, etc.
// --------------------------------------------------------
void GameState::Update(float deltaTime, float totalTime)
{
    /* Nothing to do. */
}

void GameState::Exit(void) 
{ 
    /* Nothing to do. */ 
}
