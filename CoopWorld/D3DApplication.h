#pragma once

#if defined(DEBUG) || defined(_DEBUG)
    #define _CRTDBG_MAP_ALLOC
    #include <crtdbg.h>
#endif

#include <Windows.h>
#include <wrl.h>
#include <thread>

#include "Timer.h"
#include "D3DUtilities.h"
#include "EffectManager.h"
#include "FontManager.h"
#include "StateManager.h"
#include "RenderItem.h"

#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "D3D11.lib")
#pragma comment(lib, "dxgi.lib")

//C++ SB
class Renderer;

class D3DApplication {
    protected:
        D3DApplication( HINSTANCE hInstance );
        D3DApplication( const D3DApplication& rhs ) = delete;
        D3DApplication& operator=( const D3DApplication& rhs ) = delete;
        virtual ~D3DApplication();

    public:
        static D3DApplication* GetInstance();

        HINSTANCE GetHInstance() const;
        HWND GetHWND() const;
        float AspectRatio() const;

        bool Get4xMsaaState();
        void Set4xMsaaState( bool newState );

        D3D11_VIEWPORT GetViewport() const;
        ID3D11RenderTargetView* GetRtv() const;
        ID3D11DepthStencilView* GetDsv() const;
        XMFLOAT4 GetClearColor() const;

        int Run();

        virtual bool Initialize();
        virtual LRESULT MsgProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam );

    protected:
        virtual void OnResize();
        virtual void Update( const Timer& dt ) = 0;
        virtual void Draw( const Timer& dt ) = 0;

        virtual void OnMouseDown( WPARAM mouseState, int x, int y );
        virtual void OnMouseUp( WPARAM mouseState, int x, int y );
        virtual void OnMouseMove( WPARAM mouseState, int x, int y );

    protected:
        bool InitWindow();
        bool InitD3DContents();
        void CreateSwapChain();

        void FixFPS( Timer& timer);
		void FixTPS( Timer& timer);

		void CountFPS(Timer& timer);
		void CountTPS(Timer& timer);

        void Clear();

    protected:
        static D3DApplication* mInstance;

        HINSTANCE mHInstance = nullptr;
        HWND mHwnd = nullptr;

        bool mAppPaused = false;
        bool mMinimized = false;
        bool mMaximized = false;
        bool mResizing = false;
        bool mFullScreen = false;

        bool m4xMsaa = false;
        UINT m4xMsaaQuality = 0;

        ComPtr<IDXGISwapChain> mSwapChain;
        ComPtr<ID3D11Device> mD3DDevice;
        ComPtr<ID3D11DeviceContext> mD3DContext;
        ComPtr<ID3D11Texture2D> mDepthStencilBuffer;
        ComPtr<ID3D11DepthStencilView> mDepthStencilView;
        ComPtr<ID3D11RenderTargetView> mRenderTargetView;
		std::unique_ptr<Renderer> mRenderer;
        D3D11_VIEWPORT mViewport;

		std::thread mLogicThread;

		Timer mRenderTimer;
		Timer mLogicTimer;

		float mFPS;
		float mTPS;

        D3D_DRIVER_TYPE mD3DDriverType = D3D_DRIVER_TYPE_HARDWARE;

        std::wstring mCaption = L"D3D11 Application";
        int mClientWidth = 800;
        int mClientHeight = 600;
};

