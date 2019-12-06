#include "DemoApp.h"

#include "MeshLoader.hpp"
#include "DemoGuiScreen.h"
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

    mSphereMesh = std::move( MeshLoader::LoadMesh( mD3DDevice.Get(), "resources/sphere.obj", mRenderer->GetTextureManager() )[0] );
    mPlateMesh = std::move( MeshLoader::LoadMesh( mD3DDevice.Get(), "resources/terrain.obj", mRenderer->GetTextureManager() )[0] );
	mSphere = std::make_unique<RenderItem>(mSphereMesh.get());
	mGround = std::make_unique<RenderItem>(mPlateMesh.get());
	mScene = mRenderer->GetScene();
	mCamera = mRenderer->GetCamera();
	mCamera->SetLens(0.25f * XM_PI, AspectRatio(), 1.0f, 1000.0f);
	mSphere->Offset(0.0f, 5.0f, 0.0f);
    mScene->LightCount.x = 1;
    mScene->Lights[0].Ambient = XMFLOAT4( 0.2f, 0.2f, 0.2f, 1.0f );
    mScene->Lights[0].Diffuse = XMFLOAT4( 0.6f, 0.6f, 0.6f, 1.0f );
    mScene->Lights[0].Specular = XMFLOAT4( 0.5f, 0.5f, 0.5f, 1.0f );
    mScene->Lights[0].Direction = XMFLOAT3( 0.57735f, -0.57735f, 0.57735f );
    mScene->Bounds.Center = XMFLOAT3( 0.0f, 0.0f, 0.0f );
	mScene->Bounds.Radius = sqrt(10 * 10 + 10 * 10);

	mD2DRenderTarget->CreateSolidColorBrush(D2D1::ColorF(0.0f, 1.0f, 0.0f, 1.0f), &mBrush);
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
        mCamera->Walk( dt.Delta() * -0.52 );
    }

    if ( D3DUtilities::IsKeyDown( 'W' ) )
    {
        mCamera->Walk( dt.Delta() * 0.52 );
    }

    if ( D3DUtilities::IsKeyDown( 'D' ) )
    {
        mCamera->Strafe( dt.Delta() * 0.52 );
    }

    if ( D3DUtilities::IsKeyDown( 'A' ) )
    {
        mCamera->Strafe( dt.Delta() * -0.52 );
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
	bool currGKeyState;
	if (currGKeyState = D3DUtilities::IsKeyDown('G'))
	{
		if (!mPrevGKeyState)
		{
			OpenGUI(mMainGui ? nullptr : new DemoGuiScreen(mD2DDrawer.get()));
			mPrevGKeyState = currGKeyState;
		}
	}
	else
	{
		mPrevGKeyState = false;
	}
	//mGround->Rotate(0.0f, dt.Delta() * 0.39f, 0.0f);
    mCamera->UpdateViewMatrix();
}

void DemoApp::DrawScene( const Timer& dt )
{
    assert( mD3DContext != nullptr );
    assert( mSwapChain != nullptr );
	Clear();
	mRenderer->RenderThis(mSphere.get());
	mRenderer->RenderThis(mGround.get());
	mRenderer->StartDrawProcess();
}
