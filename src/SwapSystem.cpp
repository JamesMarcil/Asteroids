#include "SwapSystem.h"

// Managers
#include "EntityManager.h"
#include "ResourceManager.h"

// DirectX
#include <d3d11.h>
#include "DXMacros.h"

using namespace DirectX;

SwapSystem::~SwapSystem(void)
{
    /* Nothing to do. */
}

void SwapSystem::Update(EntityManager* pManager, float dt, float tt)
{
    ResourceManager* pResource = ResourceManager::Instance();
    IDXGISwapChain* pSwapChain = pResource->GetSwapChain();

    // Present the buffer
    HR(pSwapChain->Present(0,0));
}
