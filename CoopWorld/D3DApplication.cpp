#include "D3DApplication.h"

//C++ SB
#include "Renderer.h"

using namespace std;

LRESULT CALLBACK MainWndProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
    return D3DApplication::GetInstance()->MsgProc( hwnd, msg, wParam, lParam );
}

D3DApplication* D3DApplication::mInstance = nullptr;

D3DApplication::D3DApplication( HINSTANCE hInstance ) :
    mHInstance( hInstance )
{
    assert( mInstance == nullptr );
    mInstance = this;

	mLogicTimer.Reset();
}

D3DApplication::~D3DApplication()
{
    if ( mD3DContext != nullptr )
    {
        mD3DContext->ClearState();
    }
}

D3DApplication* D3DApplication::GetInstance()
{
    return mInstance;
}

HINSTANCE D3DApplication::GetHInstance() const
{
    return mHInstance;
}

HWND D3DApplication::GetHWND() const
{
    return mHwnd;
}

float D3DApplication::AspectRatio() const
{
    return static_cast<float>( mClientWidth ) / mClientHeight;
}

bool D3DApplication::Get4xMsaaState()
{
    return m4xMsaa;
}

void D3DApplication::Set4xMsaaState( bool newState )
{
    if ( newState != m4xMsaa )
    {
        m4xMsaa = newState;
        CreateSwapChain();
        OnResize();
    }
}

D3D11_VIEWPORT D3DApplication::GetViewport() const
{
    return mViewport;
}

ID3D11RenderTargetView* D3DApplication::GetRtv() const
{
    return mRenderTargetView.Get();
}

ID3D11DepthStencilView* D3DApplication::GetDsv() const
{
    return mDepthStencilView.Get();
}

XMFLOAT4 D3DApplication::GetClearColor() const
{
    return { 0.0f, 0.0f, 0.0f, 0.0f };
}

int D3DApplication::Run()
{
    MSG msg = { 0 };
    mRenderTimer.Reset();
	mLogicThread = std::thread([&]()
	{
		while (msg.message != WM_QUIT)
		{
			if (!mAppPaused)
			{
				Update(mLogicTimer);
				FixTPS(mLogicTimer);
				CountTPS(mLogicTimer);
			}
			else
			{
				std::this_thread::sleep_for(100ms);
			}
		}
	});

    while ( msg.message != WM_QUIT )
    {
        if ( PeekMessage( &msg, 0, 0, 0, PM_REMOVE ) )
        {
            TranslateMessage( &msg );
            DispatchMessage( &msg );
        }
		else 
		{
			if (!mAppPaused) 
			{
				Draw(mRenderTimer);
				FixFPS(mRenderTimer);
				CountFPS(mRenderTimer);
			}
			else
			{
				std::this_thread::sleep_for(100ms);
			}
		}
    }
	mLogicThread.join();

    return ( int )msg.wParam;
}

bool D3DApplication::Initialize()
{
    if ( !InitWindow() )
    {
        return false;
    }

    if ( !InitD3DContents() )
    {
        return false;
    }

    OnResize();
    return true;
}

LRESULT D3DApplication::MsgProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
    switch ( msg )
    {
        case WM_ACTIVATE:
            if ( LOWORD( wParam ) == WA_INACTIVE )
            {
                mAppPaused = true;
                mRenderTimer.Pause();
                mLogicTimer.Pause();
            }
            else
            {
                mAppPaused = false;
                mRenderTimer.Resume();
                mLogicTimer.Resume();
            }

            return 0;

        case WM_SIZE:
            mClientWidth = LOWORD( lParam );
            mClientHeight = HIWORD( lParam );

            if ( mD3DDevice != nullptr )
            {
                if ( wParam == SIZE_MINIMIZED )
                {
                    mAppPaused = true;
                    mMinimized = true;
                    mMaximized = false;
                }
                else if ( wParam == SIZE_MAXIMIZED )
                {
                    mAppPaused = false;
                    mMinimized = false;
                    mMaximized = true;
                    OnResize();
                }
                else if ( wParam == SIZE_RESTORED )
                {
                    if ( mMinimized )
                    {
                        mAppPaused = false;
                        mMinimized = false;
                        OnResize();
                    }
                    else if ( mMaximized )
                    {
                        mAppPaused = false;
                        mMaximized = false;
                        OnResize();
                    }
                    else if ( mResizing )
                    {
                    }
                    else
                    {
                        OnResize();
                    }
                }
            }

            return 0;

        case WM_ENTERSIZEMOVE:
            mAppPaused = true;
            mResizing = true;
            mRenderTimer.Pause();
            mLogicTimer.Pause();
            return 0;

        case WM_EXITSIZEMOVE:
            mAppPaused = false;
            mResizing = false;
            mRenderTimer.Resume();
            mLogicTimer.Resume();
            OnResize();
            return 0;

        case WM_DESTROY:
            PostQuitMessage( 0 );
            return 0;

        case WM_MENUCHAR:
            return MAKELRESULT( 0, MNC_CLOSE );

        case WM_GETMINMAXINFO:
            ( ( MINMAXINFO* )lParam )->ptMinTrackSize.x = 200;
            ( ( MINMAXINFO* )lParam )->ptMinTrackSize.y = 200;
            return 0;

        case WM_LBUTTONDOWN:
        case WM_MBUTTONDOWN:
        case WM_RBUTTONDOWN:
            OnMouseDown( wParam, LOWORD( lParam ), HIWORD( lParam ) );
            return 0;

        case WM_LBUTTONUP:
        case WM_MBUTTONUP:
        case WM_RBUTTONUP:
            OnMouseUp( wParam, LOWORD( lParam ), HIWORD( lParam ) );
            return 0;

        case WM_MOUSEMOVE:
            OnMouseMove( wParam, LOWORD( lParam ), HIWORD( lParam ) );
            return 0;
    }

    return DefWindowProc( hwnd, msg, wParam, lParam );
}

