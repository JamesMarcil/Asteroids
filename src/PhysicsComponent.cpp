#include "PhysicsComponent.h"

#include "GameEntity.h" // Necessary to access GameEntity::* functions.

using namespace DirectX;

/*
 * Construct an instance of a PhysicsComponent.
 * @param   pParent     A pointer to the GameEntity that owns this PhysicsComponent.
 */
PhysicsComponent::PhysicsComponent(GameEntity* pParent)
    : IComponent( pParent )
{
    m_velocity = XMFLOAT3(0.0f, 0.0f, 0.0f);
    m_acceleration = XMFLOAT3(0.0f, 0.0f, 0.0f);
    m_pTransform = pParent->GetComponent<TransformComponent>();
}

/*
 * Destroy this PhysicsComponent.
 */
PhysicsComponent::~PhysicsComponent(void)
{
    /* Nothing to do */
}

#pragma region IComponent Functions

void PhysicsComponent::OnEnable(void)
{
    /* Nothing to do */
}

void PhysicsComponent::OnDisable(void)
{
    /* Nothing to do */
}

void PhysicsComponent::Update(float dt, float tt)
{
    XMVECTOR velocity = XMLoadFloat3(&m_velocity);
    XMVECTOR acceleration = XMLoadFloat3(&m_acceleration);

    // Update acceleration, velocity, position
    velocity = XMVectorAdd(velocity, XMVectorScale(acceleration, dt));
    m_pTransform->GetTransform().Translate(XMVectorScale(velocity, dt));

    XMStoreFloat3(&m_velocity, velocity);
    XMStoreFloat3(&m_acceleration, acceleration);
}

#pragma endregion
