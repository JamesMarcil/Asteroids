cbuffer PerFrame : register(b0)
{
	float time;
}

// Defines the input to this pixel shader
// - Should match the output of our corresponding vertex shader
struct VertexToPixel
{
	float4 position		: SV_POSITION;
	float2 uv           : TEXCOORD0;
};

// Textures and such
Texture2D pixels		: register(t0);
Texture2D mask			: register(t1);
Texture2D noise			: register(t3);
SamplerState trilinear	: register(s0);

// Entry point for this pixel shader
float4 main(VertexToPixel input) : SV_TARGET
{

	float2 Center = { 0.5, 0.5 }; ///center of the screen
	float BlurStart = 1.0f; /// blur offset
	int nsamples = 5;

	float4 color = float4(0, 0, 0, 0);
	float4 maskSample = mask.Sample(trilinear, input.uv);

	if (maskSample.g != 0)
	{
		float4 noiseSample = noise.Sample(trilinear, input.uv - float2(0, time));
		float4 normalSample = pixels.Sample(trilinear, input.uv);
		float4 distortedSample = pixels.Sample(trilinear, input.uv - float2(0, noiseSample.r / 10));
		return normalSample * (1-maskSample.g) + distortedSample * maskSample.g;
	}

	return pixels.Sample(trilinear, input.uv);
}