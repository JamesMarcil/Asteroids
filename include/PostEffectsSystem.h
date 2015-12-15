#ifndef POST_EFFECTS_SYSTEM_H
#define POST_EFFECTS_SYSTEM_H

// ECS
#include "System.h"

// IEventListener Interface
#include "IEventListener.h"

// IEffect Interface
#include "IEffect.h"
#include "NoEffect.h"

// STD
#include <vector>
#include <unordered_map>
#include <string>

// Forward Declarations
class EventManager;
class ResourceManager;

enum Effect
{
	WarpBlur
};

class PostEffectsSystem : public System<PostEffectsSystem> , public IEventListener
{
private:
    ResourceManager* m_pResource;
    EventManager* m_pEvent;
	std::vector<IEffect*> effectsOrdered;
	std::unordered_map<std::string, IEffect*> effectsMap;
	NoEffect* noEffect;
	float warpTime{2.0f};
	float timeElapsed{0.0f};
	bool warping{false};

	void EnableEffect(Effect effect);
	void SetEffectPriority(Effect effect, UINT priority);
    void CreatePostEffectRTVsAndSRVs(void);
public:
	PostEffectsSystem(void);
	virtual ~PostEffectsSystem(void);

	virtual void Update(EntityManager* pManager, float dt, float tt);
	virtual void EventRouter(const std::string& name, void* data) override;
};


#endif
