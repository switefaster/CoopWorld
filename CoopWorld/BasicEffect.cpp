#include "BasicEffect.h"

std::vector<D3D11_INPUT_ELEMENT_DESC> BasicEffect::InputLayout =
{
    {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
    {"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
    {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0}
};

BasicEffect::BasicEffect( ID3D11Device* device ) :
    Effect( device, L"basic.hlsl", nullptr, InputLayout )
{
    D3D11_BUFFER_DESC cbDesc;
    ZeroMemory( &cbDesc, sizeof( cbDesc ) );
    cbDesc.Usage = D3D11_USAGE_DEFAULT;
    cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    cbDesc.CPUAccessFlags = 0;
    cbDesc.MiscFlags = 0;
    cbDesc.StructureByteStride = 0;
    cbDesc.ByteWidth = D3DUtilities::AlignConstantBufferByte( sizeof( CBObject ) );
    ThrowIfFailed( device->CreateBuffer( &cbDesc, nullptr, &mConstantBuffers[0] ) );
    cbDesc.ByteWidth = D3DUtilities::AlignConstantBufferByte( sizeof( CBFrame ) );
    ThrowIfFailed( device->CreateBuffer( &cbDesc, nullptr, &mConstantBuffers[1] ) );
}

void BasicEffect::SetWorld( FXMMATRIX M )
{
    XMStoreFloat4x4( &mCBObject.first.World, M );
    SetDirty( mCBObject, true );
}

void BasicEffect::SetWorldInvTranspose( FXMMATRIX M )
{
    XMStoreFloat4x4( &mCBObject.first.WorldInvTranspose, M );
    SetDirty( mCBObject, true );
}

void BasicEffect::SetWorldViewProj( FXMMATRIX M )
{
    XMStoreFloat4x4( &mCBObject.first.WorldViewProj, M );
    SetDirty( mCBObject, true );
}

void BasicEffect::SetTexTransform( FXMMATRIX M )
{
    XMStoreFloat4x4( &mCBObject.first.TexTransform, M );
    SetDirty( mCBObject, true );
}

void BasicEffect::SetMaterial( const Material& mat )
{
    mCBObject.first.Material = mat;
    SetDirty( mCBObject, true );
}

void BasicEffect::SetLights( DirectionalLight lights[3], UINT count )
{
    memcpy( mCBFrame.first.DirectionalLights, lights, sizeof( DirectionalLight ) * count );
    mCBFrame.first.LightCount.x = count;
    SetDirty( mCBFrame, true );
}

void BasicEffect::SetShadowTransform( FXMMATRIX M )
{
    XMStoreFloat4x4( &mCBFrame.first.ShadowTransform, M );
}

void BasicEffect::SetEyePos( const XMFLOAT3& pos )
{
    mCBFrame.first.EyePos = pos;
    SetDirty( mCBFrame, true );
}

void BasicEffect::SetFogRange( float range )
{
    mCBFrame.first.FogRange = range;
    SetDirty( mCBFrame, true );
}

void BasicEffect::SetFogStart( float start )
{
    mCBFrame.first.FogStart = start;
    SetDirty( mCBFrame, true );
}

void BasicEffect::SetFogColor( const XMFLOAT3& color )
{
    mCBFrame.first.FogColor = color;
    SetDirty( mCBFrame, true );
}

void BasicEffect::SetShadowMap( ID3D11ShaderResourceView* srv )
{
    mShadowMap = srv;
}

void BasicEffect::SetShaderResourceView( ID3D11ShaderResourceView* srv )
{
    mTextureView = srv;
}

void BasicEffect::UpdateConstantBuffers( ID3D11DeviceContext* context )
{
    if ( mCBObject.second )
    {
        context->UpdateSubresource( mConstantBuffers[0].Get(), 0, nullptr, &mCBObject.first, 0, 0 );
        SetDirty( mCBObject, false );
    }

    if ( mCBFrame.second )
    {
        context->UpdateSubresource( mConstantBuffers[1].Get(), 0, nullptr, &mCBFrame.first, 0, 0 );
        SetDirty( mCBFrame, false );
    }

    context->PSSetShaderResources( 0, 1, &mTextureView );
    context->PSSetShaderResources( 1, 1, &mShadowMap );
}
