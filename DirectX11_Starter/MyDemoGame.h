#ifndef MY_DEMO_GAME_H
#define MY_DEMO_GAME_H

// DirectX
#include "DirectXGameCore.h"

// Game
#include "GameEntity.h"

// STD
#include <vector>

class MyDemoGame : public DirectXGameCore
{
private:
    bool	 prevSpaceBar;
    unsigned currentEntity;

	std::vector<GameEntity> entities;

public:
	MyDemoGame(HINSTANCE hInstance);
	~MyDemoGame();

	bool Init() override;
	void UpdateScene(float deltaTime, float totalTime) override;
	void DrawScene(float deltaTime, float totalTime) override;
};

#endif
