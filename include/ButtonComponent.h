#ifndef BUTTON_COMPONENT_H
#define BUTTON_COMPONENT_H

// ECS
#include "Component.h"

// STD
#include <string>

struct ButtonComponent : public Component<ButtonComponent>
{
    bool isEnabled;
    std::string event;

    /*
     * TODO
     */
    ButtonComponent(bool isEnabled, std::string event)
        : isEnabled(isEnabled), event(event)
    {
        /* Nothing to do */
    }

    virtual ~ButtonComponent(void) override {}
};

#endif
