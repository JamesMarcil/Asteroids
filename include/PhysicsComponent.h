#ifndef PHYSICS_COMPONENT_H
#define PHYSICS_COMPONENT_H

#include "Component.h"

// DirectX
#include <DirectXMath.h>

struct PhysicsComponent : public Component<PhysicsComponent>
{
    DirectX::XMFLOAT3 velocity;
    DirectX::XMFLOAT3 acceleration;

    PhysicsComponent(float vx, float vy, float vz, float ax, float ay, float az)
    {
        velocity = DirectX::XMFLOAT3( vx, vy, vz );
        acceleration = DirectX::XMFLOAT3( ax, ay, az );
    }

    PhysicsComponent(DirectX::XMVECTOR& v, DirectX::XMVECTOR& a)
    {
        DirectX::XMStoreFloat3(&velocity, v);
        DirectX::XMStoreFloat3(&acceleration, a);
    }

    PhysicsComponent(DirectX::XMFLOAT3 v, DirectX::XMFLOAT3 a)
    {
        velocity = v;
        acceleration = a;
    }

    ~PhysicsComponent(void){}
};

#endif
