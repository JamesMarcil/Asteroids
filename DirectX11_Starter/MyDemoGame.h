#pragma once

#include <DirectXMath.h>
#include "DirectXGameCore.h"
#include "SimpleShader.h"

#include "Mesh.h"
#include "GameEntity.h"
#include "ResourceManager.h"

#include <vector>

// Include run-time memory checking in debug builds, so 
// we can be notified of memory leaks
#if defined(DEBUG) || defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif

// --------------------------------------------------------
// Game class which extends the base DirectXGameCore class
// --------------------------------------------------------
class MyDemoGame : public DirectXGameCore
{
public:
	MyDemoGame(HINSTANCE hInstance);
	~MyDemoGame();

	bool Init() override;
	void UpdateScene(float deltaTime, float totalTime) override;
	void DrawScene(float deltaTime, float totalTime) override;

private:
    bool	 prevSpaceBar;
    unsigned currentEntity;

	ResourceManager*		 resourceManager;
    ID3D11SamplerState*		 samplerState;
	std::vector<GameEntity*> entities;
};
