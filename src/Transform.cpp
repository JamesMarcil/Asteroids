#include "Transform.h"

Transform::Transform(){
	translation = XMFLOAT4(0, 0, 0, 1);
	rotation	= XMFLOAT4(0, 0, 0, 1);
	scale		= XMFLOAT4(1, 1, 1, 1);
	children = std::vector<Transform*>();
}

Transform::Transform(XMFLOAT4 t) {
	translation = t;
	rotation	= XMFLOAT4(0, 0, 0, 1);
	scale		= XMFLOAT4(1, 1, 1, 1);
	children = std::vector<Transform*>();
}


Transform::~Transform(){
}

#pragma region Accessors

XMFLOAT4X4 Transform::GetTransform() {
	XMMATRIX translationMat = XMMatrixTranslation(translation.x, translation.y, translation.z);
	XMMATRIX rotationMat = XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z);
	XMMATRIX scaleMat = XMMatrixScaling(scale.x, scale.y, scale.z);

	XMFLOAT4X4 toReturn;
	XMStoreFloat4x4(&toReturn, XMMatrixTranspose(scaleMat * rotationMat * translationMat));

	return toReturn;
}

XMFLOAT4 Transform::GetTranslation() {
	return translation;
}

XMFLOAT4 Transform::GetRotation() {
	return rotation;
}

XMFLOAT4 Transform::GetScale() {
	return scale;
}

std::vector<Transform*> Transform::GetChildren() {
	return children;
}

#pragma endregion

#pragma region Setters

void Transform::SetTranslation(XMVECTOR vec) {
	XMFLOAT3 container;
	XMStoreFloat3(&container, vec);
	translation = XMFLOAT4(container.x, container.y, container.z, 1);
}

void Transform::SetTranslation(XMFLOAT4 vec) {
	translation = vec;
}

void Transform::SetTranslation(float x, float y, float z) {
	translation = XMFLOAT4(x, y, z, 1);
}

void Transform::SetRotation(XMVECTOR vec, float angle) {
	XMFLOAT3 container;
	XMStoreFloat3(&container, vec);
	rotation = XMFLOAT4(container.x * angle, container.y * angle, container.z * angle, 1);
}

void Transform::SetRotation(XMFLOAT4 vec, float angle) {
	XMVECTOR rot = XMLoadFloat4(&vec) * angle;
	XMStoreFloat4(&rotation, rot);
}

void Transform::SetRotation(float x, float y, float z, float angle) {
	rotation = XMFLOAT4(x * angle, y * angle, z * angle, 1);
}

void Transform::SetScale(float num) {
	scale = XMFLOAT4(num, num, num, 1);
}

#pragma endregion

void Transform::Translate(XMVECTOR vec) {
	XMFLOAT3 container;
	XMStoreFloat3(&container, vec);

	translation.x += container.x;
	translation.y += container.y;
	translation.z += container.z;
}

void Transform::Translate(XMFLOAT3 vec) {
	translation.x += vec.x;
	translation.y += vec.y;
	translation.z += vec.z;
}

void Transform::Translate(float x, float y, float z) {
	translation.x += x;
	translation.y += y;
	translation.z += z;
}

void Transform::Rotate(XMVECTOR vec, float angle) {
	XMFLOAT3 container;
	XMStoreFloat3(&container, vec);

	rotation.x += container.x * angle;
	rotation.y += container.y * angle;
	rotation.z += container.z * angle;
}

void Transform::Rotate(XMFLOAT3 vec, float angle) {
	rotation.x += vec.x * angle;
	rotation.y += vec.y * angle;
	rotation.z += vec.z * angle;
}

void Transform::Rotate(float x, float y, float z, float angle) {
	rotation.x += x * angle;
	rotation.y += y * angle;
	rotation.z += z * angle;
}

void Transform::Scale(float num) {
	scale.x += num;
	scale.y += num;
	scale.z += num;
}