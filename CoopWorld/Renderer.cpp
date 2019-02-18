#include "Renderer.h"



Renderer::Renderer( ID3D11Device* device, ID3D11DeviceContext* context, EffectManager* fxMan, StateManager* stateMan ) :
    mD3DDeviceContext( context ),
    mFXMan( fxMan ),
    mStateMan( stateMan ),
    mShadowMap( std::make_unique<ShadowMap>( device, 2048, 2048 ) )
{
}

void Renderer::Prepare( const Scene& scene, const Camera& camera )
{
    BuildShadowTransform( scene );
    mFXMan->BasicFX->SetFogColor( scene.FogColor );
    mFXMan->BasicFX->SetFogRange( scene.FogRange );
    mFXMan->BasicFX->SetFogStart( scene.FogStart );
    mFXMan->BasicFX->SetLights( ( DirectionalLight* )scene.Lights, scene.LightCount.x );
    mFXMan->BasicFX->SetEyePos( camera.GetPosition() );
    mFXMan->BasicFX->SetShadowTransform( XMMatrixTranspose( XMLoadFloat4x4( &mShadowTransform ) ) );
    mFXMan->BasicFX->SetShadowMap( mShadowMap->DepthMapSRV() );
    mShadowMap->BindDsvAndNullRtv( mD3DDeviceContext );
    mD3DDeviceContext->PSSetSamplers( 0, 1, mStateMan->LinearSampler.GetAddressOf() );
    ID3D11SamplerState* samplers[] = { nullptr };
    mD3DDeviceContext->PSSetSamplers( 1, 1, samplers );
    mD3DDeviceContext->RSSetState( mStateMan->SlopeScaledBiasRasterizer.Get() );
}

void Renderer::Draw( RenderItem* itemToDraw )
{
    mFXMan->ShadowFX->SetLightWVP( XMMatrixMultiplyTranspose( itemToDraw->BuildWorldMatrix(), XMLoadFloat4x4( &mLightVP ) ) );
    mFXMan->ShadowFX->SetTexTransform( XMMatrixTranspose( itemToDraw->GetTexTransform() ) );

    if ( itemToDraw->GetMesh()->HasTexture() != 0 )
    {
        mFXMan->ShadowFX->SetTexture( itemToDraw->GetMesh()->GetTexture()->GetShaderResourceView() );
    }

    mFXMan->ShadowFX->Apply( mD3DDeviceContext );
    itemToDraw->GetMesh()->Apply( mD3DDeviceContext );
    mRenderItemList.push_back( itemToDraw );
}

void Renderer::Post( const Camera& camera )
{
    mD3DDeviceContext->PSSetSamplers( 0, 1, mStateMan->LinearSampler.GetAddressOf() );
    mD3DDeviceContext->PSSetSamplers( 1, 1, mStateMan->ComparisonSampler.GetAddressOf() );
    mD3DDeviceContext->RSSetState( nullptr );

    for ( auto& itemToDraw : mRenderItemList )
    {
        XMMATRIX world = itemToDraw->BuildWorldMatrix();
        XMMATRIX viewProj = camera.ViewProj();
        XMMATRIX wInvTrans = D3DUtilities::InvTranspose( world );
        mFXMan->BasicFX->SetWorld( XMMatrixTranspose( world ) );
        mFXMan->BasicFX->SetWorldInvTranspose( XMMatrixTranspose( wInvTrans ) );
        mFXMan->BasicFX->SetWorldViewProj( XMMatrixMultiplyTranspose( world, viewProj ) );
        mFXMan->BasicFX->SetTexTransform( XMMatrixTranspose( itemToDraw->GetTexTransform() ) );
        mFXMan->BasicFX->SetMaterial( itemToDraw->GetMesh()->GetMaterial() );

        if ( itemToDraw->GetMesh()->HasTexture() != 0 )
        {
            mFXMan->BasicFX->SetShaderResourceView( itemToDraw->GetMesh()->GetTexture()->GetShaderResourceView() );
        }

        mFXMan->BasicFX->Apply( mD3DDeviceContext );
        itemToDraw->GetMesh()->Apply( mD3DDeviceContext );
    }

    mRenderItemList.clear();
}

void Renderer::BuildShadowTransform( const Scene& scene )
{
    if ( scene.LightCount.x > 0 )
    {
        XMVECTOR lightDir = XMLoadFloat3( &scene.Lights[0].Direction );
        XMVECTOR lightPos = -2.0f * scene.Bounds.Radius * lightDir;
        XMVECTOR targetPos = XMLoadFloat3( &scene.Bounds.Center );
        XMVECTOR up = XMVectorSet( 0.0f, 1.0f, 0.0f, 0.0f );
        XMMATRIX V = XMMatrixLookAtLH( lightPos, targetPos, up );
        XMFLOAT3 sphereCenterLS;
        XMStoreFloat3( &sphereCenterLS, XMVector3TransformCoord( targetPos, V ) );
        float l = sphereCenterLS.x - scene.Bounds.Radius;
        float b = sphereCenterLS.y - scene.Bounds.Radius;
        float n = sphereCenterLS.z - scene.Bounds.Radius;
        float r = sphereCenterLS.x + scene.Bounds.Radius;
        float t = sphereCenterLS.y + scene.Bounds.Radius;
        float f = sphereCenterLS.z + scene.Bounds.Radius;
        XMMATRIX P = XMMatrixOrthographicOffCenterLH( l, r, b, t, n, f );
        XMMATRIX T(
            0.5f, 0.0f, 0.0f, 0.0f,
            0.0f, -0.5f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.5f, 0.5f, 0.0f, 1.0f );
        XMMATRIX VP = V * P;
        XMMATRIX S = VP * T;
        XMStoreFloat4x4( &mLightVP, VP );
        XMStoreFloat4x4( &mShadowTransform, S );
        return;
    }

    mLightVP = mShadowTransform = Identity4x4;
}

StateManager::StateManager( ID3D11Device* device )
{
    D3D11_SAMPLER_DESC sampDesc;
    ZeroMemory( &sampDesc, sizeof( sampDesc ) );
    sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    ThrowIfFailed( device->CreateSamplerState( &sampDesc, &LinearSampler ) );
    sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
    sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
    sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
    sampDesc.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_LINEAR_MIP_POINT;
    sampDesc.BorderColor[0] = 1.0f;
    sampDesc.BorderColor[1] = 1.0f;
    sampDesc.BorderColor[2] = 1.0f;
    sampDesc.BorderColor[3] = 1.0f;
    sampDesc.ComparisonFunc = D3D11_COMPARISON_LESS_EQUAL;
    ThrowIfFailed( device->CreateSamplerState( &sampDesc, &ComparisonSampler ) );
    D3D11_RASTERIZER_DESC rastDesc;
    ZeroMemory( &rastDesc, sizeof( rastDesc ) );
    rastDesc.FillMode = D3D11_FILL_SOLID;
    rastDesc.CullMode = D3D11_CULL_BACK;
    rastDesc.SlopeScaledDepthBias = 10;
    rastDesc.DepthBiasClamp = 0.0f;
    rastDesc.DepthBias = 1.0f;
    rastDesc.DepthClipEnable = true;
    ThrowIfFailed( device->CreateRasterizerState( &rastDesc, &SlopeScaledBiasRasterizer ) );
}
