#pragma once

#include <DirectXMath.h>
#include "Mesh.h"

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
	GameEntity(Mesh* mesh);
	~GameEntity(void);

	void UpdateWorldMatrix();

	void Move(float x, float y, float z)		{ position.x += x;	position.y += y;	position.z += z; }
	void Rotate(float x, float y, float z)		{ rotation.x += x;	rotation.y += y;	rotation.z += z; }

	void SetPosition(float x, float y, float z) { position.x = x;	position.y = y;		position.z = z; }
	void SetRotation(float x, float y, float z) { rotation.x = x;	rotation.y = y;		rotation.z = z; }
	void SetScale(float x, float y, float z)	{ scale.x = x;		scale.y = y;		scale.z = z; }

	Mesh* GetMesh() { return mesh; }
	DirectX::XMFLOAT4X4* GetWorldMatrix() { return &worldMatrix; }
private:

	Mesh* mesh;
	
	DirectX::XMFLOAT4X4 worldMatrix;
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 rotation;
	DirectX::XMFLOAT3 scale;

};

