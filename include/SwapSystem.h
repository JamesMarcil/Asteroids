#ifndef SWAP_SYSTEM_H
#define SWAP_SYSTEM_H

#include "System.h"

class SwapSystem : public System<SwapSystem>
{
public:
    virtual ~SwapSystem(void);
    virtual void Update(EntityManager* pManager, float dt, float tt);
};

#endif
