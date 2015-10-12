#pragma once

#include <DirectXMath.h>

using namespace DirectX;

class Transform
{
private:
	XMFLOAT4 translation;
	XMFLOAT4 rotation;
	XMFLOAT4 scale;

public:
	Transform();
	Transform(XMFLOAT4 t);
	~Transform();

	/* Accessors */
	XMFLOAT4X4 GetTransform();
	XMFLOAT4 GetTranslation();
	XMFLOAT4 GetRotation();
	XMFLOAT4 GetScale();

	/* Setters */
	void SetTranslation(XMVECTOR vec);
	void SetTranslation(XMFLOAT4 vec);
	void SetTranslation(float x, float y, float z);

	void SetRotation(XMVECTOR vec, float angle);
	void SetRotation(XMFLOAT4 vec, float angle);
	void SetRotation(float x, float y, float z, float angle);

	void SetScale(float num);

	/* Transformation Functions */
	void Translate(XMVECTOR vec);
	void Translate(XMFLOAT3 vec);
	void Translate(float x, float y, float z);

	void Rotate(XMVECTOR vec, float angle);
	void Rotate(XMFLOAT3 vec, float angle);
	void Rotate(float x, float y, float z, float angle);

	void Scale(float num);
};