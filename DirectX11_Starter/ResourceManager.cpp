#include "ResourceManager.h"
#include "WICTextureLoader.h"
#include <iterator>

ResourceManager* ResourceManager::singleton = nullptr;

ResourceManager::ResourceManager() {
	meshes   = std::map<char*, Mesh*>();
	shaders	 = std::map<char*, ISimpleShader*>();
	textures = std::map<char*, ID3D11ShaderResourceView*>();
}

ResourceManager::~ResourceManager(){
	for (std::pair<char*, Mesh*> m : meshes) {
		m.second->~Mesh();
		m.second = 0;
	}

	for (std::pair<char*, ISimpleShader*> s : shaders) {
		delete s.second;
	}

	for (std::pair<char*, ID3D11ShaderResourceView*> t : textures) {
		t.second->Release();
		t.second = 0;
	}
}

void ResourceManager::LoadResources(ID3D11Device* device, ID3D11DeviceContext* deviceContext) {
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

ResourceManager* ResourceManager::GetInstance() {
	if (singleton == nullptr) singleton = new ResourceManager();
	return singleton;
}

Mesh* ResourceManager::GetMesh(char* id) {
	return meshes[id];
}

ISimpleShader* ResourceManager::GetShader(char* id) {
	return shaders[id];
}

ID3D11ShaderResourceView* ResourceManager::GetTexture(char* id) {
	return textures[id];
}

void ResourceManager::AddMesh(char* id, Mesh* toAdd) {
	meshes.insert(std::pair<char*, Mesh*>(id, toAdd));
}

void ResourceManager::AddShader(char* id, ISimpleShader* toAdd) {
	shaders.insert(std::pair<char*, ISimpleShader*>(id, toAdd));
}

void ResourceManager::AddTexture(char* id, ID3D11ShaderResourceView* toAdd) {
	textures.insert(std::pair<char*, ID3D11ShaderResourceView*>(id, toAdd));
}