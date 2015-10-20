#include "GameState.h"

// Managers
#include "Material.h"
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

		/* Material Creation */
		Material* defaultMat = new Material
        (
            static_cast<SimpleVertexShader*>(pManager->GetShader("StandardVertex")),
            static_cast<SimplePixelShader*>(pManager->GetShader("StandardPixel"))
        );
		defaultMat->AddTexture("diffuseTexture", "Diffuse");
		defaultMat->AddTexture("rustTexture", "Rust");
		defaultMat->AddTexture("specMapTexture", "Rust_Spec");
		pManager->RegisterMaterial("default", defaultMat);

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
        entities.emplace_back(pManager->GetMesh("Sphere"), pManager->GetMaterial("default"));
        entities.emplace_back(pManager->GetMesh("Helix"),  pManager->GetMaterial("default"));
        entities.emplace_back(pManager->GetMesh("Cube"), pManager->GetMaterial("default"));

        GameEntity* sphere = &entities[ 0 ];
        GameEntity* helix = &entities[ 1 ];
        GameEntity* cube = &entities[ 2 ];

        sphere->GetTransform()->AddChild(helix->GetTransform());
        helix->GetTransform()->AddChild(cube->GetTransform());
        helix->GetTransform()->Translate(0.0f, 0.0f, 2.0f);
        cube->GetTransform()->Translate(2.0f, 0.0f, 0.0f);

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

    // Translate the cube forward, and slowly rotate all three GameEntities
    entities[0].GetTransform()->Translate(0.5f * deltaTime, 0.0f, 0.0f);
	for (GameEntity& ge : entities)
	{
		ge.GetTransform()->Rotate(0.0f, deltaTime * 5.0f * XM_PI / 180.0f, 0.0f);
	}
}

void GameState::Render( float deltaTime, float totalTime, ID3D11RenderTargetView* const pRenderTargetView, ID3D11DepthStencilView* const pDepthStencilView )
{
    // Background color (Cornflower Blue in this case) for clearing
    const float color[4] = {0.4f, 0.6f, 0.75f, 0.0f};

    // Clear the render target and depth buffer (erases what's on the screen)
    m_pDeviceContext->ClearRenderTargetView( pRenderTargetView, color );
    m_pDeviceContext->ClearDepthStencilView( pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0 );

    // Update the Shaders
    {
        ResourceManager* pManager = ResourceManager::instance();
        ISimpleShader   *pVertexShader = pManager->GetShader( "StandardVertex" ),
                        *pPixelShader = pManager->GetShader( "StandardPixel" );
        Camera* pCamera = CameraManager::instance()->GetActiveCamera();
        XMFLOAT3 camPos = pCamera->transform.GetTranslation();

        // Update the Vertex Shader
        pVertexShader->SetMatrix4x4("view", pCamera->GetViewMatrix());
        pVertexShader->SetMatrix4x4("projection", pCamera->GetProjectionMatrix());

        // Update the Pixel Shader
        pPixelShader->SetFloat3("DirLightDirection", XMFLOAT3(1, 1, 1));
        pPixelShader->SetFloat4("DirLightColor", XMFLOAT4(0.3f, 0.3f, 0.3f, 1));
        pPixelShader->SetFloat3("PointLightPosition", XMFLOAT3(3, 3, -3));
        pPixelShader->SetFloat4("PointLightColor", XMFLOAT4(1, 1, 1, 1));
        pPixelShader->SetFloat3("CameraPosition", XMFLOAT3(camPos.x, camPos.y, camPos.z));
        pPixelShader->SetFloat("time", totalTime);
    }

    // We are drawing triangles
    m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    // Render each GameEntity
    for( GameEntity& ge : entities )
    {
        ge.Draw();
    }

    // Present the buffer
    HR( m_pSwapChain->Present( 0, 0 ) );
}

void GameState::Exit( void ) { /* Nothing to do. */ }
