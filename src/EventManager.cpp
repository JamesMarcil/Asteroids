#include "EventManager.h"

// Register an EventListener Class with the specified event
void EventManager::Register(string name, EventListener* listener)
{
	// make sure there are no duplicate registers
	for (auto& l : listeners[name])
	{
		if (l == listener)
			return;
	}

	listeners[name].emplace_back(listener);
}

// Remove the Given EventListener Class from the Given Event
void EventManager::UnRegister(string name, EventListener* listener)
{
	vector<EventListener*> list = listeners[name];

	vector<EventListener*>::iterator it = list.begin();

	while (it != list.end())
	{
		if (*it == listener)
		{
			list.erase(it);
			break;
		}
	}
}

// Find all the EventListeners Registered to the Event and Route the Event
void EventManager::Fire(string name, void* data)
{
	for (auto& l : listeners[name])
	{
		l->EventRouter(name, data);
	}
}

