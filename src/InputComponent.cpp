#include "InputComponent.h"

#include "GameEntity.h" // Necessary for GameEntity::* functions.

/*
 * Construct an instance of a InputComponent.
 * @param   pParent         A pointer to the GameEntity that owns this IComponent.
 * @param   speed           The speed this InputComponent will use.
 */
InputComponent::InputComponent(GameEntity* pParent, float speed)
    : IComponent( pParent ), m_movementSpeed(speed)
{
    m_pTransform = pParent->GetComponent<TransformComponent>();
    m_pInput = InputManager::instance();
}

/*
 * Destroy this InputComponent.
 */
InputComponent::~InputComponent()
{
    /* Nothing to do */
}

#pragma region IComponent Functions
void InputComponent::OnEnable(void)
{
    /* Nothing to do */
}

void InputComponent::OnDisable(void)
{
    /* Nothing to do */
}

void InputComponent::Update(float dt, float tt)
{
    float speed = m_movementSpeed * dt;
    Transform& t = m_pTransform->GetTransform();

    if( m_pInput->IsKeyDown(VK_UP))
    {
        t.Translate(0.0f, 0.0f, speed);
    }
    else if( m_pInput->IsKeyDown(VK_DOWN))
    {
        t.Translate(0.0f, 0.0f, -speed);
    }

    if( m_pInput->IsKeyDown(VK_LEFT))
    {
        t.Translate(-speed, 0.0f, 0.0f);
    }
    else if( m_pInput->IsKeyDown(VK_RIGHT))
    {
        t.Translate(speed, 0.0f, 0.0f);
    }
}

#pragma endregion
