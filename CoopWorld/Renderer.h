#pragma once

#include "RenderItem.h"
#include "D3DUtilities.h"
#include "EffectManager.h"
#include "Camera.h"
#include "ShadowMap.h"
#include <list>

struct Scene
{
    DirectionalLight Lights[3];
    float FogStart;
    float FogRange;
    XMFLOAT3 FogColor;
    XMFLOAT3 LightCount;

    struct BoundingSphere
    {
        XMFLOAT3 Center;
        float Radius;
    } Bounds;
};

class StateManager {
    public:
        StateManager( ID3D11Device* device );
        StateManager( const StateManager& rhs ) = delete;
        StateManager& operator=( const StateManager& rhs ) = delete;

    public:
        ComPtr<ID3D11SamplerState> LinearSampler;
        ComPtr<ID3D11SamplerState> ComparisonSampler;
        ComPtr<ID3D11RasterizerState> SlopeScaledBiasRasterizer;
};

class Renderer {
    public:
        Renderer( ID3D11Device* device, ID3D11DeviceContext* context, EffectManager* fxMan, StateManager* stateMan );
        Renderer( const Renderer& rhs ) = delete;
        Renderer& operator=( const Renderer& rhs ) = delete;
        ~Renderer() = default;

        void Prepare( const Scene& scene, const Camera& camera );

        void Draw( RenderItem* itemToDraw );

        void Post( const Camera& camera );

    private:
        void BuildShadowTransform( const Scene& scene );

    private:
        ID3D11DeviceContext* mD3DDeviceContext;
        EffectManager* mFXMan;
        StateManager* mStateMan;
        std::unique_ptr<ShadowMap> mShadowMap;
        std::list<RenderItem*> mRenderItemList;

        XMFLOAT4X4 mLightVP;
        XMFLOAT4X4 mShadowTransform;
};
