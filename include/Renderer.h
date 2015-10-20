#pragma once
#include "Mesh.h"
#include "Material.h"
#include "Transform.h"
#include "SimpleShader.h"
#include "ResourceManager.h"

class Renderer
{
public:
	Renderer() {}
	Renderer(Mesh*, Material*);
	~Renderer();

	void Draw(Transform& t);

	Mesh* GetMesh() { return mesh; }
	Material* GetMaterial() { return material; }

	void SetMesh(Mesh* mesh) { this->mesh = mesh; }
	void SetMaterial(Material* material) { this->material = material;  }

protected:
	Mesh* mesh;
	Material* material;
	ID3D11DeviceContext* deviceContext = nullptr;
};

