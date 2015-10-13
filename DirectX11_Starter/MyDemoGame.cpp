#include "MyDemoGame.h"

// Managers
#include "InputManager.h"
#include "CameraManager.h"
#include "ResourceManager.h"

// DirectX
#include <DirectXMath.h>
#include "Vertex.h"
#include "SimpleShader.h"

// For the DirectX Math library
using namespace DirectX;

#pragma region Win32 Entry Point (WinMain)

// Include run-time memory checking in debug builds, so 
// we can be notified of memory leaks
#if defined(DEBUG) || defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif

// --------------------------------------------------------
// Win32 Entry Point - Where your program starts
// --------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance,
				   PSTR cmdLine, int showCmd)
{
	// Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif

	// Create the game object.
	MyDemoGame game(hInstance);
	
	// This is where we'll create the window, initialize DirectX, 
	// set up geometry and shaders, etc.
	if( !game.Init() )
		return 0;
	
	// All set to run the game loop
	return game.Run();
}

#pragma endregion

#pragma region Constructor / Destructor
// --------------------------------------------------------
// Base class constructor will set up all of the underlying
// fields, and then we can overwrite any that we'd like
// --------------------------------------------------------
MyDemoGame::MyDemoGame(HINSTANCE hInstance) 
	: DirectXGameCore(hInstance)
{
	windowCaption = L"Galactic Bulge presents: Asteroids";

	// Custom window size - will be created by Init() later
	windowWidth = 800;
	windowHeight = 600;
}

MyDemoGame::~MyDemoGame() {}

#pragma endregion

#pragma region Initialization

// --------------------------------------------------------
// Initializes the base class (including the window and D3D),
// sets up our geometry and loads the shaders (among other things)
// --------------------------------------------------------
bool MyDemoGame::Init()
{
	if( !DirectXGameCore::Init() )
		return false;

    {
        ResourceManager* pManager = ResourceManager::instance();
        pManager->RegisterDeviceAndContext( device, deviceContext );

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
    }

	currentEntity = 1;

	// Successfully initialized
	return true;
}

#pragma endregion

#pragma region Game Loop

// --------------------------------------------------------
// Update your game here - take input, move objects, etc.
// --------------------------------------------------------
void MyDemoGame::UpdateScene(float deltaTime, float totalTime)
{
	// Quit if the escape key is pressed
	InputManager* pManager = InputManager::instance();
	if ( pManager->IsKeyDown( VK_ESCAPE ) )
    {
		Quit();
    }

    // Check for entity swap
    bool currentSpacebar = pManager->IsKeyDown( VK_TAB );
	if ( currentSpacebar && !prevSpaceBar )
	{
        currentEntity = (currentEntity + 1) % entities.size();
	}
    prevSpaceBar = currentSpacebar;
}

// --------------------------------------------------------
// Clear the screen, redraw everything, present to the user
// --------------------------------------------------------
void MyDemoGame::DrawScene(float deltaTime, float totalTime)
{
	// Background color (Cornflower Blue in this case) for clearing
	const float color[4] = {0.4f, 0.6f, 0.75f, 0.0f};

	// Clear the render target and depth buffer (erases what's on the screen)
	deviceContext->ClearRenderTargetView( renderTargetView, color );
	deviceContext->ClearDepthStencilView( depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0 );

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
    deviceContext->IASetVertexBuffers(0, 1, &vb, &stride, &offset);
    deviceContext->IASetIndexBuffer(ib, DXGI_FORMAT_R32_UINT, 0);

    // We are drawing triangles
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    // Draw the GameEntity
    deviceContext->DrawIndexed(mesh->GetIndexCount(), 0, 0);
   
	// Present the buffer
	HR(swapChain->Present(0, 0));
}
#pragma endregion
