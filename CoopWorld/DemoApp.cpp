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

    std::vector<BasicEffect::Vertex> vertices =
    {
        {XMFLOAT3( 0.5f, 1.0f, 0.5f ), XMFLOAT3( 0.0f, 0.0f, -1.0f ), XMFLOAT2( 0.0f, 0.0f )},
        {XMFLOAT3( 1.0f, 0.0f, 0.5f ), XMFLOAT3( 0.0f, 0.0f, -1.0f ), XMFLOAT2( 0.0f, 0.0f )},
        {XMFLOAT3( 0.0f, 0.0f, 0.5f ), XMFLOAT3( 0.0f, 0.0f, -1.0f ), XMFLOAT2( 0.0f, 0.0f )}
    };
    std::vector<std::uint16_t> indices = { 0, 1, 2 };
    mTriangleMesh = std::make_unique<Mesh>( mD3DDevice.Get(), vertices.data(), indices.data(), vertices.size(), indices.size() );
    mTriangleMesh->SetMaterial(
    {
        {1.0f, 1.0f, 1.0f, 0.0f},
        {1.0f, 1.0f, 1.0f, 1.0f},
        {0.8f, 0.8f, 0.8f, 16.0f},
        {0.0f, 0.0f, 0.0f, 0.0f}
    } );
    mEffectManager = std::make_unique<EffectManager>( mD3DDevice.Get() );
    mRenderer = std::make_unique<Renderer>( mD3DContext.Get(), mEffectManager.get() );
    mRenderItem = std::make_unique<RenderItem>( mTriangleMesh.get() );
    mRenderItem->Offset( 0.0f, 0.0f, 10.0f );
    mScene.LightCount.x = 0;
    return true;
}

void DemoApp::OnResize()
{
    D3DApplication::OnResize();
    mCamera.SetLens( 0.25f * XM_PI, AspectRatio(), 1.0f, 1000.0f );
}

void DemoApp::Update( const Timer& dt )
{
    if ( D3DUtilities::IsKeyDown( 'S' ) )
    {
        mCamera.Walk( dt.Delta() * -1.3 );
    }

    if ( D3DUtilities::IsKeyDown( 'W' ) )
    {
        mCamera.Walk( dt.Delta() * 1.3 );
    }

    if ( D3DUtilities::IsKeyDown( 'D' ) )
    {
        mCamera.Strafe( dt.Delta() * -1.3 );
    }

    if ( D3DUtilities::IsKeyDown( 'A' ) )
    {
        mCamera.Strafe( dt.Delta() * 1.3 );
    }

    if ( D3DUtilities::IsKeyDown( VK_LEFT ) )
    {
        mRotY = dt.Delta() * -0.13;
        mCamera.RotateY( mRotY );
    }

    if ( D3DUtilities::IsKeyDown( VK_RIGHT ) )
    {
        mRotY = dt.Delta() * 0.13;
        mCamera.RotateY( mRotY );
    }

    if ( D3DUtilities::IsKeyDown( VK_DOWN ) )
    {
        mPitch = dt.Delta() * -0.13;
        mCamera.Pitch( mPitch );
    }

    if ( D3DUtilities::IsKeyDown( VK_UP ) )
    {
        mPitch = dt.Delta() * 0.13;
        mCamera.Pitch( mPitch );
    }

    mCamera.UpdateViewMatrix();
}

void DemoApp::Draw( const Timer& dt )
{
    assert( mD3DContext != nullptr );
    assert( mSwapChain != nullptr );
    mD3DContext->ClearRenderTargetView( mRenderTargetView.Get(), reinterpret_cast<const float*>( &Colors::Black ) );
    mD3DContext->ClearDepthStencilView( mDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0 );
    mRenderer->Draw( mRenderItem.get(), mScene, mCamera );
    mRenderer->Post();
    ThrowIfFailed( mSwapChain->Present( 0, 0 ) );
}
