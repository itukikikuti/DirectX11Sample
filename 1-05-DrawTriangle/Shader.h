#pragma once
#include "Library.h"

class Shader
{
public:
    Shader(const std::string& source);
    void Attach() const;

    static Shader& GetDefault();
    static void Compile(const std::string& source, const char* entryPoint, const char* shaderModel, ID3DBlob** out);

private:
    Microsoft::WRL::ComPtr<ID3D11VertexShader> vertexShader = nullptr;
    Microsoft::WRL::ComPtr<ID3D11PixelShader> pixelShader = nullptr;
    Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout = nullptr;
};
