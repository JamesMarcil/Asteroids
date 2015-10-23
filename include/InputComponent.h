#ifndef INPUT_COMPONENT_H
#define INPUT_COMPONENT_H

// Components
#include "IComponent.h"
#include "TransformComponent.h"

// Managers
#include "InputManager.h"

class InputComponent : public IComponent
{
private:
    float m_movementSpeed;
    TransformComponent* m_pTransform;
    InputManager* m_pInput;

public:
    /*
     * Construct an instance of a InputComponent.
     * @param   pParent         A pointer to the GameEntity that owns this IComponent.
     * @param   speed           The speed this InputComponent will use.
     */
    InputComponent(GameEntity* pParent, float speed);

    /*
     * Destroy this InputComponent.
     */
    virtual ~InputComponent();

    /* IComponent interface functions */
    virtual void OnEnable(void) override;
    virtual void OnDisable(void) override;
    virtual void Update(float dt, float tt) override;
};

#endif
