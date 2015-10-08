// ----------------------------------------------------------------------------
//  A few notes on project settings
//
//  - The project is set to use the UNICODE character set
//    - This was changed in Project Properties > Config Properties > General > Character Set
//    - This basically adds a "#define UNICODE" to the project
//
//  - The include directories were automagically correct, since the DirectX 
//    headers and libs are part of the windows SDK
//    - For instance, $(WindowsSDK_IncludePath) is set as a project include 
//      path by default.  That's where the DirectX headers are located.
//
//  - Two libraries had to be manually added to the Linker Input Dependencies
//    - d3d11.lib
//    - d3dcompiler.lib
//    - This was changed in Project Properties > Config Properties > Linker > Input > Additional Dependencies
//
//  - The Working Directory was changed to match the actual .exe's 
//    output directory, since we need to load the compiled shader files at run time
//    - This was changed in Project Properties > Config Properties > Debugging > Working Directory
//
// ----------------------------------------------------------------------------

#include "MyDemoGame.h"
#include "Vertex.h"
#include "WICTextureLoader.h"
#include "InputManager.h"

// For the DirectX Math library
using namespace DirectX;


#pragma region Win32 Entry Point (WinMain)
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
	// Set up a custom caption for the game window.
	// - The "L" before the string signifies a "wide character" string
	// - "Wide" characters take up more space in memory (hence the name)
	// - This allows for an extended character set (more fancy letters/symbols)
	// - Lots of Windows functions want "wide characters", so we use the "L"
	windowCaption = L"Galactic Bulge presents: Asteroids";

	// Custom window size - will be created by Init() later
	windowWidth = 800;
	windowHeight = 600;

	camera = 0;
}

// --------------------------------------------------------
// Cleans up our DirectX stuff and any objects we need to delete
// - When you make new DX resources, you need to release them here
// - If you don't, you get a lot of scary looking messages in Visual Studio
// --------------------------------------------------------
MyDemoGame::~MyDemoGame()
{
	// Delete our simple shaders
	delete vertexShader;
	delete pixelShader;

    // Loop and remove Game Entities
    for (unsigned int i = 0; i < entities.size(); i++)
        delete entities[i];

    for (unsigned int i = 0; i < meshes.size(); i++)
        delete meshes[i];

    delete camera;

    ReleaseMacro(diffuseTexture);
    ReleaseMacro(rustTexture);
    ReleaseMacro(specMapTexture);
    ReleaseMacro(samplerState);
}

#pragma endregion

#pragma region Initialization

// --------------------------------------------------------
// Initializes the base class (including the window and D3D),
// sets up our geometry and loads the shaders (among other things)
// --------------------------------------------------------
bool MyDemoGame::Init()
{
	// Call the base class's Init() method to create the window,
	// initialize DirectX, etc.
	if( !DirectXGameCore::Init() )
		return false;

	// Helper methods to create something to draw, load shaders to draw it 
	// with and set up matrices so we can see how to pass data to the GPU.
	//  - For your own projects, feel free to expand/replace these.
	CreateGeometry();
	LoadShaders();
	CreateMatrices();

	// Tell the input assembler stage of the pipeline what kind of
	// geometric primitives we'll be using and how to interpret them
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Successfully initialized
	return true;
}

// --------------------------------------------------------
// Creates the geometry we're going to draw - a single triangle for now
// --------------------------------------------------------
void MyDemoGame::CreateGeometry()
{
    XMFLOAT4 red = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
    XMFLOAT4 green = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
    XMFLOAT4 blue = XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);
    XMFLOAT4 yellow = XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f);

    
    Mesh* sphereMesh = new Mesh("models/sphere.obj", device);
    Mesh* helixMesh = new Mesh("models/helix.obj", device);
    Mesh* cubeMesh = new Mesh("models/cube.obj", device);

    meshes.push_back(sphereMesh);
    meshes.push_back(helixMesh);
    meshes.push_back(cubeMesh);

    // Make some entities
    GameEntity* sphere = new GameEntity(sphereMesh);
    GameEntity* helix = new GameEntity(helixMesh);
    GameEntity* cube = new GameEntity(cubeMesh);
    entities.push_back(sphere);
    entities.push_back(helix);
    entities.push_back(cube);

    currentEntity = 0;
}

