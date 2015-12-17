struct VStoGS
{
	float4 startColor	: COLOR0;
	float4 midColor		: COLOR1;
	float4 endColor		: COLOR2;
	float3 position		: POSITION;
	float3 velocity		: TEXCOORD0;
	float3 acceleration	: TEXCOORD1;
	float  startSize	: TEXCOORD2;
	float  midSize		: TEXCOORD3;
	float  endSize		: TEXCOORD4;
	float  age			: TEXCOORD5;
	int    type			: TEXCOORD6;
};

VStoGS main(VStoGS input)
{
	return input;
}