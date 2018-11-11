// Shader.cpp
#include <memory>
#include <vector>
#include "Graphics.hpp"
#include "Shader.hpp"

Shader Shader::GetDefault()
{
    static std::unique_ptr<Shader> shader(new Shader(
        "cbuffer Object : register(b0)"
        "{"
        "    matrix modelMatrix;"
        "};"
        "cbuffer Camera : register(b1)"
        "{"
        "    matrix viewMatrix;"
        "    matrix projectionMatrix;"
        "};"
        "struct Vertex"
        "{"
        "    float4 position : POSITION;"
        "    float3 color : COLOR;"
        "};"
        "struct Pixel"
        "{"
        "    float4 position : SV_POSITION;"
        "    float3 color : COLOR;"
        "};"
        "Pixel VS(Vertex vertex)"
        "{"
        "    Pixel output;"
        "    output.position = mul(vertex.position, modelMatrix);"
        "    output.position = mul(output.position, viewMatrix);"
        "    output.position = mul(output.position, projectionMatrix);"
        "    output.color = vertex.color;"
        "    return output;"
        "}"
        "float4 PS(Pixel pixel) : SV_TARGET"
        "{"
        "    return float4(pixel.color, 1);"
        "}"
    ));

    return *shader.get();
}

void Shader::Compile(const std::string& source, const char* entryPoint, const char* shaderModel, ID3DBlob** out)
{
    Microsoft::WRL::ComPtr<ID3DBlob> error = nullptr;
    D3DCompile(source.c_str(), source.length(), nullptr, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, entryPoint, shaderModel, D3DCOMPILE_ENABLE_STRICTNESS, 0, out, error.GetAddressOf());

    if (error != nullptr)
    {
        OutputDebugStringA((char*)error->GetBufferPointer());
        MessageBoxA(nullptr, (char*)error->GetBufferPointer(), "Shader error", MB_ICONERROR | MB_OK);
    }
}

Shader::Shader(const std::string& source)
{
    Microsoft::WRL::ComPtr<ID3DBlob> vertexShaderBlob = nullptr;
    Compile(source, "VS", "vs_5_0", vertexShaderBlob.GetAddressOf());

    Graphics::GetDevice().CreateVertexShader(vertexShaderBlob->GetBufferPointer(), vertexShaderBlob->GetBufferSize(), nullptr, vertexShader.GetAddressOf());

    Microsoft::WRL::ComPtr<ID3DBlob> pixelShaderBlob = nullptr;
    Compile(source, "PS", "ps_5_0", pixelShaderBlob.GetAddressOf());

    Graphics::GetDevice().CreatePixelShader(pixelShaderBlob->GetBufferPointer(), pixelShaderBlob->GetBufferSize(), nullptr, pixelShader.GetAddressOf());

    std::vector<D3D11_INPUT_ELEMENT_DESC> inputElementDesc
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };

    Graphics::GetDevice().CreateInputLayout(inputElementDesc.data(), (UINT)inputElementDesc.size(), vertexShaderBlob->GetBufferPointer(), vertexShaderBlob->GetBufferSize(), inputLayout.GetAddressOf());
}

void Shader::Attach()
{
    Graphics::GetContext().VSSetShader(vertexShader.Get(), nullptr, 0);
    Graphics::GetContext().PSSetShader(pixelShader.Get(), nullptr, 0);
    Graphics::GetContext().IASetInputLayout(inputLayout.Get());
}
