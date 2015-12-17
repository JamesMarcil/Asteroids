#include "PostEffectsSystem.h"

// Managers
#include "EntityManager.h"
#include "ResourceManager.h"
#include "CameraManager.h"
#include "EventManager.h"

// Components
#include "RenderComponent.h"
#include "TransformComponent.h"

// Effects
#include "Warp.h"
#include "Refraction.h"

// DirectX
#include <d3d11.h>
#include "DXMacros.h"

using namespace DirectX;

PostEffectsSystem::PostEffectsSystem(void)
{
    m_pResource = ResourceManager::Instance();
    m_pEvent = EventManager::Instance();

    CreatePostEffectRTVsAndSRVs();

	Warp* w = new Warp();
	effectsOrdered.emplace_back(w);
	effectsMap.emplace("Warp", w);

	Refraction* r = new Refraction();
	effectsOrdered.emplace_back(r);
	effectsMap.emplace("Refraction",r);
	r->Enabled(true);

	noEffect = new NoEffect();

    m_pEvent->Register("WarpBegin", this);
    m_pEvent->Register("OnResize", this);
}

PostEffectsSystem::~PostEffectsSystem(void)
{
	delete noEffect;
    for(auto& effect : effectsOrdered)
    {
        if(effect)
        {
            delete effect;
        }
    }
	effectsOrdered.clear();
	effectsMap.clear();
}

/*
* Update Loop for the the effects system
* @param   pManager           The Game's Resource Manager
* @param   dt                 The time since last frame
* @param   tt                 Total Time Elapsed
*/
void PostEffectsSystem::Update(EntityManager * pManager, float dt, float tt)
{
    // Grab the device and context.
	ID3D11Device* pDevice =                 m_pResource->GetDevice();
	ID3D11DeviceContext* pDeviceContext =   m_pResource->GetDeviceContext();

    // Grab the main RTV and DSV.
    ID3D11RenderTargetView* mainRTV = m_pResource->GetRenderTargetView("MainRTV");
	ID3D11DepthStencilView* dsv = m_pResource->GetDepthStencilView();

    // Grab the Post-Process RTVs and SRVs.
	ID3D11RenderTargetView* postRTV = m_pResource->GetRenderTargetView("PostRTV");
	ID3D11ShaderResourceView* ppSRV =   m_pResource->GetTexture("PostEffectTexture");

	ID3D11RenderTargetView* swapRTV = m_pResource->GetRenderTargetView("PostSwapRTV");
	ID3D11ShaderResourceView* swapSRV = m_pResource->GetTexture("PostEffectSwapTexture");

	ID3D11RenderTargetView* maskRTV = m_pResource->GetRenderTargetView("MaskRTV");
	ID3D11ShaderResourceView* maskSRV = m_pResource->GetTexture("MaskTexture");

	// Unbind the buffers and set up the "triangle" to draw.
	ID3D11Buffer* nothing = 0;
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	pDeviceContext->IASetVertexBuffers(0, 1, &nothing, &stride, &offset);
	pDeviceContext->IASetIndexBuffer(0, DXGI_FORMAT_R32_UINT, 0);


	bool alt = false;
	for (auto& effect : effectsOrdered)
	{
		if (effect->Enabled())
		{
			alt = !alt;
			if (alt)
			{
				//use the next render target
				pDeviceContext->OMSetRenderTargets(1, &swapRTV, 0);

				//sample the last texture
				effect->RenderEffect(ppSRV, pDeviceContext, dt, tt);
			}
			else
			{
				pDeviceContext->OMSetRenderTargets(1, &postRTV, 0);
				effect->RenderEffect(swapSRV, pDeviceContext, dt, tt);
			}
		}
	}

	// switch back to rendering to the back buffer
	pDeviceContext->OMSetRenderTargets(1, &mainRTV, 0);

	if (alt)
	{
		noEffect->RenderEffect(swapSRV, pDeviceContext, dt, tt);
	}
	else
	{
		noEffect->RenderEffect(ppSRV, pDeviceContext, dt, tt);
	}
}

void PostEffectsSystem::EventRouter(const std::string & name, void * data)
{
	if (name == "WarpBegin")
	{
		effectsMap["Warp"]->Enabled(true);
	}
    else if(name == "OnResize")
    {
        CreatePostEffectRTVsAndSRVs();
    }
}

