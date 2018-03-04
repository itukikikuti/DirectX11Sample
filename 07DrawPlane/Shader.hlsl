cbuffer Constant : register(b0)
{
    matrix world;
    matrix view;
    matrix projection;
};
struct VSOutput
{
	float4 position : SV_POSITION;
	float3 color : COLOR;
};
VSOutput VS(float4 position : POSITION, float3 color : COLOR)
{
	VSOutput output;
    output.position = mul(position, world);
    output.position = mul(output.position, view);
	output.position = mul(output.position, projection);
	output.color = color;
    return output;
}
float4 PS(VSOutput pixel) : SV_TARGET
{
    return float4(pixel.color, 1);
}
