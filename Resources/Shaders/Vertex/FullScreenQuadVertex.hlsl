struct PS_INPUT
{
    float4 Pos : SV_POSITION;
    float2 TexCoords : TEXCOORD;
};

PS_INPUT Vertex(uint vertexID : SV_VertexId)
{
    PS_INPUT output;
    output.TexCoords = float2((vertexID << 1) & 2, vertexID & 2);
    output.Pos = float4((output.TexCoords.x * 2.0f) - 1.0f, (-output.TexCoords.y * 2.0f) + 1.0f, 0, 1.0f);
    return output;
}
