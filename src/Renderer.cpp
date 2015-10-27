#include "Renderer.h"

Renderer::Renderer(Mesh * mesh, Material * mat)
{
	this->deviceContext = nullptr;
	this->mesh = mesh;
	this->material = mat;
}

Renderer::~Renderer()
{

}

void Renderer::Draw(Transform& t)
{
	ResourceManager* pManager = ResourceManager::Instance();
	if(this->deviceContext == nullptr)
		this->deviceContext = pManager->GetDeviceContext();

	SimpleVertexShader* vert = (SimpleVertexShader*)material->GetVertexShader();
	vert->SetMatrix4x4("world", t.GetWorldMatrix());

	SimplePixelShader* pixel = (SimplePixelShader*)material->GetPixelShader();
	pixel->SetSamplerState("trilinear", pManager->GetSamplerState("trilinear"));

	material->WriteShaderInfo();

	vert->SetShader(true);
	pixel->SetShader(true);

	ID3D11Buffer* vb = mesh->GetVertexBuffer();
	ID3D11Buffer* ib = mesh->GetIndexBuffer();
	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	// Set the Vertex and Index buffers
	deviceContext->IASetVertexBuffers(0, 1, &vb, &stride, &offset);
	deviceContext->IASetIndexBuffer(ib, DXGI_FORMAT_R32_UINT, 0);

	// Draw the GameEntity
	deviceContext->DrawIndexed(mesh->GetIndexCount(), 0, 0);
}
