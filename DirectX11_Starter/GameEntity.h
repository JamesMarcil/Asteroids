#pragma once

#include <DirectXMath.h>

#include "Transform.h"
#include "Renderer.h"

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

	GameEntity(Mesh* mesh, Material* mat);
	~GameEntity(void);

	void Update(float dt);
	void Draw();

	Renderer GetRenderer() { return renderer; }

private:
	Renderer renderer;
};

