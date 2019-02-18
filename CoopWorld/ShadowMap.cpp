#include "ShadowMap.h"



ShadowMap::ShadowMap( ID3D11Device* device, UINT width, UINT height ) :
    mWidth( width ),
    mHeight( height )
{
    mViewport.TopLeftX = 0.0f;
    mViewport.TopLeftY = 0.0f;
    mViewport.Width = static_cast<float>( mWidth );
    mViewport.Height = static_cast<float>( mHeight );
    mViewport.MinDepth = 0.0f;
    mViewport.MaxDepth = 1.0f;
    D3D11_TEXTURE2D_DESC texDesc;
    texDesc.Width = mWidth;
    texDesc.Height = mHeight;
    texDesc.MipLevels = 1;
    texDesc.ArraySize = 1;
    texDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
    texDesc.SampleDesc.Count = 1;
    texDesc.SampleDesc.Quality = 0;
    texDesc.Usage = D3D11_USAGE_DEFAULT;
    texDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
    texDesc.CPUAccessFlags = 0;
    texDesc.MiscFlags = 0;
    ComPtr<ID3D11Texture2D> depthMap;
    ThrowIfFailed( device->CreateTexture2D( &texDesc, nullptr, &depthMap ) );
    D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
    dsvDesc.Flags = 0;
    dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    dsvDesc.Texture2D.MipSlice = 0;
    ThrowIfFailed( device->CreateDepthStencilView( depthMap.Get(), &dsvDesc, &mDepthDsv ) );
    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
    srvDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MipLevels = texDesc.MipLevels;
    srvDesc.Texture2D.MostDetailedMip = 0;
    ThrowIfFailed( device->CreateShaderResourceView( depthMap.Get(), &srvDesc, &mDepthSrv ) );
}

ID3D11ShaderResourceView* ShadowMap::DepthMapSRV()
{
    return mDepthSrv.Get();
}

void ShadowMap::BindDsvAndNullRtv( ID3D11DeviceContext* context )
{
    context->RSSetViewports( 1, &mViewport );
    ID3D11RenderTargetView* targets[] = { nullptr };
    context->OMSetRenderTargets( 1, targets, mDepthDsv.Get() );
	context->ClearDepthStencilView(mDepthDsv.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
}
