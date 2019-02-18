#include "ShadowEffect.h"



ShadowEffect::ShadowEffect( ID3D11Device* device ) :
    Effect( device, L"shadow.hlsl", nullptr, BasicEffect::InputLayout )
{
    D3D11_BUFFER_DESC cbDesc;
    cbDesc.Usage = D3D11_USAGE_DEFAULT;
    cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    cbDesc.ByteWidth = D3DUtilities::AlignConstantBufferByte( sizeof( CBFrame ) );
    cbDesc.CPUAccessFlags = 0;
    cbDesc.MiscFlags = 0;
    cbDesc.StructureByteStride = 0;
    ThrowIfFailed( device->CreateBuffer( &cbDesc, nullptr, &mConstantBuffers[0] ) );
}

void ShadowEffect::UpdateConstantBuffers( ID3D11DeviceContext* context )
{
    context->UpdateSubresource( mConstantBuffers[0].Get(), 0, 0, &mCBFrame, 0, 0 );
}

void ShadowEffect::SetLightWVP( FXMMATRIX M )
{
    XMStoreFloat4x4( &mCBFrame.LightWVP, M );
}

void ShadowEffect::SetTexTransform( FXMMATRIX M )
{
    XMStoreFloat4x4( &mCBFrame.TexTransform, M );
}

void ShadowEffect::SetTexture( ID3D11ShaderResourceView* srv )
{
    mDiffuseMap = srv;
    mCBFrame.HasTexture = srv == nullptr ? 0 : 1;
}
