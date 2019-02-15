#pragma once

#include "RenderItem.h"
#include "D3DUtilities.h"
#include "EffectManager.h"
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

        void Draw( RenderItem* itemToDraw, const Scene& scene );

    private:
        ID3D11DeviceContext* mD3DDeviceContext;
		EffectManager* mFXMan;
        //Shadow Mapping
        //Post-draw
        std::list<RenderItem*> mRenderItemList;
};
