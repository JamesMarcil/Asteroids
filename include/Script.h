#pragma once

#include "GameEntity.h"

class Script
{
public:
	virtual ~Script() {};

	virtual void update(GameEntity entity, float dt, float tt) = 0;
};