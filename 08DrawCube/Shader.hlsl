cbuffer Constant : register(b0)
{
    matrix _world;
    matrix _view;
    matrix _projection;
};
struct VSOutput
{
	float4 position : SV_POSITION;
	float3 normal : NORMAL;
};
VSOutput VS(float4 position : POSITION, float3 normal : NORMAL)
{
	VSOutput output;
    output.position = mul(_world, position);
    output.position = mul(_view, output.position);
	output.position = mul(_projection, output.position);
	output.normal = normal;
    return output;
}
float4 PS(VSOutput pixel) : SV_TARGET
{
    return float4(pixel.normal, 1);
}