void D3DApplication::OnResize()
{
    assert( mD3DContext );
    assert( mD3DDevice );
    assert( mSwapChain );
    mRenderTargetView.Reset();
    mDepthStencilView.Reset();
    mDepthStencilBuffer.Reset();
    ThrowIfFailed( mSwapChain->ResizeBuffers( 1, mClientWidth, mClientHeight, DXGI_FORMAT_R8G8B8A8_UNORM, 0 ) );
    ComPtr<ID3D11Texture2D> backBuffer;
    ThrowIfFailed( mSwapChain->GetBuffer( 0, IID_PPV_ARGS( backBuffer.GetAddressOf() ) ) );
    ThrowIfFailed( mD3DDevice->CreateRenderTargetView( backBuffer.Get(), nullptr, mRenderTargetView.GetAddressOf() ) );
    D3D11_TEXTURE2D_DESC depthStencilDesc;
    depthStencilDesc.Width = mClientWidth;
    depthStencilDesc.Height = mClientHeight;
    depthStencilDesc.MipLevels = 1;
    depthStencilDesc.ArraySize = 1;
    depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

    if ( m4xMsaa )
    {
        depthStencilDesc.SampleDesc.Count = 4;
        depthStencilDesc.SampleDesc.Quality = m4xMsaaQuality - 1;
    }
    else
    {
        depthStencilDesc.SampleDesc.Count = 1;
        depthStencilDesc.SampleDesc.Quality = 0;
    }

    depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
    depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    depthStencilDesc.CPUAccessFlags = 0;
    depthStencilDesc.MiscFlags = 0;
    ThrowIfFailed( mD3DDevice->CreateTexture2D( &depthStencilDesc, nullptr, mDepthStencilBuffer.GetAddressOf() ) );
    ThrowIfFailed( mD3DDevice->CreateDepthStencilView( mDepthStencilBuffer.Get(), nullptr, mDepthStencilView.GetAddressOf() ) );
    mD3DContext->OMSetRenderTargets( 1, mRenderTargetView.GetAddressOf(), mDepthStencilView.Get() );
    mViewport.TopLeftX = 0;
    mViewport.TopLeftY = 0;
    mViewport.Width = static_cast<float>( mClientWidth );
    mViewport.Height = static_cast<float>( mClientHeight );
    mViewport.MinDepth = 0.0f;
    mViewport.MaxDepth = 1.0f;
    mD3DContext->RSSetViewports( 1, &mViewport );
}

void D3DApplication::OnMouseDown( WPARAM mouseState, int x, int y )
{
}

void D3DApplication::OnMouseUp( WPARAM mouseState, int x, int y )
{
}

void D3DApplication::OnMouseMove( WPARAM mouseState, int x, int y )
{
}

bool D3DApplication::InitWindow()
{
    WNDCLASS wc;
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = MainWndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = mHInstance;
    wc.hIcon = LoadIcon( 0, IDI_APPLICATION );
    wc.hCursor = LoadCursor( 0, IDC_ARROW );
    wc.hbrBackground = ( HBRUSH )GetStockObject( NULL_BRUSH );
    wc.lpszMenuName = nullptr;
    wc.lpszClassName = L"COOPWORLD_WND_CLASS";

    if ( !RegisterClass( &wc ) )
    {
        MessageBox( nullptr, L"Failed to register class!", L"Oops", NULL );
        return false;
    }

    RECT R = { 0, 0, mClientWidth, mClientHeight };
    AdjustWindowRect( &R, WS_OVERLAPPEDWINDOW, false );
    int width = R.right - R.left;
    int height = R.bottom - R.top;
    mHwnd = CreateWindow( L"COOPWORLD_WND_CLASS", mCaption.c_str(), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, width, height, nullptr, nullptr, mHInstance, nullptr );

    if ( !mHwnd )
    {
        MessageBox( nullptr, L"Failed to create window!", L"Oops", NULL );
        return false;
    }

    ShowWindow( mHwnd, SW_SHOW );
    UpdateWindow( mHwnd );
    return true;
}

