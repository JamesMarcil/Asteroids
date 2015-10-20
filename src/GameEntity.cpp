#include "GameEntity.h"

GameEntity::GameEntity(Mesh* mesh, Material* mat)
    : renderer( mesh, mat )
{ /* Nothing to do. */ }

GameEntity::~GameEntity(void)
{ /* Nothing to do. */ }

void GameEntity::Update(float dt)
{ /* Nothing to do. */ }

void GameEntity::Draw()
{
	renderer.Draw(transform);
}
