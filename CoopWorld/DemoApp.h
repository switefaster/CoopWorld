#pragma once

#include "D3DApplication.h"
#include "Mesh.h"
#include "EffectManager.h"
#include "Renderer.h"

class DemoApp : public D3DApplication {
    public:
        DemoApp( HINSTANCE hInstance );
        DemoApp( const DemoApp& rhs ) = delete;
        DemoApp& operator=( const DemoApp& rhs ) = delete;
        ~DemoApp();

        bool Initialize() override;
        void OnResize() override;
        void Update( const Timer& dt ) override;
        void Draw( const Timer& dt ) override;

    private:
        std::unique_ptr<EffectManager> mEffectManager;
        std::unique_ptr<Mesh> mTriangleMesh;
		std::unique_ptr<RenderItem> mRenderItem;
		std::unique_ptr<Renderer> mRenderer;

		Scene mScene;
		Camera mCamera;

		float mRotY;
		float mPitch;
};

