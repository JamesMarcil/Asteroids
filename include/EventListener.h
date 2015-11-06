#pragma once
#include <string>

// Inherit from EventListener and Override EventRouter.
// Pass 'this' into EventManager.Register with the event name
class EventListener
{

public:
	EventListener(){}
	~EventListener() {}

	virtual void EventRouter(std::string name, void* data);
};