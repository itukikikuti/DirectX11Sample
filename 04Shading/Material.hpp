class Material
{
public:
    Material()
    {
        App::Initialize();
    }
    Material(const std::string& source)
    {
        App::Initialize();
        Create(source);
    }
    ~Material()
    {
    }
    void Create(const std::string& source)
    {
        ATL::CComPtr<ID3DBlob> vertexShaderBlob = nullptr;
        CompileShader(source, "VS", "vs_5_0", &vertexShaderBlob);

        vertexShader.Release();
        App::GetGraphicsDevice().CreateVertexShader(vertexShaderBlob->GetBufferPointer(), vertexShaderBlob->GetBufferSize(), nullptr, &vertexShader);

        ATL::CComPtr<ID3DBlob> pixelShaderBlob = nullptr;
        CompileShader(source, "PS", "ps_5_0", &pixelShaderBlob);

        pixelShader.Release();
        App::GetGraphicsDevice().CreatePixelShader(pixelShaderBlob->GetBufferPointer(), pixelShaderBlob->GetBufferSize(), nullptr, &pixelShader);

        std::vector<D3D11_INPUT_ELEMENT_DESC> inputElementDesc
        {
            { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
        };

        inputLayout.Release();
        App::GetGraphicsDevice().CreateInputLayout(inputElementDesc.data(), (UINT)inputElementDesc.size(), vertexShaderBlob->GetBufferPointer(), vertexShaderBlob->GetBufferSize(), &inputLayout);
    }
    void SetBuffer(void* cbuffer, UINT size)
    {
        buffer = cbuffer;

        D3D11_BUFFER_DESC constantBufferDesc = {};
        constantBufferDesc.ByteWidth = size;
        constantBufferDesc.Usage = D3D11_USAGE_DEFAULT;
        constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

        constantBuffer.Release();
        App::GetGraphicsDevice().CreateBuffer(&constantBufferDesc, nullptr, &constantBuffer);
    }
    void Attach()
    {
        if (buffer != nullptr)
        {
            App::GetGraphicsContext().UpdateSubresource(constantBuffer, 0, nullptr, buffer, 0, 0);
            App::GetGraphicsContext().VSSetConstantBuffers(0, 1, &constantBuffer.p);
            App::GetGraphicsContext().PSSetConstantBuffers(0, 1, &constantBuffer.p);
        }

        if (vertexShader != nullptr)
            App::GetGraphicsContext().VSSetShader(vertexShader, nullptr, 0);

        if (pixelShader != nullptr)
            App::GetGraphicsContext().PSSetShader(pixelShader, nullptr, 0);

        if (inputLayout != nullptr)
            App::GetGraphicsContext().IASetInputLayout(inputLayout);
    }

private:
    void* buffer = nullptr;
    ATL::CComPtr<ID3D11Buffer> constantBuffer = nullptr;
    ATL::CComPtr<ID3D11VertexShader> vertexShader = nullptr;
    ATL::CComPtr<ID3D11PixelShader> pixelShader = nullptr;
    ATL::CComPtr<ID3D11InputLayout> inputLayout = nullptr;

    static void CompileShader(const std::string& source, const char* const entryPoint, const char* const shaderModel, ID3DBlob** out)
    {
        UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined(_DEBUG)
        flags |= D3DCOMPILE_DEBUG;
#endif

        ATL::CComPtr<ID3DBlob> error = nullptr;
        D3DCompile(source.c_str(), source.length(), nullptr, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, entryPoint, shaderModel, flags, 0, out, &error);

        if (error != nullptr)
        {
            OutputDebugStringA(static_cast<char*>(error->GetBufferPointer()));
            MessageBoxA(App::GetWindowHandle(), static_cast<char*>(error->GetBufferPointer()), "Shader Error", MB_OK);
        }
    }
};