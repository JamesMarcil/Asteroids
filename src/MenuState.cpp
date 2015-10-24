#include "MenuState.h"

// DirectX
#include <d3d11.h>
#include "DXMacros.h"

// Managers
#include "InputManager.h"

// State
#include "StateMachine.h"
#include "GameStates.h"

void MenuState::Enter( void )
{
	/* Nothing to do. */
}

void MenuState::Update( float deltaTime, float totalTime )
{
    // Check input
    InputManager* pInput = InputManager::Instance();
    StateMachine<GameStates>* pState = StateMachine<GameStates>::Instance();
    if( pInput->IsKeyDown( '2' ) )
    {
        pState->GoToState( GameStates::GAME );
    }
    else if( pInput->IsKeyDown( '3' ) )
    {
        pState->GoToState( GameStates::EXIT );
    }
}

void MenuState::Render( float deltaTime, float totalTime, ID3D11RenderTargetView* const pRenderTargetView, ID3D11DepthStencilView* const pDepthStencilView )
{
    // Render the screen black
	const float black[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	m_pDeviceContext->ClearRenderTargetView( pRenderTargetView, black );
	m_pDeviceContext->ClearDepthStencilView( pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0 );
    HR( m_pSwapChain->Present( 0, 0 ) ); 
}

void MenuState::Exit( void )
{
	/* Nothing to do. */
}
