#include "Camera.h"
#include "InputManager.h"

using namespace DirectX;

// Creates a camera at the specified position
Camera::Camera(float x, float y, float z)
{
	position = XMFLOAT3(x, y, z);
    startPosition = XMFLOAT3(x, y, z);
	XMStoreFloat4(&rotation, XMQuaternionIdentity());
	xRotation = 0;
	yRotation = 0;

	XMStoreFloat4x4(&viewMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&projMatrix, XMMatrixIdentity());
}

// Nothing to really do
Camera::~Camera()
{ }


// Moves the camera relative to its orientation
void Camera::MoveRelative(float x, float y, float z)
{
	// Rotate the desired movement vector
	XMVECTOR dir = XMVector3Rotate(XMVectorSet(x, y, z, 0), XMLoadFloat4(&rotation));
	
	// Move in that direction
	XMStoreFloat3(&position, XMLoadFloat3(&position) + dir);
}

// Moves the camera in world space (not local space)
void Camera::MoveAbsolute(float x, float y, float z)
{
    // Simple add, no need to load/store
    position.x += x;
    position.y += y;
    position.z += z;
}

// Rotate on the X and/or Y axis
void Camera::Rotate(float x, float y)
{
    // Adjust the current rotation
	xRotation += x;
	yRotation += y;

    // Clamp the x between PI/2 and -PI/2
    xRotation = max(min(xRotation, XM_PIDIV2), -XM_PIDIV2);

    // Recreate the quaternion
	XMStoreFloat4(&rotation, XMQuaternionRotationRollPitchYaw(xRotation, yRotation, 0));
}

// Camera's update, which looks for key presses
void Camera::Update(float dt)
{
    // Current speed
	float speed = dt * 3;

    InputManager* pManager = InputManager::instance();

    // Speed up when shift is pressed
    if( pManager->IsKeyDown( VK_SHIFT ) ){ speed += 5; }

    // Movement
    if( pManager->IsKeyDown( 'W' ) ){ MoveRelative( 0, 0, speed ); }
    if( pManager->IsKeyDown( 'S' ) ){ MoveRelative( 0, 0, -speed ); }
    if( pManager->IsKeyDown( 'A' ) ){ MoveRelative( -speed, 0, 0 ); }
    if( pManager->IsKeyDown( 'D' ) ){ MoveRelative( speed, 0, 0 ); }
    if( pManager->IsKeyDown( 'X' ) ){ MoveAbsolute( 0, -speed, 0 ); }
    if( pManager->IsKeyDown( ' ' ) ){ MoveAbsolute( 0, speed, 0 ); }

    // Handle rotation
	if ( pManager->IsMouseDown( MouseButton::LMB ) )
	{
        POINT   prevMousePos = pManager->GetPreviousMousePos(),
                currMousePos = pManager->GetCurrentMousePos();

		float xDiff = min( 5.0f, ( currMousePos.x - prevMousePos.x ) * 5.0f * dt );
		float yDiff = min( 5.0f, ( currMousePos.y - prevMousePos.y ) * 5.0f * dt );

		Rotate( yDiff, xDiff );
	}

    // Check for reset
    if ( pManager->IsKeyDown( 'R' ) )
    {
        position = startPosition;
        xRotation = 0.0f;
        xRotation = 0.0f;
        XMStoreFloat4(&rotation, XMQuaternionIdentity());
    }

    // Update the view every frame - could be optimized
	UpdateViewMatrix();
}

// Creates a new view matrix based on current position and orientation
void Camera::UpdateViewMatrix()
{
    // Rotate the standard "forward" matrix by our rotation
    // This gives us our "look direction"
	XMVECTOR dir = XMVector3Rotate(XMVectorSet(0, 0, 1, 0), XMLoadFloat4(&rotation));

	XMMATRIX view = XMMatrixLookToLH(
		XMLoadFloat3(&position),
		dir,
		XMVectorSet(0, 1, 0, 0));

	XMStoreFloat4x4(&viewMatrix, XMMatrixTranspose(view));
}

// Updates the projection matrix
void Camera::UpdateProjectionMatrix(float aspectRatio)
{
	XMMATRIX P = XMMatrixPerspectiveFovLH(
		0.25f * 3.1415926535f,		// Field of View Angle
		aspectRatio,				// Aspect ratio
		0.1f,						// Near clip plane distance
		100.0f);					// Far clip plane distance
	XMStoreFloat4x4(&projMatrix, XMMatrixTranspose(P)); // Transpose for HLSL!
}
