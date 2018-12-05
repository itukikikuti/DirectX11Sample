#include <memory>
#include <vector>
#include "Graphics.hpp"
#include "Shader.hpp"

Shader::Shader(const std::string& source)
{
    Microsoft::WRL::ComPtr<ID3DBlob> vertexShaderBlob = nullptr;
    Compile(source, "VSMain", "vs_5_0", vertexShaderBlob.GetAddressOf());

    Graphics::GetDevice().CreateVertexShader(vertexShaderBlob->GetBufferPointer(), vertexShaderBlob->GetBufferSize(), nullptr, vertexShader.GetAddressOf());

    Microsoft::WRL::ComPtr<ID3DBlob> pixelShaderBlob = nullptr;
    Compile(source, "PSMain", "ps_5_0", pixelShaderBlob.GetAddressOf());

    Graphics::GetDevice().CreatePixelShader(pixelShaderBlob->GetBufferPointer(), pixelShaderBlob->GetBufferSize(), nullptr, pixelShader.GetAddressOf());

    std::vector<D3D11_INPUT_ELEMENT_DESC> inputElementDesc
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };

    Graphics::GetDevice().CreateInputLayout(inputElementDesc.data(), (UINT)inputElementDesc.size(), vertexShaderBlob->GetBufferPointer(), vertexShaderBlob->GetBufferSize(), inputLayout.GetAddressOf());
}

void Shader::Attach() const
{
    Graphics::GetContext().VSSetShader(vertexShader.Get(), nullptr, 0);
    Graphics::GetContext().PSSetShader(pixelShader.Get(), nullptr, 0);
    Graphics::GetContext().IASetInputLayout(inputLayout.Get());
}

Shader& Shader::GetDefault()
{
    static std::unique_ptr<Shader> shader = std::make_unique<Shader>(R"(
        struct Vertex
        {
            float4 position : POSITION;
        };

        struct Pixel
        {
            float4 position : SV_POSITION;
        };

        Pixel VSMain(Vertex vertex)
        {
            Pixel output;
            output.position = vertex.position;
            return output;
        }

        float4 PSMain(Pixel pixel) : SV_TARGET
        {
            return float4(0, 0, 0, 1);
        }
    )");

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
