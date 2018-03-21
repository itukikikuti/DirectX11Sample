cbuffer Constant : register(b0)
{
    matrix world;
    matrix view;
    matrix projection;
};
struct Vertex
{
    float4 position : POSITION;
    float3 color : COLOR;
};
struct Pixel
{
    float4 position : SV_POSITION;
    float3 color : COLOR;
};
Pixel VS(Vertex vertex)
{
    Pixel output;
    output.position = mul(vertex.position, world);
    output.position = mul(output.position, view);
    output.position = mul(output.position, projection);
    output.color = vertex.color;
    return output;
}
float4 PS(Pixel pixel) : SV_TARGET
{
    return float4(pixel.color, 1);
}
