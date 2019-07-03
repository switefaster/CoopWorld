#pragma once

#ifndef _DEMO_APP_H_
#define _DEMO_APP_H_

#include "D3DApplication.h"
#include "Renderer.h"
#include "Mesh.h"

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

		std::unique_ptr<Renderer> mRenderer;

		Scene* mScene;
		Camera* mCamera;
};

#endif
