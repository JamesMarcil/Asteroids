#include "EventListener.h"

// Not truly abstract to prevent some errors caused by abstract classes elsewhere
void EventListener::EventRouter(std::string name, void* data)
{
	return;
}