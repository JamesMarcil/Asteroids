#include "EventManager.h"

void EventManager::Register(string name, void(*callback)(Event))
{
	std::vector<void(*)(Event)> methods = callbacks[name];
	for (auto& ec : methods)
	{
		if (ec == callback)
			return;
	}

	methods.emplace_back(callback);
	callbacks[name] = methods;
}

bool EventManager::Fire(string name, uintptr_t data)
{
	if (callbacks[name].empty)
		return false;

	std::vector<void(*)(Event)> methods = callbacks[name];

	for (auto& ec : methods)
	{
		ec({ name,data });
	}

	return true;
}
