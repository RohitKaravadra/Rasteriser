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
    float3 localLightDir = mul(normalize(float3(-1, 0.2, -0.2)), transpose(TBN));
    
    color = (color / 3.1459) * float4(1, 1, 1, 1) * 5 * max(dot(localLightDir, mapNormal), 0) + color * 0.2; // calculate simple lighting
    return float4(color.rgb, 1.0);
}
