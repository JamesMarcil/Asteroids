#include "PostEffectsSystem.h"

// Managers
#include "EntityManager.h"
#include "ResourceManager.h"

// DirectX
#include <d3d11.h>

using namespace DirectX;

PostEffectsSystem::~PostEffectsSystem(void)
{
}

// TODO: break this up so that we can apply multiple effects at will
void PostEffectsSystem::Update(EntityManager * pManager, float dt, float tt)
{
	ResourceManager* pResource = ResourceManager::Instance();
	ID3D11Device* pDevice = pResource->GetDevice();
	ID3D11DeviceContext* pDeviceContext = pResource->GetDeviceContext();
	ID3D11RenderTargetView* mainRTV = pResource->GetRenderTargetView("MainRTV");
	ID3D11DepthStencilView* dsv = pResource->GetDepthStencilView();

	// Grab the effect shaders
	ISimpleShader* ppVS = pResource->GetShader("PPVS");
	ISimpleShader* ppPS = pResource->GetShader("PPPS");
	
	// Grab the Texture we rendered into
	ID3D11ShaderResourceView* ppSRV = pResource->GetTexture("PostEffectTexture");

	ID3D11SamplerState* sampler = pResource->GetSamplerState("trilinear");

	// switch back to rendering to the back buffer
	pDeviceContext->OMSetRenderTargets(1, &mainRTV, dsv);

	ppVS->SetShader();

	// set the effect shader values
	ppPS->SetInt("blurAmount", 10);
	ppPS->SetFloat("pixelWidth", 1.0f / pResource->GetWindowWidth());
	ppPS->SetFloat("pixelHeight", 1.0f / pResource->GetWindowHeight());
	ppPS->SetShaderResourceView("pixels", ppSRV);
	ppPS->SetSamplerState("trilinear", sampler);
	ppPS->SetShader();

	// unbind the buffers and set up the "triangle" to draw onto
	ID3D11Buffer* nothing = 0;
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	pDeviceContext->IASetVertexBuffers(0, 1, &nothing, &stride, &offset);
	pDeviceContext->IASetIndexBuffer(0, DXGI_FORMAT_R32_UINT, 0);

	// Draw a specific number of vertices (without buffers)
	pDeviceContext->Draw(3, 0);

	// Unbind shader resource view so we can render into the
	// texture at the beginning of next frame
	ppPS->SetShaderResourceView("pixels", 0);
}
