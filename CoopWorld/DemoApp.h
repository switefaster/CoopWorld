#pragma once

#include "D3DApplication.h"
#include "Mesh.h"
#include "ShadowedRenderer.h"

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
        std::unique_ptr<Mesh> mSphereMesh;
		std::unique_ptr<Mesh> mPlateMesh;
		std::unique_ptr<RenderItem> mSphere;
		std::unique_ptr<RenderItem> mGround;

		Scene* mScene;
		Camera* mCamera;
};
