#include "CollisionSphere.h"

CollisionSphere::CollisionSphere() {
}

CollisionSphere::CollisionSphere(Mesh& m, DirectX::XMFLOAT3& p, float scale) {
	position = p;
	radius = 0;
	isColliding = false;

	float distance;
	DirectX::XMFLOAT3 current;
	DirectX::XMVECTOR container;

	// Loop through vertices to find farthest from centroid
	for (int i = 0; i < m.GetVertexCount(); i += 100) {
		current = m.GetVertices()[i].Position;
		container = DirectX::XMLoadFloat3(&current);
		DirectX::XMVectorScale(container, scale);
		container = DirectX::XMVector3Length(container);
		XMStoreFloat3(&current, container); // Stores magnitude in each index

		if (current.x > radius) {
			radius = current.x;
		}
	}
}

CollisionSphere::CollisionSphere(float r, DirectX::XMFLOAT3& p) {
	position = p;
	radius = r;
	isColliding = false;
}

DirectX::XMFLOAT3 CollisionSphere::GetPosition() {
	return position;
}

float CollisionSphere::GetRadius() {
	return radius;
}

bool CollisionSphere::IsColliding() {
	return isColliding;
}

void CollisionSphere::IsColliding(bool val) {
	isColliding = val;
}

void CollisionSphere::SetPosition(DirectX::XMFLOAT3& p) {
	position = p;
}

bool CollisionSphere::CollidesWith(CollisionSphere& other) {
	float xDiff = position.x - other.GetPosition().x;
	float yDiff = position.y - other.GetPosition().y;
	float zDiff = position.z - other.GetPosition().z;
	float distance = sqrtf(xDiff * xDiff + yDiff * yDiff + zDiff * zDiff);

	if (distance < radius + other.GetRadius())
	{
		return true;
	}
	return false;
}