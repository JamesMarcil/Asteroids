#ifndef MENU_STATE_H
#define MENU_STATE_H

// IGameState interface
#include "IGameState.h"

// DirectX
#include <d3d11.h>

class MenuState : public IGameState
{
public:
    MenuState( ID3D11Device* const pDevice, ID3D11DeviceContext* const pDeviceContext, IDXGISwapChain* pSwapChain, ID3D11RenderTargetView* const pRenderTargetView, ID3D11DepthStencilView* const pDepthStencilView )
        : m_pDevice( pDevice ), m_pDeviceContext( pDeviceContext ), m_pSwapChain( pSwapChain ), m_pRenderTargetView( pRenderTargetView ), m_pDepthStencilView( pDepthStencilView )
    {}

private:
    ID3D11Device*           const m_pDevice;
    ID3D11DeviceContext*    const m_pDeviceContext;
    IDXGISwapChain*         const m_pSwapChain;
    ID3D11RenderTargetView* const m_pRenderTargetView;
    ID3D11DepthStencilView* const m_pDepthStencilView;

    virtual void Enter( void ) override;
    virtual void Update( float deltaTime, float totalTime ) override;
    virtual void Render( float deltaTime, float totalTime ) override;
    virtual void Exit( void ) override;
};

#endif
