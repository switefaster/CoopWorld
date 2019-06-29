#include "Renderer.h"

Renderer::Renderer( ID3D11Device* device, ID3D11DeviceContext* context, D3DApplication* app ) :
    mD3DDeviceContext( context ),
	mD3DDevice(device),
	mEffectManager(std::make_unique<EffectManager>(device)),
	mStateManager(std::make_unique<StateManager>(device)),
	mTextureManager(std::make_unique<TextureManager>(device)),
	mFontManager(std::make_unique<FontManager>(device)),
	mCamera(std::make_unique<Camera>()),
	mScene(std::make_unique<Scene>()),
	mRenderPasses(GetPasses()),
    mApp( app )
{
	mBuffers.resize(mRenderPasses.size());
	mRTVs.resize(mRenderPasses.size());
	mSRVs.resize(mRenderPasses.size());

	D3D11_TEXTURE2D_DESC texDesc;
	ZeroMemory(&texDesc, sizeof(texDesc));
	texDesc.Width = static_cast<UINT>(mApp->GetViewport().Width);
	texDesc.Height = static_cast<UINT>(mApp->GetViewport().Height);
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	texDesc.SampleDesc.Count = 1;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = 0;

	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
	ZeroMemory(&rtvDesc, sizeof(rtvDesc));
	rtvDesc.Format = texDesc.Format;
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	rtvDesc.Texture2D.MipSlice = 0;

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(srvDesc));
	srvDesc.Format = texDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;
	srvDesc.Texture2D.MostDetailedMip = 0;

	for (int i = 0; i < mRenderPasses.size(); i++)
	{
		if (mRenderPasses[i]->ShareBuffer())
		{
			ThrowIfFailed(mD3DDevice->CreateTexture2D(&texDesc, nullptr, &mBuffers[i]));
			ThrowIfFailed(mD3DDevice->CreateShaderResourceView(mBuffers[i].Get(), &srvDesc, &mSRVs[i]));
			ThrowIfFailed(mD3DDevice->CreateRenderTargetView(mBuffers[i].Get(), &rtvDesc, &mRTVs[i]));
		}
		mCorrespondingIndices[mRenderPasses[i]->GetGBufferName()] = i;
	}

	D3D11_TEXTURE2D_DESC dsDesc;
	ZeroMemory(&dsDesc, sizeof(dsDesc));
	dsDesc.Width = texDesc.Width;
	dsDesc.Height = texDesc.Height;
	dsDesc.MipLevels = 1;
	dsDesc.ArraySize = 1;
	dsDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	dsDesc.SampleDesc.Count = 1;
	dsDesc.SampleDesc.Quality = 0;
	dsDesc.Usage = D3D11_USAGE_DEFAULT;
	dsDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	dsDesc.CPUAccessFlags = 0;
	dsDesc.MiscFlags = 0;

	ThrowIfFailed(mD3DDevice->CreateTexture2D(&dsDesc, nullptr, &mDepthStencilBuffer));

	CD3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
	ZeroMemory(&dsvDesc, sizeof(dsvDesc));
	dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	dsvDesc.Texture2D.MipSlice = 0;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	ThrowIfFailed(mD3DDevice->CreateDepthStencilView(mDepthStencilBuffer.Get(), &dsvDesc, &mDSV));
}

void Renderer::StartDrawProcess()
{
	for (auto& v : mRenderPasses)
	{
		if (v->ShareBuffer())
		{
			float black[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
			ID3D11RenderTargetView* rtv = GetRTV(v->GetGBufferName());
			mD3DDeviceContext->ClearRenderTargetView(rtv, black);
			mD3DDeviceContext->ClearDepthStencilView(GetDSV(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
			mD3DDeviceContext->OMSetRenderTargets(1, &rtv, GetDSV());
		}
		v->DoRender(mD3DDeviceContext, mItemsToDraw, this);
	}
	mItemsToDraw.clear();
}

void Renderer::RenderThis(RenderItem* item)
{
	mItemsToDraw.push_back(item);
}

ID3D11RenderTargetView* Renderer::GetRTV(std::string id) const
{
	return GetRTV(mCorrespondingIndices.at(id));
}

ID3D11RenderTargetView* Renderer::GetRTV(UINT id) const
{
	return mRTVs[id].Get();
}

ID3D11ShaderResourceView* Renderer::GetSRV(std::string id) const
{
	return GetSRV(mCorrespondingIndices.at(id));
}

ID3D11ShaderResourceView* Renderer::GetSRV(UINT id) const
{
	return mSRVs[id].Get();
}

RenderPass* Renderer::GetPass(std::string id) const
{
	return GetPass(mCorrespondingIndices.at(id));
}

RenderPass* Renderer::GetPass(UINT id) const
{
	return mRenderPasses[id].get();
}

D3DApplication* Renderer::GetApp() const
{
	return mApp;
}
