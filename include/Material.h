#pragma once
/*
TODO
Contains shader and texture information
Sends that info to shader on draw call
*/
#include "SimpleShader.h"
#include <unordered_map>
class Material
{
public:
	Material(SimpleVertexShader* vertShader, SimplePixelShader* pixelShader);
	~Material();

	void WriteShaderInfo(); // Pushes any changed variables to shaders
	void AddTexture(std::string key, std::string textureName) {
		textures.emplace(key, textureName);
	}

	SimpleVertexShader* GetVertexShader() { return vertexShader; }
	SimplePixelShader* GetPixelShader() { return pixelShader; }
private:
	SimpleVertexShader* vertexShader;
	SimplePixelShader* pixelShader;

	//TODO implement dictionary for textures
	std::unordered_map<std::string, std::string> textures;
};

