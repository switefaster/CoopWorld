#pragma once
#include "Renderer.h"
class ShadowedRenderer :
	public Renderer
{
public:
	ShadowedRenderer(ID3D11Device* device, ID3D11DeviceContext* context, D3DApplication* app);
	virtual std::vector<std::unique_ptr<RenderPass>> GetPasses() override;
};

