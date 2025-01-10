
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
    output.Albedo = float4(0, 1, 0, 1);
    output.Normal = float4(input.Normal, 0);
    
    return output;
}
