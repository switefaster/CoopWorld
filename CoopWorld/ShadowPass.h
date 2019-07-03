#pragma once

#ifndef _SHADOW_PASS_H_
#define _SHADOW_PASS_H_

#include "Renderer.h"
#include "ShadowMap.h"

class ShadowPass :
	public RenderPass
{
public:
	ShadowPass(ID3D11Device* device, UINT width, UINT height);

	virtual std::string GetGBufferName() override;
	virtual void DoRender(ID3D11DeviceContext* context, const std::vector<RenderItem*>& renderItems, Renderer* renderer) override;
	virtual bool ShareBuffer() override;

	ID3D11ShaderResourceView* GetSRV() const;

	XMFLOAT4X4 GetLightVP() const;
	XMFLOAT4X4 GetShadowTransform() const;

private:
	void BuildShadowTransform(Renderer* renderer);

private:
	std::unique_ptr<ShadowMap> mShadowMap;

	XMFLOAT4X4 mLightVP;
	XMFLOAT4X4 mShadowTransform;
};

#endif
