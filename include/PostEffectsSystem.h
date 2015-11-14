#ifndef POST_EFFECTS_SYSTEM_H
#define POST_EFFECTS_SYSTEM_H

#include "System.h"

class PostEffectsSystem : public System<PostEffectsSystem>
{
public:
	virtual ~PostEffectsSystem(void);
	virtual void Update(EntityManager* pManager, float dt, float tt);
};


#endif