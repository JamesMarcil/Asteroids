#include "RenderSystem.h"

// Managers
#include "ResourceManager.h"
#include "EntityManager.h"
#include "CameraManager.h"

// DirectX
#include <d3d11.h>
#include <DirectXMath.h>
#include "DXMacros.h"
#include "SimpleShader.h"
#include "Vertex.h"

// Components
#include "RenderComponent.h"
#include "TransformComponent.h"

using namespace DirectX;

void RenderSystem::Update(EntityManager* pManager, float dt, float tt )
{
    ResourceManager* pResource = ResourceManager::instance();
    ID3D11Device* pDevice = pResource->GetDevice();
    ID3D11DeviceContext* pDeviceContext = pResource->GetDeviceContext();
    ID3D11RenderTargetView* pRender = pResource->GetRenderTargetView();
    ID3D11DepthStencilView* pDepthStencil = pResource->GetDepthStencilView();
    IDXGISwapChain* pSwapChain = pResource->GetSwapChain();

    // Background color (Cornflower Blue in this case) for clearing
    const float color[4] = {0.4f, 0.6f, 0.75f, 0.0f};

    // Clear the render target and depth buffer (erases what's on the screen)
    pDeviceContext->ClearRenderTargetView( pRender, color );
    pDeviceContext->ClearDepthStencilView( pDepthStencil, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0 );
    pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    // Update the Shaders
    ISimpleShader   *pVertexShader = pResource->GetShader( "StandardVertex" ),
                    *pPixelShader = pResource->GetShader( "StandardPixel" );
    Camera* pCamera = CameraManager::instance()->GetActiveCamera();
    XMFLOAT3 camPos = pCamera->transform.GetTranslation();

    // Update the Vertex Shader
    pVertexShader->SetMatrix4x4("view", pCamera->GetViewMatrix());
    pVertexShader->SetMatrix4x4("projection", pCamera->GetProjectionMatrix());

    // Update the Pixel Shader
    pPixelShader->SetFloat3("DirLightDirection", XMFLOAT3(1, 1, 1));
    pPixelShader->SetFloat4("DirLightColor", XMFLOAT4(0.3f, 0.3f, 0.3f, 1));
    pPixelShader->SetFloat3("PointLightPosition", XMFLOAT3(3, 3, -3));
    pPixelShader->SetFloat4("PointLightColor", XMFLOAT4(1, 1, 1, 1));
    pPixelShader->SetFloat3("CameraPosition", XMFLOAT3(camPos.x, camPos.y, camPos.z));
    pPixelShader->SetFloat("time", tt);

    // Render every GameEntity.
    for(auto& entity : pManager->EntitiesWithComponents<RenderComponent, TransformComponent>())
    {
        RenderComponent* pRender = pManager->GetComponent<RenderComponent>(entity);
        TransformComponent* pTransform = pManager->GetComponent<TransformComponent>(entity);

        Renderer& r = pRender->renderer;
        Transform& t = pTransform->transform;

        r.Draw(t);
    }

    // Present the buffer
    HR(pSwapChain->Present(0, 0));
}
