#ifndef SKYBOX_SYSTEM_H
#define SKYBOX_SYSTEM_H

#include <string>
#include "System.h"
#include "IEventListener.h"

class SkyboxSystem : public System<SkyboxSystem>, public IEventListener
{
public:
	SkyboxSystem();
    virtual ~SkyboxSystem(void);
    virtual void Update(EntityManager* pManager, float dt, float tt);
	virtual void EventRouter(const std::string& name, void* data) override;
private:
	std::string textureNames[3];
	int currentTexture;
	bool warping;
	float lerpT = 1;
	float timeElapsed;
	float warpTime = 2;
};

#endif
