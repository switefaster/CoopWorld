#pragma once

#include "RenderItem.h"
#include "D3DUtilities.h"
#include "EffectManager.h"
#include "Camera.h"
#include "D3DApplication.h"
#include "StateManager.h"
#include "RenderItem.h"
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

class Renderer;

class RenderPass
{
public:
	RenderPass() = default;
	~RenderPass() = default;
	RenderPass& operator=(const RenderPass& rhs) = delete;
	RenderPass(const RenderPass& rhs) = delete;

	virtual std::string GetGBufferName() = 0;
	virtual void DoRender(ID3D11DeviceContext* context, const std::vector<RenderItem*>& renderItems, Renderer* renderer) = 0;
	virtual bool ShareBuffer() = 0;
};

class Renderer {
    public:
        Renderer( ID3D11Device* device, ID3D11DeviceContext* context, D3DApplication* app );
        Renderer( const Renderer& rhs ) = delete;
        Renderer& operator=( const Renderer& rhs ) = delete;
        ~Renderer() = default;

		void StartDrawProcess();
		void RenderThis(RenderItem* item);

		ID3D11RenderTargetView* GetRTV(std::string id) const;
		ID3D11RenderTargetView* GetRTV(UINT id) const;
		ID3D11ShaderResourceView* GetSRV(std::string id) const;
		ID3D11ShaderResourceView* GetSRV(UINT id) const;
		RenderPass* GetPass(std::string id) const;
		RenderPass* GetPass(UINT id) const;

		D3DApplication* GetApp() const;

		StateManager* GetStateMan()
		{
			return mStateManager.get();
		}

		EffectManager* GetFXMan()
		{
			return mEffectManager.get();
		}

		TextureManager* GetTexMan()
		{
			return mTextureManager.get();
		}

		FontManager* GetFontMan()
		{
			return mFontManager.get();
		}

		Scene* GetScene()
		{
			return mScene.get();
		}

		Camera* GetCamera()
		{
			return mCamera.get();
		}

		ID3D11DepthStencilView* GetDSV()
		{
			return mDSV.Get();
		}

	public:
		virtual std::vector<std::unique_ptr<RenderPass>> GetPasses() = 0;
    protected:
        ID3D11DeviceContext* mD3DDeviceContext;
		ID3D11Device* mD3DDevice;
		D3DApplication* mApp;

		std::vector<std::unique_ptr<RenderPass>> mRenderPasses;
		std::unordered_map<std::string, UINT> mCorrespondingIndices;
		std::vector<ComPtr<ID3D11Texture2D>> mBuffers;
		std::vector<ComPtr<ID3D11ShaderResourceView>> mSRVs;
		std::vector<ComPtr<ID3D11RenderTargetView>> mRTVs;
		ComPtr<ID3D11Texture2D> mDepthStencilBuffer;
		ComPtr<ID3D11DepthStencilView> mDSV;
		std::unique_ptr<Scene> mScene;
		std::unique_ptr<Camera> mCamera;

		std::unique_ptr<EffectManager> mEffectManager;
		std::unique_ptr<StateManager> mStateManager;
		std::unique_ptr<TextureManager> mTextureManager;
		std::unique_ptr<FontManager> mFontManager;

		std::vector<RenderItem*> mItemsToDraw;
};
