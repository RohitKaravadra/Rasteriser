
Texture2D tex : register(t0);
Texture2D nor : register(t1);
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
    float3 Normal : NORMAL;
    float3 Tangent : TANGENT;
    float2 TexCoords : TEXCOORD;
};
struct PS_OUTPUT
{
    float4 Albedo : SV_Target0;
    float4 Normal : SV_Target1;
    float4 Tangent : SV_Target2;
};

float4 Pixel(PS_INPUT input) : SV_Target0
{
    PS_OUTPUT output;
    output.Albedo = tex.Sample(samplerLinear, input.TexCoords);
    
    if (output.Albedo.a < 0.5f)
        discard;
    
    float3 normal = normalize(input.Normal);
    float3 tangent = normalize(input.Tangent);
    float3 binormal = normalize(cross(input.Normal, tangent));
    float3x3 TBN = float3x3(tangent, binormal, normal);

    float3 mapNormal = nor.Sample(samplerLinear, input.TexCoords).xyz;
    
    mapNormal = mapNormal * 2.0 - 1.0;
    
    output.Normal = float4(mapNormal, 1);
    output.Tangent = float4(tangent, 1);
    
    return output;
}
