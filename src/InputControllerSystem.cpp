#include "InputControllerSystem.h"

// Managers
#include "EntityManager.h" // Need to to include this to access EntityManager::* functions.
#include "InputManager.h"
#include "CameraManager.h"

// ECS
#include "TransformComponent.h"
#include "PhysicsComponent.h"
#include "InputComponent.h"

// DirectX
#include <DirectXMath.h>

// STD
#include <cmath>

InputControllerSystem::InputControllerSystem(void)
{
	m_pCamera = CameraManager::Instance();
	m_pInput = InputManager::Instance();
}

InputControllerSystem::~InputControllerSystem(void)
{
	/* Nothing to do. */
}

void InputControllerSystem::Update(EntityManager* pManager, float dt, float tt)
{
	using namespace DirectX;

	if (m_pCamera->IsDebugActive()) return; //Don't overlap controls

    bool isWHeld = (m_pInput->IsKeyDown('W'));
    bool isAHeld = (m_pInput->IsKeyDown('A'));
    bool isSHeld = (m_pInput->IsKeyDown('S'));
    bool isDHeld = (m_pInput->IsKeyDown('D'));
	float horiz = (isAHeld ? -1.0f : 0.0f) + (isDHeld ? 1.0f : 0.0f);
	float vert  = (isSHeld ? -1.0f : 0.0f) + (isWHeld ? 1.0f : 0.0f);

	for (auto& entity : pManager->EntitiesWithComponents<InputComponent, PhysicsComponent, TransformComponent>())
	{
		InputComponent* inputCom = pManager->GetComponent<InputComponent>(entity);
		PhysicsComponent* pPhysics = pManager->GetComponent<PhysicsComponent>(entity);
		TransformComponent* pTransform = pManager->GetComponent<TransformComponent>(entity);
		Transform& t = pTransform->transform;

        // Update acceleration.
        XMVECTOR acceleration = XMVectorSet(horiz, vert, 0.0f, 0.0f);
        acceleration = XMVector3Normalize(acceleration);
        acceleration = XMVectorScale(acceleration, inputCom->movementSpeed);

		float maxRot = XM_PI / 8.0f;
		XMFLOAT3 rot = t.GetRotationEuler();
        XMVECTOR rotVelocity = XMLoadFloat3(&pPhysics->rotationalVelocity);
        XMVECTOR rotAcceleration = XMLoadFloat3(&pPhysics->rotationalAcceleration);

        // Handle rotation around the z-axis.
        if(isAHeld || isDHeld)
        {
            // Rotating towards left/right.
            bool rotatingOppositeDirection = (isDHeld && rot.z > 0.0f) || (isAHeld && rot.z < 0.0f);
            if (std::abs(rot.z) <= maxRot || rotatingOppositeDirection)
            {
                rotAcceleration = XMVectorSet(XMVectorGetX(rotAcceleration), 0.0f, -horiz * 5.0f, 0.0f);
            }
            // Stop if still held down but we have reached maximum rotation.
            else
            {
                rotVelocity = XMVectorSet(XMVectorGetX(rotVelocity), 0.0f, 0.0f, 0.0f);
                rotAcceleration = XMVectorSet(XMVectorGetX(rotAcceleration), 0.0f, 0.0f, 0.0f);
            }
        }
        // Rotate the ship back towards 0.
        else if (std::abs(rot.z) > 1.0f * (XM_PI / 180.0f))
        {
            float sign = (rot.z > 0.0f) ? -1.0f : 1.0f;
			rotAcceleration = XMVectorSet(XMVectorGetX(rotAcceleration), 0.0f, sign * 5.0f, 0.0f);
		}
        // Snap back to 0 once we are within 1 degree.
        else
        {
            rot.z = 0.0f;
            rotVelocity = XMVectorSet(XMVectorGetX(rotVelocity), 0.0f, 0.0f, 0.0f);
            rotAcceleration = XMVectorSet(XMVectorGetX(rotAcceleration), 0.0f, 0.0f, 0.0f);
        }

        // Handle rotation around x-axis.
        if(isWHeld || isSHeld)
        {
            // Rotating towards up/down.
            bool rotatingOppositeDirection = (isWHeld && rot.x > 0.0f) || (isSHeld && rot.x < 0.0f);
            if (std::abs(rot.x) <= maxRot || rotatingOppositeDirection)
            {
                rotAcceleration = XMVectorSet(-vert * 5.0f, 0.0f, XMVectorGetZ(rotAcceleration), 0.0f);
            }
            // Stop if still held down but we have reached maximum rotation.
            else
            {
                rotVelocity = XMVectorSet(0.0f, 0.0f, XMVectorGetZ(rotVelocity), 0.0f);
                rotAcceleration = XMVectorSet(0.0f, 0.0f, XMVectorGetZ(rotAcceleration), 0.0f);
            }
        }
        // Rotate the ship back towards 0.
		else if(std::abs(rot.x) > 1.0f * (XM_PI / 180.0f))
        {
            float sign = (rot.x > 0.0f) ? -1.0f : 1.0f;
			rotAcceleration = XMVectorSet(sign * 5.0f, 0.0f, XMVectorGetZ(rotAcceleration), 0.0f);
		}
        // Snap back once we are within 1 degree.
        else
        {
            rot.x = 0.0f;
            rotVelocity = XMVectorSet(0.0f, 0.0f, XMVectorGetZ(rotVelocity), 0.0f);
            rotAcceleration = XMVectorSet(0.0f, 0.0f, XMVectorGetZ(rotAcceleration), 0.0f);
        }

        // Cleaning issues caused by euler rotation.
		t.SetRotation(rot.x, 0, rot.z);

        // Save results.
        XMStoreFloat3(&pPhysics->acceleration, acceleration);
        XMStoreFloat3(&pPhysics->rotationalVelocity, rotVelocity);
        XMStoreFloat3(&pPhysics->rotationalAcceleration, rotAcceleration);
	}
}
