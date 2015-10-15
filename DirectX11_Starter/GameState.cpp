#include "GameState.h"

// Managers
#include "InputManager.h"
#include "CameraManager.h"
#include "ResourceManager.h"

// State
#include "StateMachine.h"
#include "GameStates.h"

// DirectX
#include <d3d11.h>
#include <DirectXMath.h>
#include "DXMacros.h"
#include "Vertex.h"
#include "SimpleShader.h"

// For the DirectX Math library
using namespace DirectX;

#pragma region Initialization

// --------------------------------------------------------
// Initializes the base class (including the window and D3D),
// sets up our geometry and loads the shaders (among other things)
// --------------------------------------------------------
void GameState::Enter( void )
{
    // Register resources with the ResourceManager
    if( !isInitialized )
    {
        ResourceManager* pManager = ResourceManager::instance();
        pManager->RegisterDeviceAndContext( m_pDevice, m_pDeviceContext );

        /* Mesh Creation */
        pManager->RegisterMesh( "Sphere", "models/sphere.obj" );
        pManager->RegisterMesh( "Helix", "models/helix.obj" );
        pManager->RegisterMesh( "Cube", "models/cube.obj" );

        /* Shader Creation */
        pManager->RegisterShader<SimpleVertexShader>( "StandardVertex", L"VertexShader.cso" );
        pManager->RegisterShader<SimplePixelShader>( "StandardPixel", L"PixelShader.cso" );

        /* Texture Creation */
        pManager->RegisterTexture("Diffuse", L"textures/crate.png" );
        pManager->RegisterTexture("Rust", L"textures/rusty.jpg" );
        pManager->RegisterTexture("Rust_Spec", L"textures/rustySpec.png" );

        /* Sampler Creation */
        D3D11_SAMPLER_DESC samplerDesc;
        ZeroMemory(&samplerDesc, sizeof(samplerDesc));
        samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
        samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
        samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
        samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
        samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
        pManager->RegisterSamplerState( "trilinear", samplerDesc );

        /* GameEntity Creation */
        entities.emplace_back( pManager->GetMesh("Sphere") );
        entities.emplace_back( pManager->GetMesh("Helix") );
        entities.emplace_back( pManager->GetMesh( "Cube" ) );

        isInitialized = true;
    }

	currentEntity = 1;
}

#pragma endregion

#pragma region Game Loop

// --------------------------------------------------------
// Update your game here - take input, move objects, etc.
// --------------------------------------------------------
void GameState::Update( float deltaTime, float totalTime )
{
	// Handle Input
    InputManager* pInput = InputManager::instance();
    StateMachine<GameStates>* pState = StateMachine<GameStates>::instance();

    if( pInput->IsKeyDown( '1' ) )
    {
        pState->GoToState( GameStates::MENU );
    }
    else if( pInput->IsKeyDown( '3' ) )
    {
        pState->GoToState( GameStates::EXIT );
    }

    // Check for entity swap.
    bool currentSpacebar = pInput->IsKeyDown( VK_TAB );
    if ( currentSpacebar && !prevSpaceBar )
    {
        currentEntity = (currentEntity + 1) % entities.size();
    }
    prevSpaceBar = currentSpacebar;
}

void GameState::Render( float deltaTime, float totalTime, ID3D11RenderTargetView* const pRenderTargetView, ID3D11DepthStencilView* const pDepthStencilView )
{
	// Background color (Cornflower Blue in this case) for clearing
	const float color[4] = {0.4f, 0.6f, 0.75f, 0.0f};

	// Clear the render target and depth buffer (erases what's on the screen)
	m_pDeviceContext->ClearRenderTargetView( pRenderTargetView, color );
	m_pDeviceContext->ClearDepthStencilView( pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0 );

    GameEntity* ge = &entities[currentEntity];
    Mesh* mesh = ge->GetMesh();
    ID3D11Buffer* vb = mesh->GetVertexBuffer();
    ID3D11Buffer* ib = mesh->GetIndexBuffer();
    UINT stride = sizeof(Vertex);
    UINT offset = 0;

    // Update the Shaders
    {
        ResourceManager* pManager = ResourceManager::instance();
        ISimpleShader   *pVertexShader = pManager->GetShader( "StandardVertex" ),
                        *pPixelShader = pManager->GetShader( "StandardPixel" );
        Camera* pCamera = CameraManager::instance()->GetActiveCamera();
        XMFLOAT4 camPos = pCamera->transform.GetTranslation();

        // Update the Vertex Shader
        pVertexShader->SetMatrix4x4("world", ge->transform.GetTransform());
        pVertexShader->SetMatrix4x4("view", pCamera->GetViewMatrix());
        pVertexShader->SetMatrix4x4("projection", pCamera->GetProjectionMatrix());
        pVertexShader->SetShader(true);

        // Update the Pixel Shader
        pPixelShader->SetFloat3("DirLightDirection", XMFLOAT3(1, 1, 1));
        pPixelShader->SetFloat4("DirLightColor", XMFLOAT4(0.3f, 0.3f, 0.3f, 1));
        pPixelShader->SetFloat3("PointLightPosition", XMFLOAT3(3, 3, -3));
        pPixelShader->SetFloat4("PointLightColor", XMFLOAT4(1, 1, 1, 1));
        pPixelShader->SetFloat3("CameraPosition", XMFLOAT3(camPos.x, camPos.y, camPos.z));
        pPixelShader->SetFloat("time", totalTime);
        pPixelShader->SetShaderResourceView("diffuseTexture", pManager->GetTexture("Diffuse"));
        pPixelShader->SetShaderResourceView("rustTexture", pManager->GetTexture("Rust"));
        pPixelShader->SetShaderResourceView("specMapTexture", pManager->GetTexture("Rust_Spec"));
        pPixelShader->SetSamplerState("trilinear", pManager->GetSamplerState( "trilinear" ) );
        pPixelShader->SetShader(true);
    }

    // Set the Vertex and Index buffers
    m_pDeviceContext->IASetVertexBuffers(0, 1, &vb, &stride, &offset);
    m_pDeviceContext->IASetIndexBuffer(ib, DXGI_FORMAT_R32_UINT, 0);

    // We are drawing triangles
	m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    // Draw the GameEntity
    m_pDeviceContext->DrawIndexed(mesh->GetIndexCount(), 0, 0);
   
	// Present the buffer
	HR( m_pSwapChain->Present( 0, 0 ) );
}

void GameState::Exit( void )
{
    /* Nothing to do. */
}
