#pragma once
#include "Component.h"
#include "Script.h"
#include <vector>
#include <iostream>
class ScriptComponent : public Component<ScriptComponent>
{
private:
	std::vector<Script*> scripts;
	bool deleted = false;
public:
	ScriptComponent() {}
	~ScriptComponent() {
		for (Script* script : scripts) {
			delete script;
		}
		scripts.clear();
		deleted = true;
	};

	void Update(GameEntity& entity, float dt, float tt) {
		if (deleted) return; //an update may cause this to be destroyed
		for (int i = 0; !deleted && i < scripts.size(); i++){
			scripts[i]->update(entity, dt, tt);
		}
	};

	void AddScript(Script* script) {
		scripts.push_back(script);
	};

	void RemoveScript(Script* script) {
		std::remove(scripts.begin(), scripts.end(), script);
	};
};

