#pragma once

#include "D3DApplication.h"
#include "Mesh.h"
#include "EffectManager.h"
#include "Renderer.h"
#include "FontManager.h"

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
        std::unique_ptr<Mesh> mSphereMesh;
		std::unique_ptr<RenderItem> mRenderItem;
		std::unique_ptr<Renderer> mRenderer;
		std::unique_ptr<TextureManager> mTextureManager;
		std::unique_ptr<FontManager> mFontManager;

		Scene mScene;
		Camera mCamera;
};

