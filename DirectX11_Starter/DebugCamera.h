#ifndef DEBUG_CAMERA_h
#define DEBUG_CAMERA_H

#include "Camera.h"

class DebugCamera : public Camera
{
public:
	/*
	 *	Construct an instance of the DebugCamera at the origin.
	 */
	DebugCamera();

	/*
	 * Construct an instance of the DebugCamera at the provided location.
	 */
	DebugCamera(float x, float y, float z);

	/*
	 * Default Destructor
	 */
	virtual ~DebugCamera();

	/*
	 *	Update the position of the DebugCamera based on user input.
	 *	@param	dt		The deltaTime value this frame.
	 */
	virtual void Update(float dt) override;
};

#endif