void PostEffectsSystem::CreatePostEffectRTVsAndSRVs(void)
{
    ID3D11Device* pDevice = m_pResource->GetDevice();

	ID3D11Texture2D* ppTexture;
	ID3D11RenderTargetView* ppRTV;
	ID3D11ShaderResourceView* ppSRV;

	ID3D11Texture2D* swapTexture;
	ID3D11RenderTargetView* swapRTV;
	ID3D11ShaderResourceView* swapSRV;

	ID3D11Texture2D* maskTexture;
	ID3D11RenderTargetView* maskRTV;
	ID3D11ShaderResourceView* maskSRV;

    // Destroy previous RTVs.
    {
        ppRTV = m_pResource->GetRenderTargetView("PostRTV");
        swapRTV = m_pResource->GetRenderTargetView("PostSwapRTV");
        maskRTV = m_pResource->GetRenderTargetView("MaskRTV");

        ReleaseMacro(ppRTV);
        ReleaseMacro(swapRTV);
        ReleaseMacro(maskRTV);
    }

    // Destroy previoue Post-Process Textures.
    {
        ppSRV =     m_pResource->GetTexture("PostEffectTexture");
        swapSRV =   m_pResource->GetTexture("PostEffectSwapTexture");
        maskSRV =   m_pResource->GetTexture("MaskTexture");

        ReleaseMacro(ppSRV);
        ReleaseMacro(swapSRV);
        ReleaseMacro(maskSRV);
    }

	// Create the Post-Process Textures.
    {
        D3D11_TEXTURE2D_DESC desc;
        ZeroMemory(&desc, sizeof(desc));
        desc.Width =  m_pResource->GetWindowWidth();
        desc.Height = m_pResource->GetWindowHeight();
        desc.ArraySize = 1;
        desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
        desc.CPUAccessFlags = 0;
        desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        desc.MipLevels = 1;
        desc.MiscFlags = 0;
        desc.SampleDesc.Count = 1;
        desc.SampleDesc.Quality = 0;
        desc.Usage = D3D11_USAGE_DEFAULT;

        // create the texture that we will render into
        pDevice->CreateTexture2D(&desc, 0, &ppTexture);
        pDevice->CreateTexture2D(&desc, 0, &swapTexture);
        pDevice->CreateTexture2D(&desc, 0, &maskTexture);
    }

	// Create the RTVs for the scene.
    {
        D3D11_RENDER_TARGET_VIEW_DESC desc;
        ZeroMemory(&desc, sizeof(desc));
        desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        desc.Texture2D.MipSlice = 0;
        desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

        pDevice->CreateRenderTargetView(ppTexture, &desc, &ppRTV);
        pDevice->CreateRenderTargetView(swapTexture, &desc, &swapRTV);
        pDevice->CreateRenderTargetView(maskTexture, &desc, &maskRTV);
    }

	// Create the SRVs for the scene.
    {
        D3D11_SHADER_RESOURCE_VIEW_DESC desc;
        ZeroMemory(&desc, sizeof(desc));
        desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        desc.Texture2D.MipLevels = 1;
        desc.Texture2D.MostDetailedMip = 0;
        desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;

        pDevice->CreateShaderResourceView(ppTexture, &desc, &ppSRV);
        pDevice->CreateShaderResourceView(swapTexture, &desc, &swapSRV);
        pDevice->CreateShaderResourceView(maskTexture, &desc, &maskSRV);
    }

	// Register the Post-Process SRVs.
	m_pResource->RegisterShaderResourceView("PostEffectTexture", ppSRV);
	m_pResource->RegisterShaderResourceView("PostEffectSwapTexture", swapSRV);
	m_pResource->RegisterShaderResourceView("MaskTexture", maskSRV);

	// Register the Post-Process RTVs.
	m_pResource->RegisterRenderTargetView("PostRTV", ppRTV);
	m_pResource->RegisterRenderTargetView("PostSwapRTV", swapRTV);
	m_pResource->RegisterRenderTargetView("MaskRTV", maskRTV);

	// We can release the textures because the RTV and SRV now handle it.
    ReleaseMacro(ppTexture);
    ReleaseMacro(swapTexture);
    ReleaseMacro(maskTexture);
}
