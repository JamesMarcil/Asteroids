#ifndef COMPONENT_H
#define COMPONENT_H

// STD
#include <type_traits>

class IComponent
{
private:
    // Boolean indicating if this IComponent will be updated.
    bool m_isEnabled;

protected:
    // Owner of this IComponent.
    friend class GameEntity;
    GameEntity* m_pParent;

    // Current # of components instantiated.
    // Used to provide a unique ID # to each component.
    static std::size_t  m_sNumComponents;

public:
    // Constant indicating the max # of components that can be attached to a GameEntity.
    static constexpr std::size_t MAX_COMPONENTS = 50;

    /*
     * Construct an IComponent instance.
     * @param   pParent     A pointer to the GameEntity that owns this IComponent.
     */
    IComponent( GameEntity* pParent ) : m_pParent( pParent ), m_isEnabled( true ) 
    {
        /* Nothing to do. */
    }

    /*
     * Necessary so that the destructor is invoked for classes implementing IComponent.
     */
    virtual ~IComponent()
    {
        /* Nothing to do. */
    }

    /* Getters */
    bool GetIsEnabled(void) const
    {
        return m_isEnabled;
    }

    /*
     * Sets IsEnabled to true so that this component will be enabled.
     */
    void Enable(void)
    {
        if(!m_isEnabled)
        {
            m_isEnabled = true;
            OnEnable();
        }
    }

    /*
     * Sets IsEnabled to false so that this component will be disabled.
     */
    void Disable(void)
    {
        if(m_isEnabled)
        {
            m_isEnabled = false;
            OnDisable();
        }
    }

    /*
     * Gets the unique ID # of the component T.
     * @return  The ID # associated with components of class T.
     */
    template <typename T>
    static std::size_t GetComponentID(void)
    {
        static_assert( std::is_base_of<IComponent, T>::value, "Must be an instance of IComponent!" );
        static std::size_t id = m_sNumComponents++; // This will only be incremented the first time, then it will always have the same value
        return id;
    }

    /* IComponent Interface functions */
    virtual void OnEnable(void) = 0;
    virtual void OnDisable(void) = 0;
    virtual void Update(float dt, float tt) = 0;
};

#endif
