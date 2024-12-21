cbuffer ConstBuffer
{
	float4x4 VP;
	float T;
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
    
    // Billboard rotation (cancel out W rotation)
	float3x3 billBRot = (float3x3) VP;
	
    // Apply billboard rotation to the vertex position
	float4 worldPos = float4(mul(input.Pos.xyz, billBRot), 1);
	worldPos.xyz += input.InstancePosition;
	
    // Apply world position and project to screen space
	output.Pos = mul(worldPos, VP);
    
    // Transform normal and tangent to billboard space
	output.Normal = normalize(mul(input.Normal, billBRot));
	output.Tangent = normalize(mul(input.Tangent, billBRot));
    
	output.TexCoords = input.TexCoords;
    
	return output;
}
