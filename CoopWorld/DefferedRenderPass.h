#pragma once

#include "Renderer.h"
#include "DefferedBuffer.h"

class DefferedRenderPass :
	public RenderPass
{
public:
	virtual void DoRender(ID3D11DeviceContext* context, const std::vector<RenderItem*>& renderItems) override;
	virtual std::string GetID() const override;

private:
	std::unique_ptr<DefferedBuffer> mDefferedBuffer;
};

