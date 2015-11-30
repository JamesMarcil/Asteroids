#ifndef POST_EFFECTS_SYSTEM_H
#define POST_EFFECTS_SYSTEM_H

#include "System.h"

#include "IEventListener.h"

class PostEffectsSystem : public System<PostEffectsSystem> , public IEventListener
{
public:
	PostEffectsSystem();
	virtual ~PostEffectsSystem(void);
	virtual void Update(EntityManager* pManager, float dt, float tt);
	virtual void EventRouter(const std::string& name, void* data) override;
private:
	 
};


#endif