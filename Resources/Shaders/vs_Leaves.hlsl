cbuffer ConstBuffer
{
	float4x4 VP;
	float4x4 R;
	float2 HT;
	float2 padding;
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

float GetHeight()
{
	float period = 10.0f;
	float value = fmod(HT.y, period) / period;
	return value;
}

PS_INPUT Vertex(VS_INPUT input)
{
	PS_INPUT output;
	
	float3x3 rot = (float3x3) R;
	
	output.Pos = float4(mul(input.Pos.xyz, rot), 1);
	float offset = output.Pos.y;
	output.Pos.xyz += input.InstancePosition;
	
	// horizontal movement
	output.Pos.xz += sin(output.Pos.x + HT.y * 2);
    // vertical movement
	output.Pos.y = HT.x - fmod(output.Pos.y - offset + GetHeight() * HT.x, HT.x) + offset;
	
    // Apply world position and project to screen space
	output.Pos = mul(output.Pos, VP);
    
	
    // normal and tangent projection
	output.Normal = normalize(mul(input.Normal, rot));
	output.Tangent = normalize(mul(input.Tangent, rot));
    
	output.TexCoords = input.TexCoords;
    
	return output;
}
