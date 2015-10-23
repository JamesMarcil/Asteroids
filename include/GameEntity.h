#ifndef GAME_ENTITY_H
#define GAME_ENTITY_H

// STD
#include <array>
#include <bitset>
#include <utility>
#include <type_traits>

// DirectX
#include <DirectXMath.h>

// Components
#include "IComponent.h"

class GameEntity
{
private:
    std::bitset<IComponent::MAX_COMPONENTS> m_componentMask;            // Mask used to indicate if this GameEntity has a particular IComponent attached.
    std::array<IComponent*, IComponent::MAX_COMPONENTS> m_components;   // Array of all components

public:
    /*
     * Construct a GameEntity instance.
     */
	GameEntity(void);

    /*
     * Destroy the GameEntity and any attached IComponents
     */
	~GameEntity(void);

    /*
     * Attaches an IComponent of class T to the GameEntity.
     * @param   args        The arguments to pass to T's constructor.
     * @return A pointer to the newly constructed IComponent.
     */
    template <typename T, typename... Args>
    T* AddComponent(Args&&... args)
    {
       static_assert(std::is_base_of<IComponent, T>::value, "Must be an instance of IComponent!");

       // Test if we already have an IComponent of class T attached.
       const std::size_t id = IComponent::GetComponentID<T>();
       if(m_componentMask.test(id))
       {
           return static_cast<T*>(m_components[id]);
       }

       // Create the new IComponent, and set the mask
       T* component = new T(this, std::forward<Args>(args)...);
       m_components[id] = component;
       m_componentMask.set(id);

       return component;
    }

    /*
     * Removes an IComponent of class T if one is attacehd to this GameEntity.
     * @return  A boolean indicating if the IComponent was successfully removed.
     */
    template <typename T>
    bool RemoveComponent(void)
    {
        static_assert(std::is_base_of<IComponent,T>::value, "Must be an instance of IComponent!");

        // Test if we have an IComponent of class T attached
        const std::size_t id = IComponent::GetComponentID<T>();
        if(m_componentMask.test(id))
        {
            // Delete the component
            T* pComponent = m_components[id];
            delete pComponent;

            // Null the location in the array and reset the mask
            m_components[id] = nullptr;
            m_componentMask.reset(id);

            return true;
        }

        return false;
    }

    /*
     * Returns a pointer to an IComponent of class T if one is attached to this GameEntity.
     * @return A pointer to the IComponent if it exists, or nullptr if not.
     */
    template <typename T>
    T* GetComponent(void)
    {
        static_assert(std::is_base_of<IComponent,T>::value, "Must be an instance of IComponent!");

        // Test if we have an IComponent of class T attached.
        const std::size_t id = IComponent::GetComponentID<T>();
        if(m_componentMask.test(id))
        {
            return static_cast<T*>(m_components[id]);
        }

        return nullptr;
    }

    /*
     * Test if we have IComponent of class T attached to this GameEntity.
     * @return A boolean indicating if we have an IComponent of class T.
     */
    template <typename T>
    bool HasComponent(void)
    {
        static_assert(std::is_base_of<IComponent,T>::value, "Must be an instance of IComponent!");

        const std::size_t id = IComponent::GetComponentID<T>();

        return m_componentMask.test(id);
    }

    /*
     * Update all IComponents attached to this GameEntity.
     * @param   dt      The deltaTime value this frame.
     * @param   tt      The totalTime value this frame.
     */
    void Update(float dt, float tt);
};

#endif
