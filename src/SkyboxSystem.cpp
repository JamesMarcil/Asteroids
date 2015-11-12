#include "SkyboxSystem.h"

// Managers
#include "EntityManager.h"
#include "ResourceManager.h"
#include "CameraManager.h"
#include "EventManager.h"
#include "InputManager.h"

// DirectX
#include <d3d11.h>
#include "DXMacros.h"
#include "SimpleShader.h"
#include "Vertex.h"
#include "Mesh.h"

using namespace DirectX;

SkyboxSystem::SkyboxSystem()
{
	EventManager::Instance()->Register("WarpBegin", this);
	textureNames[0] = "GreenSpace";
	textureNames[1] = "PurpleSpace";
	textureNames[2] = "Space";
	warping = false;
	timeElapsed = 0;
	currentTexture = 0;
}

SkyboxSystem::~SkyboxSystem(void)
{
    /* Nothing to do. */
}

void SkyboxSystem::EventRouter(const std::string& name, void* data)
{
	if (name == "WarpBegin")
	{
		currentTexture++;
		currentTexture = currentTexture > 2 ? 0 : currentTexture;
		warping = true;
		timeElapsed = 0;
	}
}

void SkyboxSystem::Update(EntityManager* pManager, float dt, float tt)
{
	if (InputManager::Instance()->IsKeyDown('L') && !warping) EventManager::Instance()->Fire("WarpBegin", nullptr);

	int lastTexture = currentTexture - 1 < 0 ? 2 : currentTexture - 1;

	if (warping)
	{
		timeElapsed += dt;
		if (timeElapsed > warpTime)
		{
			warping = false;
			lerpT = 1;
		}
		else
		{
			lerpT = timeElapsed / warpTime;
		}
	}

    ResourceManager* pResource = ResourceManager::Instance();
    ID3D11DeviceContext* pDeviceContext = pResource->GetDeviceContext();
    ID3D11RasterizerState* pRasterizer = pResource->GetRasterizerState("Skybox_Rasterizer");
    ID3D11DepthStencilState* pDepthStencil = pResource->GetDepthStencilState("Skybox_DepthStencil");
	ID3D11ShaderResourceView* pToSkySRV = pResource->GetTexture(textureNames[currentTexture]);
	ID3D11ShaderResourceView* pFromSkySRV = pResource->GetTexture(textureNames[lastTexture]);
    ISimpleShader   *pSkyVertex = pResource->GetShader("SkyboxVertex"),
                    *pSkyPixel = pResource->GetShader("SkyboxPixel");
    Camera* pCamera = CameraManager::Instance()->GetActiveCamera();

    // Update Mesh data.
    Mesh* pCube = pResource->GetMesh("Cube");
    ID3D11Buffer* vb = pCube->GetVertexBuffer();
    ID3D11Buffer* ib = pCube->GetIndexBuffer();
    UINT stride = sizeof(Vertex);
    UINT offset = 0;
    pDeviceContext->IASetVertexBuffers(0, 1, &vb, &stride, &offset);
    pDeviceContext->IASetIndexBuffer(ib, DXGI_FORMAT_R32_UINT, 0);

    // Send the matrices.
    pSkyVertex->SetMatrix4x4("view", pCamera->GetViewMatrix());
    pSkyVertex->SetMatrix4x4("projection", pCamera->GetProjectionMatrix());
    pSkyVertex->CopyBufferData("PerFrame");

    // Send the texure and sampler.
	pSkyPixel->SetShaderResourceView("skybox", pToSkySRV);
	pSkyPixel->SetShaderResourceView("fromSkybox", pFromSkySRV);
	pSkyPixel->SetFloat("t", lerpT);
    pSkyPixel->SetSamplerState("trilinear", pResource->GetSamplerState("trilinear"));
	pSkyPixel->CopyBufferData("PerFrame");

    // Set Shader without copying buffers.
    pSkyVertex->SetShader(false);
    pSkyPixel->SetShader(false);

    // Set Rasterizer and DepthStencil.
    pDeviceContext->RSSetState(pRasterizer);
    pDeviceContext->OMSetDepthStencilState(pDepthStencil, 0);

    // Render the Skybox.
    pDeviceContext->DrawIndexed(pCube->GetIndexCount(), 0, 0);

    // Reset Rasterizer and DepthStencil.
    pDeviceContext->RSSetState(nullptr);
    pDeviceContext->OMSetDepthStencilState(nullptr, 0);
}
