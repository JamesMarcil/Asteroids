#include "TransformComponent.h"

/*
 * Construct an instance of a TransformComponent.
 * @param   pParent         A pointer to the GameEntity that owns this IComponent.
 */
TransformComponent::TransformComponent(GameEntity* pParent)
    : IComponent( pParent )
{
    /* Nothing to do */
}

/*
 * Destroy this TransformComponent.
 */
TransformComponent::~TransformComponent()
{
    /* Nothing to do */
}

#pragma region IComponent Functions

void TransformComponent::OnEnable(void)
{
    /* Nothing to do */
}

void TransformComponent::OnDisable(void)
{
    /* Nothing to do */
}

void TransformComponent::Update(float dt, float tt)
{
    /* Nothing to do */
}

#pragma endregion
