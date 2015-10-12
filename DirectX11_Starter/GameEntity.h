#pragma once

#include <DirectXMath.h>
#include "Mesh.h"
#include "Transform.h"

/*
TODO 
Split off rendering to Renderer class
Split off position to Transform class
Handle velocity and movement
Abstract this class for use as custom entities
*/
class GameEntity
{
public:
	Transform transform;

	GameEntity(Mesh* mesh);
	~GameEntity(void);

	void Update(float dt);

	Mesh* GetMesh() { return mesh; }

private:
	Mesh* mesh;
};

