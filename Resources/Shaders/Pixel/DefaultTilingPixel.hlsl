Texture2D tex : register(t0);
SamplerState samplerLinear : register(s0);

cbuffer ConstBuffer
{
    float3 Dir; // light direction
    float Amb; // ambient light
    float Int; // light intensity
    float2 T; // time
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
    float4 color = tex.Sample(samplerLinear, input.TexCoords * T);
    if (color.a < 0.5f)
        discard;
    
    color = color * saturate(dot(Dir, input.Normal)) * Int + color * Amb; // calculate simple lighting
    return float4(color.rgb, 1.0);
}
