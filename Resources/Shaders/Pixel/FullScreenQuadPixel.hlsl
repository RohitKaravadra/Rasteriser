
Texture2D alb : register(t0);
Texture2D nor : register(t1);
Texture2D tan : register(t2);
Texture2D zbuf : register(t3);

SamplerState samplerLinear : register(s0);

cbuffer ConstBuffer
{
	float3 Dir; // light direction
	float Amb; // ambient light
	float Int; // light intensity
};

struct PS_INPUT
{
	float4 Pos : SV_POSITION;
	float2 TexCoords : TEXCOORD;
};

float4 Pixel(PS_INPUT input) : SV_Target0
{
	float4 color = alb.Sample(samplerLinear, input.TexCoords);
	float4 normal = nor.Sample(samplerLinear, input.TexCoords);
	float4 tangent = tan.Sample(samplerLinear, input.TexCoords);
	float depth = zbuf.Sample(samplerLinear, input.TexCoords);
	
	if (normal.a < 0.2) // check if pixel is not lit
		return float4(color.rgb, 1.0);
    
	float3 binormal = normalize(cross(normal.xyz, tangent.xyz));
	float3x3 TBN = float3x3(tangent.xyz, binormal, normal.xyz);
    
	float3 localLightDir = mul(float3(-1, -1, -1), transpose(TBN));
    
	color = color * saturate(dot(localLightDir, normal.xyz)) * 5 + color * 0.3; // calculate simple lighting
	
	return float4(depth, depth, depth, 1);
}