bool D3DApplication::InitD3DContents()
{
    UINT deviceCreationFlags = 0;
    #if defined(DEBUG) || defined(_DEBUG)
    deviceCreationFlags |= D3D11_CREATE_DEVICE_DEBUG;
    #endif
    D3D_FEATURE_LEVEL featureLevel;
    ThrowIfFailed( D3D11CreateDevice( nullptr, mD3DDriverType, nullptr, deviceCreationFlags, nullptr, 0, D3D11_SDK_VERSION, &mD3DDevice, &featureLevel, &mD3DContext ) );

    if ( featureLevel != D3D_FEATURE_LEVEL_11_0 )
    {
        MessageBox( nullptr, L"Feature Level Unsupported!", L"Oops", NULL );
        return false;
    }

    ThrowIfFailed( mD3DDevice->CheckMultisampleQualityLevels( DXGI_FORMAT_R8G8B8A8_UNORM, 4, &m4xMsaaQuality ) );
    assert( m4xMsaaQuality > 0 );
    CreateSwapChain();
    OnResize();
    return true;
}

void D3DApplication::CreateSwapChain()
{
    DXGI_SWAP_CHAIN_DESC scDesc;
    scDesc.BufferDesc.Width = mClientWidth;
    scDesc.BufferDesc.Height = mClientHeight;
    scDesc.BufferDesc.RefreshRate.Numerator = 60;
    scDesc.BufferDesc.RefreshRate.Denominator = 1;
    scDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    scDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    scDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

    if ( m4xMsaa )
    {
        scDesc.SampleDesc.Count = 4;
        scDesc.SampleDesc.Quality = m4xMsaaQuality - 1;
    }
    else
    {
        scDesc.SampleDesc.Count = 1;
        scDesc.SampleDesc.Quality = 0;
    }

    scDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    scDesc.BufferCount = 1;
    scDesc.OutputWindow = mHwnd;
    scDesc.Windowed = true;
    scDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    scDesc.Flags = 0;
    ComPtr<IDXGIDevice> dxgiDevice;
    ComPtr<IDXGIAdapter> dxgiAdapter;
    ComPtr<IDXGIFactory> dxgiFactory;
    ThrowIfFailed( mD3DDevice->QueryInterface<IDXGIDevice>( &dxgiDevice ) );
    ThrowIfFailed( dxgiDevice->GetParent( IID_PPV_ARGS( &dxgiAdapter ) ) );
    ThrowIfFailed( dxgiAdapter->GetParent( IID_PPV_ARGS( &dxgiFactory ) ) );
    ThrowIfFailed( dxgiFactory->CreateSwapChain( mD3DDevice.Get(), &scDesc, &mSwapChain ) );
}

void D3DApplication::FixFPS( Timer& timer )
{
	timer.Tick();
	float dt = timer.Delta();
	float loopStart = timer.Elapsed() - dt;
	float loopEnd = loopStart + 1.0f / 30;
	using namespace std::chrono_literals;
	while (timer.CurrentTime() <= loopEnd)
	{
		std::this_thread::sleep_for(1ms);
	}
}

void D3DApplication::FixTPS( Timer& timer )
{
	timer.Tick();
	float dt = timer.Delta();
	float loopStart = timer.Elapsed() - dt;
	float loopEnd = loopStart + 1.0f / 10;
	using namespace std::chrono_literals;
	while (timer.CurrentTime() <= loopEnd && !mAppPaused)
	{
		std::this_thread::sleep_for(1ms);
	}
}

static int tcount = 0;
static int fcount = 0;
static float flast = 0;
static float tlast = 0;
void D3DApplication::CountFPS(Timer& timer)
{
	fcount++;
	float el = timer.Elapsed();
	if (el - flast >= 1.0f)
	{
		mFPS = fcount;
		fcount = 0;
		flast = el;
	}
}

void D3DApplication::CountTPS(Timer& timer)
{
	tcount++;
	float el = timer.Elapsed();
	if (el - tlast >= 1.0f)
	{
		mTPS = tcount;
		tcount = 0;
		tlast = el;
	}
}

void D3DApplication::Clear()
{
    mD3DContext->ClearRenderTargetView( mRenderTargetView.Get(), reinterpret_cast<const float*>( &GetClearColor() ) );
    mD3DContext->ClearDepthStencilView( mDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0 );
}
