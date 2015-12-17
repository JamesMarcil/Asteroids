#ifndef MENU_STATE_H
#define MENU_STATE_H

// Interfaces
#include "GameStates.h"
#include "IGameState.h"
#include "IEventListener.h"

#include "GameEntity.h"

// STD
#include <string>

// Forward Declarations
class ResourceManager;
class EntityManager;
class EventManager;
class CameraManager;
template <typename T> class StateMachine;

class MenuState : public IGameState, public IEventListener
{
public:
    MenuState(void);
	virtual ~MenuState(void) override;

private:
    EventManager* m_pEvent;
    CameraManager* m_pCamera;
    EntityManager* m_pEntity;
    ResourceManager* m_pResource;
    StateMachine<GameStates>* m_pState;

    GameEntity title, button;

    void SpawnAsteroids(void);
    virtual void EventRouter(const std::string& event, void* pData) override;
    virtual void Enter(void) override;
    virtual void Update(float deltaTime, float totalTime) override;
    virtual void Exit(void) override;
};

#endif
