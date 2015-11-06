
#include "Singleton.h"
#include "EventListener.h"
#include <unordered_map>
#include <string>

using namespace std;

class EventManager : public Singleton<EventManager>, EventListener
{
public:
	EventManager(void)
	{

	}

	virtual ~EventManager()
	{

	}

	void Register(string name, EventListener* listener);

	void UnRegister(string name, EventListener* listener);

	void Fire(string name, void* data);
private:

	unordered_map<string, vector<EventListener*>> listeners;
	
};

