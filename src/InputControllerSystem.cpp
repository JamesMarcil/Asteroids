#include "InputControllerSystem.h"

#include "EntityManager.h" // Need to to include this to access EntityManager::* functions.
#include "InputManager.h"

// Components
#include "TransformComponent.h"
#include "PhysicsComponent.h"
#include "InputComponent.h"

#include "SimpleMath.h"
// DirectX
#include <DirectXMath.h>
void InputControllerSystem::Update(EntityManager* pManager, float dt, float tt)
{
	using namespace DirectX;
	using namespace SimpleMath;

	InputManager* pInput = InputManager::Instance();

	float horiz = (pInput->IsKeyDown('A') ? -1 : 0) + (pInput->IsKeyDown('D') ? 1 : 0);
	float vert = (pInput->IsKeyDown('S') ? -1 : 0) + (pInput->IsKeyDown('W') ? 1 : 0);
	float roll = (pInput->IsKeyDown('E') ? -1 : 0) + (pInput->IsKeyDown('Q') ? 1 : 0);

	for (auto& entity : pManager->EntitiesWithComponents<InputComponent, PhysicsComponent, TransformComponent>())
	{
		TransformComponent* pTransform = pManager->GetComponent<TransformComponent>(entity);
		Transform& t = pTransform->transform;

		InputComponent* inputCom = pManager->GetComponent<InputComponent>(entity);
		PhysicsComponent* pPhysics = pManager->GetComponent<PhysicsComponent>(entity);

		Vector3 rot = Vector3(t.GetRotationEuler().x, t.GetRotationEuler().y, t.GetRotationEuler().z);
		Vector3 horizAdd = Vector3(cos(rot.z) * horiz, sin(rot.z) * horiz, 0);
		Vector3 vertAdd  = Vector3(-sin(rot.z) * vert, cos(rot.z) * vert, 0);
		Vector3 accel = horizAdd + vertAdd;
		accel.Normalize();
		accel *= inputCom->movementSpeed;

		pPhysics->acceleration = XMFLOAT3(accel.x, accel.y, 0);

		//t.Rotate(0, 0, roll*0.003f);
		pPhysics->rotationalAcceleration = XMFLOAT3(0, 0, roll*120.0f);
	}
}
