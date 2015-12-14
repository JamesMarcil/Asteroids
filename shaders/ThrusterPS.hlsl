#include "Light.hlsl"

struct VertexToPixel
{
	float4 position		: SV_POSITION;
	float3 worldPos     : POSITION;
	float3 normal       : NORMAL;
	float2 uv           : TEXCOORD1;
};
float4 main(VertexToPixel input) : SV_TARGET0
{

	//output.color = float4(0, 0, 1, 0);// *(dirColor + pointColor + spotColor);
	//output.mask = float4(0, 1, 0, 0);
	return float4(0, 1, 0, 0);
}
