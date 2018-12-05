#pragma once
#include <string>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <wrl.h>
#pragma comment(lib, "d3dcompiler.lib")

class Shader
{
public:
    Shader(const std::string& source);
    void Attach() const;

    static Shader& GetDefault();
    static void Compile(const std::string& source, const char* entryPoint, const char* shaderModel, ID3DBlob** out);

private:
    Microsoft::WRL::ComPtr<ID3D11VertexShader> vertexShader;
    Microsoft::WRL::ComPtr<ID3D11PixelShader> pixelShader;
    Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout;
};
