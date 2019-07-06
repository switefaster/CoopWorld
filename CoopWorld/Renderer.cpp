#include "Renderer.h"

#include "ForwardPass.h"

Renderer::Renderer( ID3D11Device* device, ID3D11DeviceContext* context ) :
    mD3DDeviceContext( context ),
	mD3DDevice(device),
	mEffectManager(std::make_unique<EffectManager>(device)),
	mStateManager(std::make_unique<StateManager>(device)),
	mTextureManager(std::make_unique<TextureManager>(device)),
	mCamera(std::make_unique<Camera>()),
	mScene(std::make_unique<Scene>()),
	mRenderPasses(GetPasses())
{
	for (size_t i = 0; i < mRenderPasses.size(); i++)
	{
		mCorrespondingIndices[mRenderPasses[i]->GetID()] = i;
	}
}

Renderer::~Renderer()
{
}

void Renderer::StartDrawProcess()
{
	for (auto& v : mRenderPasses)
	{
		v->DoRender(mD3DDeviceContext, mItemsToDraw);
	}
	mItemsToDraw.clear();
}

void Renderer::RenderThis(RenderItem* item)
{
	mItemsToDraw.push_back(item);
}

RenderPass* Renderer::GetPass(std::string id) const
{
	return GetPass(mCorrespondingIndices.at(id));
}

RenderPass* Renderer::GetPass(UINT id) const
{
	return mRenderPasses[id].get();
}

StateManager* Renderer::GetStateManager()
{
	return mStateManager.get();
}

EffectManager* Renderer::GetFXManager()
{
	return mEffectManager.get();
}

TextureManager* Renderer::GetTextureManager()
{
	return mTextureManager.get();
}

Scene* Renderer::GetScene()
{
	return mScene.get();
}

Camera* Renderer::GetCamera()
{
	return mCamera.get();
}

std::vector<std::unique_ptr<RenderPass>> Renderer::GetPasses()
{
	std::vector<std::unique_ptr<RenderPass>> list;
	list.emplace_back(std::make_unique<ShadowPass>(mD3DDevice, 2048, 2048));
	list.emplace_back(std::make_unique<ForwardPass>());
	return list;
}
