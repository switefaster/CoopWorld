#pragma once

#include "RenderItem.h"
#include "D3DUtilities.h"
#include "EffectManager.h"
#include "Camera.h"
#include <list>

struct Scene
{
    DirectionalLight Lights[3];
    float FogStart;
    float FogRange;
    XMFLOAT3 FogColor;
    XMFLOAT3 LightCount;
};

class Renderer {
    public:
        Renderer( ID3D11DeviceContext* context, EffectManager* fxMan );
        ~Renderer() = default;

        void Draw( RenderItem* itemToDraw, const Scene& scene, const Camera& camera );

        void Post();

    private:
        ID3D11DeviceContext* mD3DDeviceContext;
        EffectManager* mFXMan;
        std::list<RenderItem*> mRenderItemList;
};
