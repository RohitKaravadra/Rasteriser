cbuffer ConstBuffer
{
    float4x4 W;
    float4x4 VP;
    float T;
	float3 padding;
};

struct VS_INPUT
{
    float4 Pos : POS;
    float3 Normal : NORMAL;
    float3 Tangent : TANGENT;
    float2 TexCoords : TEXCOORD;
    float3 InstancePosition : INSTANCEPOSITION;
};

struct PS_INPUT
{
    float4 Pos : SV_POSITION;
    float3 Normal : NORMAL;
    float3 Tangent : TANGENT;
    float2 TexCoords : TEXCOORD;
};

PS_INPUT Vertex(VS_INPUT input)
{
    PS_INPUT output;
    
     // animate by changing position
    input.Pos.y += sin(input.Pos.x + T * 3);
    output.Pos.x += sin(input.Pos.y + T * 2);
    
    // position projection
    output.Pos = mul(input.Pos, W);
    output.Pos = output.Pos + float4(input.InstancePosition, 0);
    output.Pos = mul(output.Pos, VP);
    
    // normal and tangent projection
    output.Normal = normalize(mul(input.Normal, (float3x3) W));
    output.Tangent = normalize(mul(input.Tangent, (float3x3) W));
    
    output.TexCoords = input.TexCoords;
    
    return output;
}
