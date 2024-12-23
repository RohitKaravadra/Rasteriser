Texture2D alb : register(t0);
Texture2D nor : register(t1);
Texture2D zbuf : register(t2);
Texture2D lMap : register(t3); // Shadow map from light's perspective

SamplerState samplerLinear : register(s0);

cbuffer ConstBuffer
{
	float2 AmbInt;
	float2 padding;
};

cbuffer MatrixBuffer
{
	float4x4 IV; // Camera view inverse
	float4x4 IP; // Camera projection inverse
	float4x4 LV; // Light view matrix
	float4x4 LP; // Light projection matrix
};

struct PS_INPUT
{
	float4 Pos : SV_POSITION;
	float2 TexCoords : TEXCOORD;
};

float4 Pixel(PS_INPUT input) : SV_Target0
{
    // Sample G-buffer
	float4 albedo = alb.Sample(samplerLinear, input.TexCoords);
	float4 normal = normalize(nor.Sample(samplerLinear, input.TexCoords));
	float depth = zbuf.Sample(samplerLinear, input.TexCoords);

	if (normal.a < 0.2 || depth >= 1)
		return float4(albedo.rgb, 1.0);

    // Reconstruct world position from depth buffer
	float4 clipSpacePos = float4(input.TexCoords * 2 - 1, depth, 1.0);
	float4 viewSpacePos = mul(IP, clipSpacePos);
	viewSpacePos        = viewSpacePos / viewSpacePos.w;
	float4 worldPos     = mul(IV, viewSpacePos);

    // Project world position to light space
	float4 lightSpacePos = mul(LV, worldPos);
	lightSpacePos        = mul(LP, lightSpacePos);
	float2 shadowUV      = lightSpacePos.xy / lightSpacePos.w * 0.5 + 0.5;

    // Sample the light-space depth from the light's perspective
	float shadowDepth  = lMap.Sample(samplerLinear, shadowUV).r;
	float currentDepth = lightSpacePos.z / lightSpacePos.w;

    // Shadow comparison with bias
	float shadow = currentDepth > shadowDepth + 0.005 ? 0.3 : 1.0;

    // Calculate lighting
	float3 lightDir = normalize(LV[2].xyz);
	float3 diffuse  = max(dot(normal.xyz, lightDir), 0.0) * AmbInt.y * albedo.rgb;
	float3 ambient  = albedo.rgb * AmbInt.x;

	float3 resultColor = (diffuse + ambient);// * shadow;
    
	return float4(resultColor, 1.0);
}
