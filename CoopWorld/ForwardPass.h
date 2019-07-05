#pragma once

#ifndef _FORWARD_PASS_H_
#define _FORWARD_PASS_H_

#include "Renderer.h"
#include "ShadowPass.h"

class ForwardPass :
	public RenderPass
{
public:
	ForwardPass() = default;
	~ForwardPass() = default;

	virtual void DoRender(ID3D11DeviceContext* context, const std::vector<RenderItem*>& renderItems) override;
	virtual std::string GetID() const override;
};

#endif
