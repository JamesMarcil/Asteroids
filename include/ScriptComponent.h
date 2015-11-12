#ifndef SCRIPT_COMPONENT_H
#define SCRIPT_COMPONENT_H

// ECS
#include "Component.h"

// Scripting
#include "IScript.h"

// STD
#include <vector>

class ScriptComponent : public Component<ScriptComponent>
{
private:
	std::vector<IScript*> scripts;
	bool deleted = false;

public:
    ScriptComponent(void) = default;

	virtual ~ScriptComponent(void)
    {
		for (auto& script : scripts)
        {
			delete script;
		}
		scripts.clear();

		deleted = true;
	}

	void Update(GameEntity& entity, float dt, float tt)
    {
        // An update may cause this to be destroyed.
		if (deleted)
        {
            return;
        }

        for(std::size_t i = 0; !deleted && i < scripts.size(); ++i)
        {
            scripts[i]->Update(entity, dt, tt);
        }
	}

	void AddScript(IScript* script)
    {
		scripts.push_back(script);
	}

	void RemoveScript(IScript* script)
    {
        // C++ Erase-Remove Idiom
        scripts.erase
        (
            std::remove
            (
                scripts.begin(),
                scripts.end(),
                script
            ),
            scripts.end()
        );

	}
};

#endif
