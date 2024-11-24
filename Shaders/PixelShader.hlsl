
// constant buffer declaration
cbuffer bufferName
{
    float time;
    float2 lights[4];
};

float dist(float2 screenspace_pos, float2 light)
{
    return (length(screenspace_pos - light) / (50.0 * abs(cos(time))));
}

// pixel shader input data
struct PixelIn
{
    float4 Pos : SV_POSITION;
    float3 Color : COLOR;
};

// pixel shader entry point
float4 Pixel(PixelIn input) : SV_Target0
{
    float3 accumulated = float3(0, 0, 0);
    
    accumulated += 1.0 / dist(input.Pos.xy, lights[0]);
    accumulated += 1.0 / dist(input.Pos.xy, lights[1]);
    accumulated += 1.0 / dist(input.Pos.xy, lights[2]);
    accumulated += 1.0 / dist(input.Pos.xy, lights[3]);
    
    accumulated *= input.Color;
    return float4(accumulated, 1.0);

};