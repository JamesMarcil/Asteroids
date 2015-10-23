#include "RenderComponent.h"

#include "GameEntity.h" // Necessary to access GameEntity::* functions.

/*
 * Construct an instace of a RenderComponent.
 * @param   pParent     A pointer to the GameEntity that owns this RenderComponent.
 * @param   pMesh       A pointer to the Mesh for this Renderable object.
 * @param   pMaterial   A pointer to the Material for this Renderable object.
 */
RenderComponent::RenderComponent( GameEntity* pParent, Mesh* pMesh, Material* pMaterial)
    : IComponent( pParent ), m_renderer( pMesh, pMaterial )
{
    m_pTransform = pParent->GetComponent<TransformComponent>();
}

/*
 * Destroy this RenderComponent.
 */
RenderComponent::~RenderComponent(void)
{
    /* Nothing to do */
}

#pragma region IComponent Functions

void RenderComponent::OnEnable(void)
{
    /* Nothing to do */
}

void RenderComponent::OnDisable(void)
{
    /* Nothing to do */
}

void RenderComponent::Update(float dt, float tt)
{
    m_renderer.Draw( m_pTransform->GetTransform() );
}

#pragma endregion
