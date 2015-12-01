#include "Warp.h"

#include "ResourceManager.h"

Warp::Warp()
{
	ppVS = ResourceManager::Instance()->GetShader("PPVS");
	ppPS = ResourceManager::Instance()->GetShader("PPWarpBlur");
}

Warp::~Warp(void)
{
}

void Warp::RenderEffect(ID3D11ShaderResourceView * srv, ID3D11DeviceContext* pDeviceContext)
{
	if (true)
	{
		ID3D11SamplerState* sampler = ResourceManager::Instance()->GetSamplerState("trilinear");
		ppVS->SetShader();

		ppPS->SetShaderResourceView("pixels", srv);
		ppPS->SetSamplerState("trilinear", sampler);
		ppPS->SetShader();

		// Draw a specific number of vertices (without buffers)
		pDeviceContext->Draw(3, 0);

		// Unbind shader resource view so we can render into the
		// texture at the beginning of next frame
		ppPS->SetShaderResourceView("pixels", 0);
	}
}
