#include "GameEntity.h"

/*
 * Construct a GameEntity instance.
 */
GameEntity::GameEntity(void)
{
    /* Nothing to do. */
}

/*
 * Destroy the GameEntity and any attached IComponents
 */
GameEntity::~GameEntity(void)
{
    // Remove all attached components
    for( int i = 0; i < IComponent::MAX_COMPONENTS; ++i )
    {
        if(m_componentMask.test(i))
        {
            delete m_components[i];
        }
    }
}

/*
 * Update all IComponents attached to this GameEntity.
 * @param   dt      The deltaTime value this frame.
 * @param   tt      The totalTime value this frame.
 */
void GameEntity::Update(float dt, float tt)
{
    for( int i = 0; i < IComponent::MAX_COMPONENTS; ++i )
    {
        if(m_componentMask.test(i) && m_components[i]->GetIsEnabled())
        {
            m_components[ i ]->Update( dt, tt );
        }
    }
}
