#include "ExitState.h"

// DirectX
#include "DXMacros.h"

// Managers
#include "InputManager.h"

// State
#include "StateMachine.h"
#include "GameStates.h"

void ExitState::Enter( void )
{
	/* Nothing to do. */
}

void ExitState::Update( float deltaTime, float totalTime )
{
    // Check input
    InputManager* pInput = InputManager::instance();
    StateMachine<GameStates>* pState = StateMachine<GameStates>::instance();

    if( pInput->IsKeyDown( '1' ) )
    {
        pState->GoToState( GameStates::MENU );
    }
    else if( pInput->IsKeyDown( '2' ) )
    {
        pState->GoToState( GameStates::GAME );
    }
}

void ExitState::Render( float deltaTime, float totalTime )
{
    // Render the screen white
	const float white[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	m_pDeviceContext->ClearRenderTargetView( m_pRenderTargetView, white );
	m_pDeviceContext->ClearDepthStencilView( m_pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0 );
    HR( m_pSwapChain->Present( 0, 0 ) ); 
}

void ExitState::Exit( void )
{
	/* Nothing to do. */
}
