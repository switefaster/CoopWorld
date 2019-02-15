#pragma once

#include "D3DApplication.h"
#include "Mesh.h"
#include "EffectManager.h"

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
        EffectManager mEffectManager;
        std::unique_ptr<Mesh<ColorEffect::Vertex>> mTriangleMesh;
};

