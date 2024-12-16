
Texture2D tex : register(t0);
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
    float4 Tangent : SV_Target2;
};

PS_OUTPUT Pixel(PS_INPUT input)
{
    PS_OUTPUT output;
    output.Albedo = tex.Sample(samplerLinear, input.TexCoords);
    
    if (output.Albedo.a < 0.5f)
        discard;
    
    output.Normal = float4(input.Normal, 0);
    output.Tangent = float4(input.Tangent, 1);
    
    return output;
}
