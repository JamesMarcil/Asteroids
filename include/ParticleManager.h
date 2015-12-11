#ifndef PARTICLE_MANAGER
#define PARTICLE_MANAGER

#include "Singleton.h"
#include "ResourceManager.h"
#include "ParticleGenerator.h"
#include "CameraManager.h"
#include <vector>
#include <random>
#include <time.h>

class ParticleManager : public Singleton<ParticleManager>
{
private:
	std::vector<ParticleGenerator> generators;
	ID3D11Texture1D*			randomTexture;
	ID3D11ShaderResourceView*	randomSRV;
	SimpleGeometryShader*		generatorGS;

public:
	ParticleManager() {
		ResourceManager* rManager = ResourceManager::Instance();
		ID3D11Device* device = rManager->GetDevice();
		ID3D11DeviceContext* deviceContext = rManager->GetDeviceContext();

		generators = std::vector<ParticleGenerator>();

		rManager->RegisterShader<SimpleGeometryShader>("ParticleGeneratorGS", L"ParticleGeneratorGS.cso", new SimpleGeometryShader(device, deviceContext, true, false));

		// Random stuff
		unsigned randomTextureWidth = 1024;
		srand((unsigned)time(0));
		std::vector<float> data(randomTextureWidth * 4);
		for (unsigned int i = 0; i < randomTextureWidth * 4; i++) {
			data[i] = rand() / (float)RAND_MAX * 2.0f - 1.0f;
		}

		D3D11_TEXTURE1D_DESC textureDesc;
		textureDesc.ArraySize		= 1;
		textureDesc.BindFlags		= D3D11_BIND_SHADER_RESOURCE;
		textureDesc.CPUAccessFlags	= 0;
		textureDesc.Format			= DXGI_FORMAT_R32G32B32A32_FLOAT;
		textureDesc.MipLevels		= 1;
		textureDesc.MiscFlags		= 0;
		textureDesc.Usage			= D3D11_USAGE_DEFAULT;
		textureDesc.Width			= 100;

		D3D11_SUBRESOURCE_DATA randData;
		randData.pSysMem		  = (void*)&data[0];
		randData.SysMemPitch	  = randomTextureWidth * sizeof(float) * 4;
		randData.SysMemSlicePitch = 0;
		device->CreateTexture1D(&textureDesc, &randData, &randomTexture);

		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		srvDesc.Format = textureDesc.Format;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE1D;
		srvDesc.Texture1D.MipLevels = 1;
		srvDesc.Texture1D.MostDetailedMip = 0;
		device->CreateShaderResourceView(randomTexture, &srvDesc, &randomSRV);

		// Create particle blendstate
		D3D11_BLEND_DESC blendDesc;
		ZeroMemory(&blendDesc, sizeof(D3D11_BLEND_DESC));
		blendDesc.AlphaToCoverageEnable = false;
		blendDesc.IndependentBlendEnable = false;
		blendDesc.RenderTarget[0].BlendEnable = true;
		blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		D3D11_DEPTH_STENCIL_DESC depthDesc;
		ZeroMemory(&depthDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
		depthDesc.DepthEnable = true;
		depthDesc.DepthFunc = D3D11_COMPARISON_LESS;
		depthDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;

		rManager->RegisterTexture("randomTexture", randomSRV);
		rManager->RegisterDepthStencilState("particleDepthState", depthDesc);
		rManager->RegisterBlendState("particleBlendState", blendDesc);
	}

	~ParticleManager() {
		//randomTexture->Release();
		//randomSRV->Release();
	}

	void Update(float dt, float tt) {
		ResourceManager* rManager = ResourceManager::Instance();
		ID3D11Device* device = rManager->GetDevice();
		ID3D11DeviceContext* deviceContext = rManager->GetDeviceContext();
		Camera* activeCam = CameraManager::Instance()->GetActiveCamera();

		SimpleVertexShader* generatorVS = dynamic_cast<SimpleVertexShader*>(rManager->GetShader("ParticleGeneratorVS"));
		SimpleGeometryShader* generatorGS = dynamic_cast<SimpleGeometryShader*>(rManager->GetShader("ParticleGeneratorGS"));

		deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

		for (auto& pGen : generators) {
			pGen.Update(dt, tt);
		}
	}

	void AddGenerator(ParticleGenerator generator) 
	{ 
		generators.push_back(generator); 
	}
	std::vector<ParticleGenerator> GetGenerators() { return generators; }
	ID3D11Texture1D* GetRandomTexture() { return randomTexture; }
	ID3D11ShaderResourceView* GetRandomSRV() { return randomSRV; }
};

#endif