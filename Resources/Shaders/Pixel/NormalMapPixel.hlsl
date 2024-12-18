
Texture2D tex : register(t0);
Texture2D nor : register(t1);
SamplerState samplerLinear : register(s0);

struct PS_INPUT
{
	float4 Pos : SV_POSITION;
	float3 Normal : NORMAL;
	float3 Tangent : TANGENT;
	float2 TexCoords : TEXCOORD;
};

struct PS_OUTPUT
{
	float4 Albedo : SV_Target0;
	float4 Normal : SV_Target1;
};

PS_OUTPUT Pixel(PS_INPUT input)
{
	PS_OUTPUT output;
	output.Albedo = tex.Sample(samplerLinear, input.TexCoords);
    
	if (output.Albedo.a < 0.5f)
		discard;
    
	float3 mapNormal = nor.Sample(samplerLinear, input.TexCoords).xyz * 2.0 - 1.0;
	
	float3 normal = normalize(input.Normal);
	float3 tangent = normalize(input.Tangent);
	
	float3 binormal = normalize(cross(normal, tangent));
	
	float3x3 TBN = float3x3(tangent, binormal, normal);

	float3 worldNormal = normalize(mul(TBN, mapNormal)); // world space normal conversion
    
	output.Normal = float4(worldNormal, 1);
    
	return output;
}
