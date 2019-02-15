#include "ColorEffect.h"

std::vector<D3D11_INPUT_ELEMENT_DESC> ColorEffect::InputLayout =
{
    {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
    {"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
};

ColorEffect::ColorEffect( ID3D11Device* device ) :
    Effect( device, L"color.hlsl", nullptr, InputLayout )
{
    D3D11_BUFFER_DESC cbDesc;
    ZeroMemory( &cbDesc, sizeof( cbDesc ) );
    cbDesc.Usage = D3D11_USAGE_DEFAULT;
    cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    cbDesc.ByteWidth = D3DUtilities::AlignConstantBufferByte( sizeof( CBuffer ) );
    cbDesc.CPUAccessFlags = 0;
    cbDesc.MiscFlags = 0;
    cbDesc.StructureByteStride = 0;
    ThrowIfFailed( device->CreateBuffer( &cbDesc, nullptr, &mConstantBuffers[0] ) );
}

void ColorEffect::SetWorld( FXMMATRIX M )
{
    mWorldMatrix.WorldMatrix = M;
}

void ColorEffect::UpdateConstantBuffers( ID3D11DeviceContext* context )
{
    context->UpdateSubresource( mConstantBuffers[0].Get(), 0, nullptr, &mWorldMatrix, 0, 0 );
}
