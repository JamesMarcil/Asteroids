#ifndef UI_TEXT_COMPONENT_H
#define UI_TEXT_COMPONENT_H

// ECS
#include "Component.h"

// STD
#include <string>

// DirectX
#include <DirectXMath.h>

struct UITextComponent : public Component<UITextComponent>
{
public:
    std::wstring text;
    std::string spritefontID;
    DirectX::XMFLOAT2 position;
    DirectX::XMFLOAT4 color;
    
    /*
     * TODO
     */
    UITextComponent(std::wstring text, std::string spritefontID, DirectX::XMFLOAT2 position, DirectX::XMFLOAT4 color)
        : text(text), spritefontID(spritefontID), position(position), color(color)
    {
        /* Nothing to do. */
    }

    /*
     * TODO
     */
    virtual ~UITextComponent(void)
    {
        /* Nothing to do. */
    }
};

#endif
