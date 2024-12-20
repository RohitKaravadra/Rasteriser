
Texture2D tex : register(t0);
Texture2D nor : register(t2);
SamplerState samplerLinear : register(s0);

struct PS_INPUT
{
	float4 Pos : SV_POSITION;
	float3 Normal : NORMAL;
	float3 Tangent : TANGENT;
	float2 TexCoords : TEXCOORD;
};

float4 Pixel(PS_INPUT input) : SV_Target0
{
	discard;
	return float4(0, 0, 0, 0);
}
