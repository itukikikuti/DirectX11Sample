cbuffer Constant : register(b0)
{
    matrix world;
    matrix view;
    matrix projection;
	float3 lightColor;
	float lightIntensity;
	float3 lightDirection;
};
Texture2D texture0 : register(t0);
SamplerState sampler0 : register(s0);
struct VSOutput
{
	float4 position : SV_POSITION;
	float3 normal : NORMAL;
	float2 uv : TEXCOORD;
};
VSOutput VS(float4 position : POSITION, float3 normal : NORMAL, float2 uv : TEXCOORD)
{
	VSOutput output;
    output.position = mul(position, world);
    output.position = mul(output.position, view);
	output.position = mul(output.position, projection);
	output.normal = mul(normal, (float3x3)world);
	output.uv = uv;
    return output;
}
float4 PS(VSOutput pixel) : SV_TARGET
{
	float3 normal = normalize(pixel.normal);

	float3 diffuseIntensity = dot(-lightDirection, normal) * lightColor * lightIntensity;
	float3 ambientIntensity = lightColor * 0.2;

	float4 diffuseColor = texture0.Sample(sampler0, pixel.uv);

	return diffuseColor * float4(diffuseIntensity + ambientIntensity, 1);
}
