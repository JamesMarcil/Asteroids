#include "ResourceManager.h"

// DirectX
#include "WICTextureLoader.h"
#include "DXMacros.h"

// STD
#include <utility>

ResourceManager::ResourceManager() {}

ResourceManager::~ResourceManager()
{
    // Delete Meshes
	for ( auto& pair : meshes) {
        delete pair.second;
	}

    // Delete Shaders
	for ( auto& pair : shaders) {
		delete pair.second;
	}

    // Release Textures
	for ( auto& pair : textures) {
        ReleaseMacro( pair.second );
	}
}

void ResourceManager::LoadResources(ID3D11Device* device, ID3D11DeviceContext* deviceContext) 
{
	/* Mesh Creation */
	Mesh* sphereMesh = new Mesh("models/sphere.obj", device);
	Mesh* helixMesh = new Mesh("models/helix.obj", device);
	Mesh* cubeMesh = new Mesh("models/cube.obj", device);
	AddMesh("Sphere", sphereMesh);
	AddMesh("Helix", helixMesh);
	AddMesh("Cube", cubeMesh);

	/* Shader Creation */
	SimpleVertexShader* vertexShader = new SimpleVertexShader(device, deviceContext);
	vertexShader->LoadShaderFile(L"VertexShader.cso");
	SimplePixelShader* pixelShader = new SimplePixelShader(device, deviceContext);
	pixelShader->LoadShaderFile(L"PixelShader.cso");
	AddShader("StandardVertex", vertexShader);
	AddShader("StandardPixel", pixelShader);

	/* Texture Creation */
	ID3D11ShaderResourceView* diffuseTexture;
	ID3D11ShaderResourceView* rustTexture;
	ID3D11ShaderResourceView* rustSpecTexture;
	DirectX::CreateWICTextureFromFile(device, deviceContext, L"textures/crate.png", 0, &diffuseTexture);
	DirectX::CreateWICTextureFromFile(device, deviceContext, L"textures/rusty.jpg", 0, &rustTexture);
	DirectX::CreateWICTextureFromFile(device, deviceContext, L"textures/rustySpec.png", 0, &rustSpecTexture);
	AddTexture("Diffuse", diffuseTexture);
	AddTexture("Rust", rustTexture);
	AddTexture("Rust_Spec", rustSpecTexture);
}

ResourceManager* ResourceManager::instance() 
{
    static ResourceManager manager;
    return &manager;
}

Mesh* ResourceManager::GetMesh( const std::string& id ) 
{
	return meshes[id];
}

ISimpleShader* ResourceManager::GetShader( const std::string& id ) 
{
	return shaders[id];
}

ID3D11ShaderResourceView* ResourceManager::GetTexture( const std::string& id ) 
{
	return textures[id];
}

void ResourceManager::AddMesh(std::string&& id, Mesh* toAdd) 
{
    meshes.emplace( std::forward<std::string>( id ), toAdd );
}

void ResourceManager::AddShader(std::string&& id, ISimpleShader* toAdd) 
{
    shaders.emplace( std::forward<std::string>( id ), toAdd );
}

void ResourceManager::AddTexture(std::string&& id, ID3D11ShaderResourceView* toAdd) 
{
    textures.emplace( std::forward<std::string>( id ), toAdd );
}
