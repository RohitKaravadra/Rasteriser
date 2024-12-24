Texture2D alb  : register(t0);
Texture2D nor  : register(t1);
Texture2D zbuf : register(t2);
Texture2D lMap : register(t3); // Shadow map from light's perspective

SamplerState linearSampler : register(s0); // Wrap
SamplerState shadowSampler : register(s1); // Clamp

cbuffer ConstBuffer
{
	float4x4 IV; // Camera view inverse
	float4x4 IP; // Camera projection inverse
};

cbuffer LightBuffer
{
	float4x4 LV; // Light view matrix
	float4x4 LP; // Light projection matrix
	float2   AmbInt; // ambient and Intensity
	float2   padding;
};

struct PS_INPUT
{
	float4 Pos : SV_POSITION;
	float2 TexCoords : TEXCOORD;
};

float4 Pixel(PS_INPUT input) : SV_Target0
{
    // Sample G-buffer
	float4 albedo = alb.Sample(linearSampler, input.TexCoords);
	float4 normal = normalize(nor.Sample(linearSampler, input.TexCoords));
	float depth   = zbuf.Sample(shadowSampler, input.TexCoords).r;

	if (normal.a < 0.2 || depth >= 1)
		return float4(albedo.rgb, 1.0);

    // Reconstruct world position from depth buffer
	float4 clipSpacePos = float4(input.TexCoords.x * 2  - 1,1 - input.TexCoords.y * 2, depth, 1.0);
	float4 viewSpacePos = mul(clipSpacePos, IP);
	viewSpacePos        = viewSpacePos / viewSpacePos.w;
	float4 worldPos     = mul(viewSpacePos, IV);

    // Project world position to light space
	float4 lightSpacePos = mul(worldPos, LV);
	lightSpacePos        = mul(lightSpacePos,LP);
	float3 lightClip     = lightSpacePos.xyz / lightSpacePos.w * 0.5 + 0.5;

    // sample light space depth from shadow map
	float shadowDepth = lMap.Sample(shadowSampler, lightClip.xy).r;

    // compare depths if no depth in shadow map then no shadow
	float shadow = lightClip.z >= 1 ? 1 :lightClip.z > shadowDepth ? 0.2 : 1.0;
	
    // Calculate lighting
	float3 lightDir = normalize(LV._13_23_33);
	float3 diffuse  = max(dot(normal.xyz, lightDir), 0.0) * AmbInt.y * albedo.rgb;
	float3 ambient  = albedo.rgb * AmbInt.x;

	float3 resultColor = (diffuse + ambient) * shadow;
    
	return float4(resultColor, 1.0);
}
