#ifndef TRANSFORM_COMPONENT_H
#define TRANSFORM_COMPONENT_H

#include "IComponent.h"
#include "Transform.h"

class TransformComponent : public IComponent
{
private:
    Transform m_transform;

public:
    /*
     * Construct an instance of a TransformComponent.
     * @param   pParent         A pointer to the GameEntity that owns this IComponent.
     */
    TransformComponent(GameEntity* pParent);

    /*
     * Destroy this TransformComponent.
     */
    virtual ~TransformComponent();

    /* Getters */
    Transform& GetTransform(void)
    {
        return m_transform;
    }

    /* IComponent interface functions */
    virtual void OnEnable(void) override;
    virtual void OnDisable(void) override;
    virtual void Update(float dt, float tt) override;
};

#endif
