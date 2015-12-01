#ifndef POST_EFFECTS_SYSTEM_H
#define POST_EFFECTS_SYSTEM_H

#include "System.h"
#include <vector>

#include "IEventListener.h"

#include "ImageEffect.h"
#include "NoEffect.h"

enum Effect
{
	WarpBlur
};

class PostEffectsSystem : public System<PostEffectsSystem> , public IEventListener
{
public:
	PostEffectsSystem();
	virtual ~PostEffectsSystem(void);
	virtual void Update(EntityManager* pManager, float dt, float tt);
	virtual void EventRouter(const std::string& name, void* data) override;
	void EnableEffect(Effect effect);
	void SetEffectPriority(Effect effect, UINT priority);
private:
	std::vector<ImageEffect*> effects;
	NoEffect* noEffect;
};


#endif