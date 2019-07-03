#pragma once

#ifndef _FORWARD_PASS_H_
#define _FORWARD_PASS_H_

#include "Renderer.h"
#include "ShadowPass.h"

class ForwardPass :
	public RenderPass
{
public:
	virtual std::string GetGBufferName() override;
	virtual void DoRender(ID3D11DeviceContext* context, const std::vector<RenderItem*>& renderItems, Renderer* renderer) override;
	virtual bool ShareBuffer() override;
};

#endif
