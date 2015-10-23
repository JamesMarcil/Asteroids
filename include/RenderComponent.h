#ifndef RENDER_COMPONENT_H
#define RENDER_COMPONENT_H

#include "IComponent.h"
#include "TransformComponent.h"
#include "Renderer.h"
#include "Material.h"
#include "Mesh.h"

class RenderComponent : public IComponent
{
private:
    Renderer m_renderer;
    TransformComponent* m_pTransform;       // Renderable components need a Transform.

public:
    /*
     * Construct an instace of a RenderComponent.
     * @param   pParent     A pointer to the GameEntity that owns this RenderComponent.
     * @param   pMesh       A pointer to the Mesh for this Renderable object.
     * @param   pMaterial   A pointer to the Material for this Renderable object.
     */
    RenderComponent( GameEntity* pParent, Mesh* pMesh, Material* pMaterial);

    /*
     * Destroy this RenderComponent.
     */
    virtual ~RenderComponent(void);

    /* Getters */
    Renderer& GetRenderer(void)
    {
        return m_renderer;
    }

    /* IComponent interface functions */
    virtual void OnEnable(void) override;
    virtual void OnDisable(void) override;
    virtual void Update(float dt, float tt) override;
};

#endif
