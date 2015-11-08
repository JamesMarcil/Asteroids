#ifndef SKYBOX_SYSTEM_H
#define SKYBOX_SYSTEM_H

#include "System.h"

class SkyboxSystem : public System<SkyboxSystem>
{
public:
    virtual ~SkyboxSystem(void);
    virtual void Update(EntityManager* pManager, float dt, float tt);
};

#endif
