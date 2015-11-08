#ifndef CLEAR_SYSTEM_H
#define CLEAR_SYSTEM_H

#include "System.h"

class ClearSystem : public System<ClearSystem>
{
public:
    virtual ~ClearSystem(void);
    virtual void Update(EntityManager* pManager, float dt, float tt);
};

#endif
