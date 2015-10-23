#ifndef PHYSICS_COMPONENT_H
#define PHYSICS_COMPONENT_H

// DirectX
#include <DirectXMath.h>

#include "IComponent.h"
#include "TransformComponent.h"

class PhysicsComponent : public IComponent
{
private:
    DirectX::XMFLOAT3 m_velocity;
    DirectX::XMFLOAT3 m_acceleration;
    TransformComponent* m_pTransform;

public:
    /*
     * Construct an instance of a PhysicsComponent.
     * @param   pParent     A pointer to the GameEntity that owns this PhysicsComponent.
     */
    PhysicsComponent(GameEntity* pParent);

    /*
     * Destroy this PhysicsComponent.
     */
    virtual ~PhysicsComponent(void);

    /* Getters */
    DirectX::XMFLOAT3& GetVelocity(void)
    {
        return m_velocity;
    }

    DirectX::XMFLOAT3& GetAcceleration(void)
    {
        return m_acceleration;
    }

    /* IComponent inteface functions */
    virtual void OnEnable(void) override;
    virtual void OnDisable(void) override;
    virtual void Update(float dt, float tt) override;
};

#endif
