#ifndef EXIT_STATE_H
#define EXIT_STATE_H

// State
#include "IGameState.h"

// DirectX
#include <d3d11.h>

class ExitState : public IGameState
{
private:
    ID3D11Device*           const m_pDevice;
    ID3D11DeviceContext*    const m_pDeviceContext;
    IDXGISwapChain*         const m_pSwapChain;

    virtual void Enter( void ) override;
    virtual void Update( float deltaTime, float totalTime ) override;
    virtual void Render( float deltaTime, float totalTime, ID3D11RenderTargetView* const pRenderTargetView, ID3D11DepthStencilView* const pDepthStencilView ) override;
    virtual void Exit( void ) override;
    
public:
    ExitState( ID3D11Device* const pDevice, ID3D11DeviceContext* const pDeviceContext, IDXGISwapChain* pSwapChain )
        : m_pDevice( pDevice ), m_pDeviceContext( pDeviceContext ), m_pSwapChain( pSwapChain )
    {}

	virtual ~ExitState() {}
};

#endif
