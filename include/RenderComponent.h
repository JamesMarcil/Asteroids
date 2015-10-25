#ifndef RENDER_COMPONENT_H
#define RENDER_COMPONENT_H

#include "Component.h"

#include "Renderer.h"

struct RenderComponent : public Component<RenderComponent>
{
    Renderer renderer;

    RenderComponent(Material* const pMaterial, Mesh* const pMesh)
        : renderer(pMesh, pMaterial)
    {
        /* Nothing to do */
    }

    ~RenderComponent(void){}
};

#endif
