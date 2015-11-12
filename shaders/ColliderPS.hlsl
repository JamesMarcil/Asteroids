// Defines the input to this pixel shader
// - Should match the output of our corresponding vertex shader
struct VertexToPixel
{
    float4 position		: SV_POSITION;
    float3 normal       : NORMAL;
    float3 worldPos     : TEXCOORD0;
    float2 uv           : TEXCOORD1;
};

int isColliding;

Texture2D loamTexture		: register(t0);
Texture2D rockTexture		: register(t1);

SamplerState trilinear      : register(s0);

// Entry point for this pixel shader
float4 main(VertexToPixel input) : SV_TARGET
{
    // Re-normalize interpolated normal
    input.normal = normalize(input.normal);

	float4 diffuseColor;

    // Sample the diffuse texture
	if (isColliding == 1) {
		diffuseColor = rockTexture.Sample(trilinear, input.uv);
	}
	else {
		diffuseColor = loamTexture.Sample(trilinear, input.uv);
	}

    // Combine lights
	return diffuseColor;
}
