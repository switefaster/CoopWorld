#include "DemoApp.h"

#include "MeshLoader.hpp"

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

    mStateManager = std::make_unique<StateManager>( mD3DDevice.Get() );
    mFontManager = std::make_unique<FontManager>( mD3DDevice.Get() );
    mTextureManager = std::make_unique<TextureManager>( mD3DDevice.Get() );
    mSphereMesh = std::move( MeshLoader::LoadMesh( mD3DDevice.Get(), "resources/sphere.obj", mTextureManager.get() )[0] );
    mPlateMesh = std::move( MeshLoader::LoadMesh( mD3DDevice.Get(), "resources/plate.obj", mTextureManager.get() )[0] );
    mEffectManager = std::make_unique<EffectManager>( mD3DDevice.Get() );
    mRenderer = std::make_unique<Renderer>( mD3DDevice.Get(), mD3DContext.Get(), mEffectManager.get(), mStateManager.get() );
    mRenderItem = std::make_unique<RenderItem>( mSphereMesh.get() );
    mPlateItem = std::make_unique<RenderItem>( mPlateMesh.get() );
    mRenderItem->Offset( 0.0f, 3.0f, 0.0f );
    mScene.LightCount.x = 1;
    mScene.Lights[0].Ambient = XMFLOAT4( 0.2f, 0.2f, 0.2f, 1.0f );
    mScene.Lights[0].Diffuse = XMFLOAT4( 0.6f, 0.6f, 0.6f, 1.0f );
    mScene.Lights[0].Specular = XMFLOAT4( 0.5f, 0.5f, 0.5f, 1.0f );
    mScene.Lights[0].Direction = XMFLOAT3( 0.57735f, -0.57735f, 0.57735f );
    mScene.Bounds.Center = XMFLOAT3( 0.0f, 0.0f, 0.0f );
    mScene.Bounds.Radius = sqrt( 10 * 10 + 10 * 10 );
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
        mCamera.Strafe( dt.Delta() * 1.3 );
    }

    if ( D3DUtilities::IsKeyDown( 'A' ) )
    {
        mCamera.Strafe( dt.Delta() * -1.3 );
    }

    if ( D3DUtilities::IsKeyDown( VK_LEFT ) )
    {
        mCamera.RotateY( dt.Delta() * -0.39 );
    }

    if ( D3DUtilities::IsKeyDown( VK_RIGHT ) )
    {
        mCamera.RotateY( dt.Delta() * 0.39 );
    }

    if ( D3DUtilities::IsKeyDown( VK_DOWN ) )
    {
        mCamera.Pitch( dt.Delta() * 0.39 );
    }

    if ( D3DUtilities::IsKeyDown( VK_UP ) )
    {
        mCamera.Pitch( dt.Delta() * -0.39 );
    }

    mCamera.UpdateViewMatrix();
}

void DemoApp::Draw( const Timer& dt )
{
    assert( mD3DContext != nullptr );
    assert( mSwapChain != nullptr );
    mD3DContext->ClearRenderTargetView( mRenderTargetView.Get(), reinterpret_cast<const float*>( &Colors::Black ) );
    mD3DContext->ClearDepthStencilView( mDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0 );
    mRenderer->Prepare( mScene, mCamera );
    mRenderer->Draw( mRenderItem.get() );
    mRenderer->Draw( mPlateItem.get() );
    mD3DContext->RSSetViewports( 1, &mViewport );
    mD3DContext->OMSetRenderTargets( 1, mRenderTargetView.GetAddressOf(), mDepthStencilView.Get() );
    mRenderer->Post( mCamera );
    mFontManager->DrawFont( mD3DContext.Get(), L"Arial", L"Hello World!", 128.0f, 0.0f, 0.0f, 0xFFFFFFFF );
    ThrowIfFailed( mSwapChain->Present( 0, 0 ) );
}
