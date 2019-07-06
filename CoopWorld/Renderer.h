#pragma once

#ifndef _RENDERER_H_
#define _RENDERER_H_

#include "RenderItem.h"
#include "D3DUtilities.h"
#include "EffectManager.h"
#include "Texture.h"
#include "Camera.h"
#include "StateManager.h"
#include <list>

//TODO: Implement deffered rendering

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

class RenderPass
{
public:
	RenderPass() = default;
	virtual ~RenderPass() = default;
	RenderPass& operator=(const RenderPass& rhs) = delete;
	RenderPass(const RenderPass& rhs) = delete;

	virtual void DoRender(ID3D11DeviceContext* context, const std::vector<RenderItem*>& renderItems) = 0;
	virtual std::string GetID() const = 0;
};

class Renderer {
    public:
        Renderer( ID3D11Device* device, ID3D11DeviceContext* context);
        Renderer( const Renderer& rhs ) = delete;
        Renderer& operator=( const Renderer& rhs ) = delete;
		~Renderer();

		void StartDrawProcess();
		void RenderThis(RenderItem* item);

		RenderPass* GetPass(std::string id) const;
		RenderPass* GetPass(UINT id) const;

		StateManager* GetStateManager();
		EffectManager* GetFXManager();
		TextureManager* GetTextureManager();
		Scene* GetScene();
		Camera* GetCamera();

		std::vector<std::unique_ptr<RenderPass>> GetPasses();
    protected:
        ID3D11DeviceContext* mD3DDeviceContext;
		ID3D11Device* mD3DDevice;

		std::vector<std::unique_ptr<RenderPass>> mRenderPasses;
		std::unordered_map<std::string, UINT> mCorrespondingIndices;
		std::unique_ptr<Scene> mScene;
		std::unique_ptr<Camera> mCamera;

		std::unique_ptr<EffectManager> mEffectManager;
		std::unique_ptr<StateManager> mStateManager;
		std::unique_ptr<TextureManager> mTextureManager;

		std::vector<RenderItem*> mItemsToDraw;
};

#endif
