#pragma once

#ifndef _DEFFERED_BUFFER_H_
#define _DEFFERED_BUFFER_H_

#include "D3DUtilities.h"

enum DefferedStage
{
	COLOR,
	NORMAL,
	POSITION,
	AMBIENT,
	DIFFUSE,
	SPECULAR,
	_COUNTER
};

class DefferedBuffer
{
public:
	DefferedBuffer(ID3D11Device* device);
	~DefferedBuffer() = default;

	DefferedBuffer(const DefferedBuffer& rhs) = delete;
	DefferedBuffer& operator=(const DefferedBuffer& rhs) = delete;

	void ApplyRTV(ID3D11DeviceContext* context);
	void ClearRTV(ID3D11DeviceContext* context);

	ID3D11ShaderResourceView* GetSRV(const DefferedStage& stage);

private:
	ComPtr<ID3D11Texture2D> mRenderTextureArray[DefferedStage::_COUNTER];
	ComPtr<ID3D11RenderTargetView> mRenderTargetViewArray[DefferedStage::_COUNTER];
	ID3D11RenderTargetView* mRawRTVArray[DefferedStage::_COUNTER];
	ComPtr<ID3D11ShaderResourceView> mShaderResourceViewArray[DefferedStage::_COUNTER];
};

#endif
