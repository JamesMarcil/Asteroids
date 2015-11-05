
#include "Singleton.h"
#include "Event.h"
#include <unordered_map>
#include <string>

class EventManager : public Singleton<EventManager> 
{
public:

	EventManager(void)
	{
		Register("test", &TestFunction);


	}

	virtual ~EventManager()
	{

	}

	void TestFunction(Event evt)
	{

	}

	void Register(string name, void(*callback)(Event));
	bool Fire(string name, uintptr_t data);

private:
	std::unordered_map<string, std::vector<void(*) (Event)>> callbacks;
	
};

