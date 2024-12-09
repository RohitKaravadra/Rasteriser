
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

float4 Pixel(PS_INPUT input) : SV_Target0
{
    float4 color = tex.Sample(samplerLinear, input.TexCoords);
    if (color.a < 0.5f)
        discard;
    
    float3 normal = normalize(input.Normal);
    float3 tangent = normalize(input.Tangent);
    float3 binormal = normalize(cross(input.Normal, tangent));
    float3x3 TBN = float3x3(tangent, binormal, normal);

    float3 mapNormal = nor.Sample(samplerLinear, input.TexCoords).xyz;
    
    mapNormal = mapNormal * 2.0 - 1.0;
    
    float3 localLightDir = mul(Dir, transpose(TBN));
    
    color = color * saturate(dot(localLightDir, mapNormal)) * Int + color * Amb; // calculate simple lighting
    
    return float4(color.rgb, 1.0);
}
