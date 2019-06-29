#include "DemoApp.h"

#include "MeshLoader.hpp"
#include <algorithm>

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

	mRenderer = std::make_unique<ShadowedRenderer>(mD3DDevice.Get(), mD3DContext.Get(), this);

    mSphereMesh = std::move( MeshLoader::LoadMesh( mD3DDevice.Get(), "resources/sphere.obj", mRenderer->GetTexMan() )[0] );
    mPlateMesh = std::move( MeshLoader::LoadMesh( mD3DDevice.Get(), "resources/plate.obj", mRenderer->GetTexMan() )[0] );
	mSphere = std::make_unique<RenderItem>(mSphereMesh.get());
	mGround = std::make_unique<RenderItem>(mPlateMesh.get());
	mScene = mRenderer->GetScene();
	mCamera = mRenderer->GetCamera();
    mScene->LightCount.x = 1;
    mScene->Lights[0].Ambient = XMFLOAT4( 0.2f, 0.2f, 0.2f, 1.0f );
    mScene->Lights[0].Diffuse = XMFLOAT4( 0.6f, 0.6f, 0.6f, 1.0f );
    mScene->Lights[0].Specular = XMFLOAT4( 0.5f, 0.5f, 0.5f, 1.0f );
    mScene->Lights[0].Direction = XMFLOAT3( 0.57735f, -0.57735f, 0.57735f );
    mScene->Bounds.Center = XMFLOAT3( 0.0f, 0.0f, 0.0f );
	mScene->Bounds.Radius = sqrt(10 * 10 + 10 * 10);
    return true;
}

void DemoApp::OnResize()
{
    D3DApplication::OnResize();
	if (mCamera)
	{
		mCamera->SetLens(0.25f * XM_PI, AspectRatio(), 1.0f, 1000.0f);
	}
}

void DemoApp::Update( const Timer& dt )
{
    if ( D3DUtilities::IsKeyDown( 'S' ) )
    {
        mCamera->Walk( dt.Delta() * -1.3 );
    }

    if ( D3DUtilities::IsKeyDown( 'W' ) )
    {
        mCamera->Walk( dt.Delta() * 1.3 );
    }

    if ( D3DUtilities::IsKeyDown( 'D' ) )
    {
        mCamera->Strafe( dt.Delta() * 1.3 );
    }

    if ( D3DUtilities::IsKeyDown( 'A' ) )
    {
        mCamera->Strafe( dt.Delta() * -1.3 );
    }

    if ( D3DUtilities::IsKeyDown( VK_LEFT ) )
    {
        mCamera->RotateY( dt.Delta() * -0.39 );
    }

    if ( D3DUtilities::IsKeyDown( VK_RIGHT ) )
    {
        mCamera->RotateY( dt.Delta() * 0.39 );
    }

    if ( D3DUtilities::IsKeyDown( VK_DOWN ) )
    {
        mCamera->Pitch( dt.Delta() * 0.39 );
    }

    if ( D3DUtilities::IsKeyDown( VK_UP ) )
    {
        mCamera->Pitch( dt.Delta() * -0.39 );
    }

    mCamera->UpdateViewMatrix();

	mRenderer->RenderThis(mSphere.get());
	mRenderer->RenderThis(mGround.get());
}

void DemoApp::Draw( const Timer& dt )
{
    assert( mD3DContext != nullptr );
    assert( mSwapChain != nullptr );
	Clear();
	mRenderer->StartDrawProcess();
	mRenderer->GetFontMan()->DrawFont(mD3DContext.Get(), L"Consolas", std::to_wstring(mFPS), 36.0f, 0.0f, 0.0f, 0XFFFFFFFF);
	mRenderer->GetFontMan()->DrawFont(mD3DContext.Get(), L"Consolas", std::to_wstring(mTPS), 36.0f, 300.0f, 0.0f, 0XFFFFFFFF);
    ThrowIfFailed( mSwapChain->Present( 0, 0 ) );
}
