#ifndef PARTICLE_SYSTEM
#define PARTICLE_SYSTEM

#include <vector>
#include "System.h"
#include "ParticleGenerator.h"

class ParticleSystem : public System<ParticleSystem>
{
private:
	std::vector<ParticleGenerator> generators;

	virtual void Update(EntityManager* p, float dt, float tt) override;
};

#endif