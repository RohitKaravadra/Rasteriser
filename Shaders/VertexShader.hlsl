
// vertex shader input data
struct VertexIn
{
	float4 Pos : POS;
	float3 Color : COLOR;
};

// vertex shader output data
struct PixelIn
{
	float4 Pos : SV_POSITION;
	float3 Color : COLOR;
};

// vertex shader entry point
PixelIn Vertex(VertexIn input)
{
	PixelIn output;
	output.Pos = input.Pos;
	output.Color = input.Color;
	return output;
};