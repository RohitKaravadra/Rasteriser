Texture2D tex : register(t0);
SamplerState samplerLinear : register(s0);

cbuffer ConstBuffer
{
    float2 T;
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
};

PS_OUTPUT Pixel(PS_INPUT input)
{
    PS_OUTPUT output;
    output.Albedo = tex.Sample(samplerLinear, input.TexCoords * T);
    
    if (output.Albedo.a < 0.5f)
        discard;
    
    output.Normal = float4(input.Normal, 1);
    
    return output;
}
