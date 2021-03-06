#include "Light.hlsl"

struct VertexToPixel
{
	float4 position		: SV_POSITION;
	float3 worldPos     : POSITION;
	float3 normal       : NORMAL;
	float2 uv           : TEXCOORD1;
};

cbuffer PerFrame : register(b0)
{
	float3 cameraPosition;
};

cbuffer Lights : register(b1)
{
	DirectionalLight directionalLights[MAX_LIGHTS];
	PointLight pointLights[MAX_LIGHTS];
	SpotLight spotLights[MAX_LIGHTS];
};

Texture2D shipTexture		: register(t0);
Texture2D shipSpecMap       : register(t1);
SamplerState trilinear      : register(s0);

struct PS_OUTPUT
{
	float4 color: SV_Target0;
	float4 mask: SV_Target1;
};

PS_OUTPUT main(VertexToPixel input)
{
	PS_OUTPUT output;

	input.uv = float2(input.uv.x, -input.uv.y);

	// Re-normalize interpolated normal
	input.normal = normalize(input.normal);

	// Sample the Specular Map.
	float specMapValue = shipSpecMap.Sample(trilinear, input.uv).r;

	// Initialize Lighting.
	float4 dirColor = float4(0, 0, 0, 1);
	float4 pointColor = float4(0, 0, 0, 1);
	float4 spotColor = float4(0, 0, 0, 1);

	// Calculate Directional Lighting.
	[unroll] for (int i = 0; i < MAX_LIGHTS; ++i)
	{
		dirColor += DirectionalLightCalculation(directionalLights[i], input.worldPos, input.normal, cameraPosition, specMapValue);
	}

	// Calculate Point Lighting.
	[unroll] for (int j = 0; j < MAX_LIGHTS; ++j)
	{
		pointColor += PointLightCalculation(pointLights[j], input.worldPos, input.normal, cameraPosition, specMapValue);
	}

	// Calculate Spot Lighting.
	[unroll] for (int k = 0; k < MAX_LIGHTS; ++k)
	{
		spotColor += SpotLightCalculation(spotLights[k], input.worldPos, input.normal, cameraPosition, specMapValue);
	}

	// Sample the diffuse texture.
	float4 diffuseColor = shipTexture.Sample(trilinear, input.uv);
	
	// Calculate the final color.
	output.color = diffuseColor *(dirColor + pointColor + spotColor);
	output.mask = float4(1, 0, 0, 0);

	return output;
}
