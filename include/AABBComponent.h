#ifndef AABB_COMPONENT_H
#define AABB_COMPONENT_H

// ECS
#include "Component.h"

// DirectX
#include <DirectXMath.h>

#include "AABB.h"

struct AABBComponent : public Component<AABBComponent>
{
    AABB boundingBox;

    /*
     * TODO
     */
    AABBComponent(DirectX::XMFLOAT2 min, DirectX::XMFLOAT2 max)
        : boundingBox(min, max)
    {
        /* Nothing to do. */
    }

    /*
     * TODO
     */
    AABBComponent(DirectX::XMFLOAT2 center, float halfWidth, float halfHeight)
        : boundingBox(center, halfWidth, halfHeight)
    {
        /* Nothing to do. */
    }

    virtual ~AABBComponent(void) override {}
};

#endif
