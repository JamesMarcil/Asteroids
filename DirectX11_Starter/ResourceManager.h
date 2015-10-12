#pragma once

#include <map>
#include "Mesh.h"
#include "SimpleShader.h"

class ResourceManager
{
private:
	static ResourceManager* singleton;

	std::map<char*, Mesh*> meshes;
	std::map<char*, ISimpleShader*> shaders;
	std::map<char*, ID3D11ShaderResourceView*> textures;

	ResourceManager();
	~ResourceManager();

public:
	static ResourceManager* GetInstance();

	void LoadResources(ID3D11Device* device, ID3D11DeviceContext* deviceContext);

	Mesh* GetMesh(char* id);
	ISimpleShader* GetShader(char* id);
	ID3D11ShaderResourceView* GetTexture(char* id);

	void AddMesh(char* id, Mesh* toAdd);
	void AddShader(char* id, ISimpleShader* toAdd);
	void AddTexture(char* id, ID3D11ShaderResourceView* toAdd);

};

