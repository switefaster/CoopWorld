#include "DemoApp.h"

using namespace DirectX;

DemoApp::DemoApp( HINSTANCE hInstance ) :
    D3DApplication( hInstance )
{
}

DemoApp::~DemoApp()
{
}

bool DemoApp::Initialize()
{
    if ( !D3DApplication::Initialize() )
    {
        return false;
    }

    mEffectManager.Initialize( mD3DDevice.Get() );
    std::vector<ColorEffect::Vertex> vertices =
    {
        {XMFLOAT3( 0.5f, 1.0f, 0.5f ), XMFLOAT4( Colors::Aqua )},
        {XMFLOAT3( 1.0f, 0.0f, 0.5f ), XMFLOAT4( Colors::Aqua )},
        {XMFLOAT3( 0.0f, 0.0f, 0.5f ), XMFLOAT4( Colors::Aqua )}
    };
    std::vector<std::uint16_t> indices = { 0, 1, 2 };
    mTriangleMesh = std::make_unique<Mesh<ColorEffect::Vertex>>( mD3DDevice.Get(), vertices.data(), indices.data(), vertices.size(), indices.size() );
    return true;
}

void DemoApp::OnResize()
{
    D3DApplication::OnResize();
}

void DemoApp::Update( const Timer& dt )
{
    if ( D3DUtilities::IsKeyDown( VK_DOWN ) )
    {
        mEffectManager.ColorFX->SetWorld( XMMatrixTranspose( XMMatrixTranslation( 0.0f, 0.5f, 0.0f ) ) );
    }
}

void DemoApp::Draw( const Timer& dt )
{
    assert( mD3DContext != nullptr );
    assert( mSwapChain != nullptr );
    mD3DContext->ClearRenderTargetView( mRenderTargetView.Get(), reinterpret_cast<const float*>( &Colors::Black ) );
    mD3DContext->ClearDepthStencilView( mDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0 );
    mEffectManager.ColorFX->Apply( mD3DContext.Get() );
    mTriangleMesh->Apply( mD3DContext.Get() );
    ThrowIfFailed( mSwapChain->Present( 0, 0 ) );
}
