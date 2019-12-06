#include "DefferedBuffer.h"

#include "D3DApplication.h"

DefferedBuffer::DefferedBuffer(ID3D11Device* device)
{
	D3D11_TEXTURE2D_DESC texDesc;
	ZeroMemory(&texDesc, sizeof(texDesc));
	texDesc.Width = static_cast<UINT>(D3DApplication::GetInstance()->GetViewport().Width);
	texDesc.Height = static_cast<UINT>(D3DApplication::GetInstance()->GetViewport().Height);
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	texDesc.SampleDesc.Count = 1;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = 0;

	for (int i = 0; i < DefferedStage::_COUNTER; i++)
	{
		ThrowIfFailed(device->CreateTexture2D(&texDesc, nullptr, &mRenderTextureArray[i]));
	}

	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
	renderTargetViewDesc.Format = texDesc.Format;
	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	renderTargetViewDesc.Texture2D.MipSlice = 0;

	for (int i = 0; i < DefferedStage::_COUNTER; i++)
	{
		ThrowIfFailed(device->CreateRenderTargetView(mRenderTextureArray[i].Get(), &renderTargetViewDesc, &mRenderTargetViewArray[i]));
		mRawRTVArray[i] = mRenderTargetViewArray[i].Get();
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format = texDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;
	srvDesc.Texture2D.MostDetailedMip = 0;

	for (int i = 0; i < DefferedStage::_COUNTER; i++)
	{
		ThrowIfFailed(device->CreateShaderResourceView(mRenderTextureArray[i].Get(), &srvDesc, &mShaderResourceViewArray[i]));
	}
}

void DefferedBuffer::ApplyRTV(ID3D11DeviceContext* context)
{
	context->OMSetRenderTargets(DefferedStage::_COUNTER, mRawRTVArray, D3DApplication::GetInstance()->GetDsv());
	context->RSSetViewports(1, &D3DApplication::GetInstance()->GetViewport());
}

void DefferedBuffer::ClearRTV(ID3D11DeviceContext* context)
{
	float clearColor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	for (int i = 0; i < DefferedStage::_COUNTER; i++)
	{
		context->ClearRenderTargetView(mRenderTargetViewArray[i].Get(), clearColor);
	}
	context->ClearDepthStencilView(D3DApplication::GetInstance()->GetDsv(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

ID3D11ShaderResourceView* DefferedBuffer::GetSRV(const DefferedStage& stage)
{
	return mShaderResourceViewArray[stage].Get();
}
