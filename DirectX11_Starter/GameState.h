#ifndef GAME_STATE_H
#define GAME_STATE_H

// DirectX
#include <d3d11.h>

// State
#include "IGameState.h"

// Game
#include "GameEntity.h"

// STD
#include <vector>

class GameState : public IGameState
{
private:
    ID3D11Device*           const m_pDevice;
    ID3D11DeviceContext*    const m_pDeviceContext;
    IDXGISwapChain*         const m_pSwapChain;
    ID3D11RenderTargetView* const m_pRenderTargetView;
    ID3D11DepthStencilView* const m_pDepthStencilView;

    bool     isInitialized;
    bool	 prevSpaceBar;
    unsigned currentEntity;
	std::vector<GameEntity> entities;

    virtual void Enter( void ) override;
    virtual void Update( float deltaTime, float totalTime ) override;
    virtual void Render( float deltaTime, float totalTime ) override;
    virtual void Exit( void ) override;

public:
    GameState( ID3D11Device* const pDevice, ID3D11DeviceContext* const pDeviceContext, IDXGISwapChain* const pSwapChain, ID3D11RenderTargetView* const pRenderTargetView, ID3D11DepthStencilView* const pDepthStencilView  )
        : m_pDevice( pDevice ), m_pDeviceContext( pDeviceContext ), m_pSwapChain( pSwapChain ), m_pRenderTargetView( pRenderTargetView ), m_pDepthStencilView( pDepthStencilView )
    {
    }

	virtual ~GameState() {}
};

#endif