// --------------------------------------------------------
// Loads shaders from compiled shader object (.cso) files
// - These simple shaders provide helpful methods for sending
//   data to individual variables on the GPU
// --------------------------------------------------------
void MyDemoGame::LoadShaders()
{
	vertexShader = new SimpleVertexShader(device, deviceContext);
	vertexShader->LoadShaderFile(L"VertexShader.cso");

	pixelShader = new SimplePixelShader(device, deviceContext);
	pixelShader->LoadShaderFile(L"PixelShader.cso");

    // Load textures
    DirectX::CreateWICTextureFromFile(device, deviceContext, L"textures/crate.png", 0, &diffuseTexture);
    DirectX::CreateWICTextureFromFile(device, deviceContext, L"textures/rusty.jpg", 0, &rustTexture);
    DirectX::CreateWICTextureFromFile(device, deviceContext, L"textures/rustySpec.png", 0, &specMapTexture);

    // Fill out a description and then create the sampler state
    D3D11_SAMPLER_DESC samplerDesc;
    ZeroMemory(&samplerDesc, sizeof(samplerDesc));
    samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
    device->CreateSamplerState(&samplerDesc, &samplerState);

}

// --------------------------------------------------------
// Initializes the matrices necessary to represent our geometry's 
// transformations and our 3D camera
// --------------------------------------------------------
void MyDemoGame::CreateMatrices()
{
	// Set up the camera
	camera = new Camera(0, 0, -5);
	camera->UpdateProjectionMatrix(aspectRatio);
}

#pragma endregion

#pragma region Window Resizing

// --------------------------------------------------------
// Handles resizing DirectX "stuff" to match the (usually) new
// window size and updating our projection matrix to match
// --------------------------------------------------------
void MyDemoGame::OnResize()
{
	// Handle base-level DX resize stuff
	DirectXGameCore::OnResize();

	if (camera != 0)
	{
		camera->UpdateProjectionMatrix(aspectRatio);
	}
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

    // Update the GameEntity position
    entities[currentEntity]->UpdateWorldMatrix();

    // Check for entity swap
    bool currentSpacebar = pManager->IsKeyDown( VK_TAB );
	if ( currentSpacebar && !prevSpaceBar )
	{
        currentEntity = (currentEntity + 1) % entities.size();
	}
    prevSpaceBar = currentSpacebar;

	// Update the camera
	camera->Update(deltaTime);
}

// --------------------------------------------------------
// Clear the screen, redraw everything, present to the user
// --------------------------------------------------------
void MyDemoGame::DrawScene(float deltaTime, float totalTime)
{
	// Background color (Cornflower Blue in this case) for clearing
	const float color[4] = {0.4f, 0.6f, 0.75f, 0.0f};

	// Clear the render target and depth buffer (erases what's on the screen)
	//  - Do this ONCE PER FRAME
	//  - At the beginning of DrawScene (before drawing *anything*)
	deviceContext->ClearRenderTargetView(renderTargetView, color);
	deviceContext->ClearDepthStencilView(
		depthStencilView, 
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f,
		0);


	

    
    GameEntity* ge = entities[currentEntity];
    ID3D11Buffer* vb = ge->GetMesh()->GetVertexBuffer();
    ID3D11Buffer* ib = ge->GetMesh()->GetIndexBuffer();

    // Set buffers in the input assembler
    UINT stride = sizeof(Vertex);
    UINT offset = 0;
    deviceContext->IASetVertexBuffers(0, 1, &vb, &stride, &offset);
    deviceContext->IASetIndexBuffer(ib, DXGI_FORMAT_R32_UINT, 0);

    vertexShader->SetMatrix4x4("world", *ge->GetWorldMatrix());
    vertexShader->SetMatrix4x4("view", camera->GetView());
    vertexShader->SetMatrix4x4("projection", camera->GetProjection());

    // Pass in some light data to the pixel shader
    pixelShader->SetFloat3("DirLightDirection", XMFLOAT3(1, 1, 1));
    pixelShader->SetFloat4("DirLightColor", XMFLOAT4(0.3f, 0.3f, 0.3f, 1));

    pixelShader->SetFloat3("PointLightPosition", XMFLOAT3(3, 3, -3));
    pixelShader->SetFloat4("PointLightColor", XMFLOAT4(1, 1, 1, 1));

    pixelShader->SetFloat3("CameraPosition", camera->GetPosition());

    pixelShader->SetFloat("time", totalTime);

    pixelShader->SetShaderResourceView("diffuseTexture", diffuseTexture);
    pixelShader->SetShaderResourceView("rustTexture", rustTexture);
    pixelShader->SetShaderResourceView("specMapTexture", specMapTexture);
    pixelShader->SetSamplerState("trilinear", samplerState);

    vertexShader->SetShader(true);
    pixelShader->SetShader(true);

    // Finally do the actual drawingw
    deviceContext->DrawIndexed(ge->GetMesh()->GetIndexCount(), 0, 0);
   

	// Present the buffer
	//  - Puts the image we're drawing into the window so the user can see it
	//  - Do this exactly ONCE PER FRAME
	//  - Always at the very end of the frame
	HR(swapChain->Present(0, 0));
}
#pragma endregion
