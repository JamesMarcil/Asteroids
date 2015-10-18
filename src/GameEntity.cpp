#include "GameEntity.h"

using namespace DirectX;

GameEntity::GameEntity(Mesh* mesh, Material* mat)
{
	this->renderer = Renderer(mesh, mat);
}

GameEntity::~GameEntity(void)
{
	
}

void GameEntity::Update(float dt) {

}

void GameEntity::Draw()
{
	renderer.Draw(transform);
}
