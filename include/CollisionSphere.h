#ifndef COLLISION_SPHERE
#define COLLISION_SPHERE

#include <DirectXMath.h>
#include "Mesh.h"

class CollisionSphere
{
private:
	DirectX::XMFLOAT3 position;
	float radius;
	bool isColliding;

public:
	CollisionSphere();
	CollisionSphere(Mesh& m, DirectX::XMFLOAT3& p, float scale);
	CollisionSphere(float r, DirectX::XMFLOAT3& p);

	DirectX::XMFLOAT3 GetPosition();
	float GetRadius();
	bool IsColliding();
	void IsColliding(bool val);
	void SetPosition(DirectX::XMFLOAT3& pos);

	bool CollidesWith(CollisionSphere& other);
};

#endif