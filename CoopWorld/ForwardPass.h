#pragma once
#include "Renderer.h"

class ForwardPass :
	public RenderPass
{
public:
	virtual std::string GetGBufferName() override;
	virtual void DoRender(ID3D11DeviceContext* context, const std::vector<RenderItem*>& renderItems, Renderer* renderer) override;
	virtual bool ShareBuffer() override;
};

