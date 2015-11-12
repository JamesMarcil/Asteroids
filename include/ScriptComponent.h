#pragma once
#include "Component.h"
#include "Script.h"
#include <vector>
#include <iostream>
class ScriptComponent : public Component<ScriptComponent>
{
private:
	std::vector<Script*> scripts;
public:
	ScriptComponent() {}
	~ScriptComponent() {
		for (Script* script : scripts) {
			delete script;
		}
	};

	void Update(GameEntity& entity, float dt, float tt) {
		for (Script* script : scripts) {
			script->update(entity, dt, tt);
		}
	};

	void AddScript(Script* script) {
		scripts.push_back(script);
	};

	void RemoveScript(Script* script) {
		std::remove(scripts.begin(), scripts.end(), script);
	};
};

