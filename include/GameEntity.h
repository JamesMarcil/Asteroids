#ifndef GAME_ENTITY_H
#define GAME_ENTITY_H

// DirectX
#include <DirectXMath.h>
#include "Renderer.h"

#include "Transform.h"

/*
TODO 
Handle velocity and movement
Abstract this class for use as custom entities
*/
class GameEntity
{
public:

	GameEntity(Mesh* mesh, Material* mat);
	~GameEntity(void);

	void Update(float dt);
	void Draw();

	Renderer GetRenderer() { return renderer; }
    Transform* GetTransform() { return &transform; }

private:
	Renderer renderer;
	Transform transform;
};

#endif
