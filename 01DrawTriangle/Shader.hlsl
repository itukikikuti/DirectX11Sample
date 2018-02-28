cbuffer Constant : register(b0)
{
    matrix _world;
    matrix _view;
    matrix _projection;
};
struct VSOutput
{
	float4 position : SV_POSITION;
	float3 color : COLOR;
};
VSOutput VS(float4 position : POSITION, float3 color : COLOR)
{
	VSOutput output;
    output.position = mul(_world, position);
    output.position = mul(_view, output.position);
	output.position = mul(_projection, output.position);
	output.color = color;
    return output;
}
float4 PS(VSOutput pixel) : SV_TARGET
{
    return float4(pixel.color, 1);
}
