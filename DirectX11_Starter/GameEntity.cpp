#include "GameEntity.h"

using namespace DirectX;

GameEntity::GameEntity(Mesh* mesh)
{
	// Save the mesh
	this->mesh = mesh;
}

GameEntity::~GameEntity(void)
{
}

void GameEntity::Update(float dt) {

}
