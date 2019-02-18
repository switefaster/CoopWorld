#include "Effect.h"

std::unordered_map<std::vector<D3D11_INPUT_ELEMENT_DESC>, ComPtr<ID3D11InputLayout>, IL_VECTOR_HASH> Effect::InputLayoutCache{};

Effect::Effect( ID3D11Device* device, const std::wstring& fileName, const D3D_SHADER_MACRO* defines, const std::vector<D3D11_INPUT_ELEMENT_DESC>& inputLayout )
{
    assert( device != nullptr );
    ComPtr<ID3DBlob> vsByteCode;
    ComPtr<ID3DBlob> psByteCode;
    vsByteCode = D3DUtilities::CompileShader( fileName, defines, "VS", "vs_4_0" );
    psByteCode = D3DUtilities::CompileShader( fileName, defines, "PS", "ps_4_0" );
    ThrowIfFailed( device->CreateVertexShader( vsByteCode->GetBufferPointer(), vsByteCode->GetBufferSize(), nullptr, &mVertexShader ) );
    ThrowIfFailed( device->CreatePixelShader( psByteCode->GetBufferPointer(), psByteCode->GetBufferSize(), nullptr, &mPixelShader ) );
	if (InputLayoutCache.count(inputLayout) <= 0) {
		ThrowIfFailed(device->CreateInputLayout(inputLayout.data(), inputLayout.size(), vsByteCode->GetBufferPointer(), vsByteCode->GetBufferSize(), &InputLayoutCache[inputLayout]));
	}
	mInputLayout = InputLayoutCache[inputLayout].Get();
}

void Effect::Apply( ID3D11DeviceContext* context )
{
    context->VSSetShader( mVertexShader.Get(), nullptr, 0 );
    context->PSSetShader( mPixelShader.Get(), nullptr, 0 );
    context->IASetInputLayout( mInputLayout );
    UpdateConstantBuffers( context );

    for ( auto&[slot, buffer] : mConstantBuffers )
    {
        context->VSSetConstantBuffers( slot, 1, buffer.GetAddressOf() );
        context->PSSetConstantBuffers( slot, 1, buffer.GetAddressOf() );
    }
}

ID3D11VertexShader* Effect::GetVS()
{
    return mVertexShader.Get();
}

ID3D11PixelShader* Effect::GetPS()
{
    return mPixelShader.Get();
}

void Effect::UpdateConstantBuffers( ID3D11DeviceContext* context )
{
}
