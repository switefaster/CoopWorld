#include "ShadowedRenderer.h"

#include "ShadowPass.h"
#include "ForwardPass.h"

ShadowedRenderer::ShadowedRenderer(ID3D11Device* device, ID3D11DeviceContext* context, D3DApplication* app) :
	Renderer(device, context, app)
{
}

std::vector<std::unique_ptr<RenderPass>> ShadowedRenderer::GetPasses()
{
	std::vector<std::unique_ptr<RenderPass>> list;
	list.push_back(std::make_unique<ShadowPass>(mD3DDevice, 2048, 2048));
	list.push_back(std::make_unique<ForwardPass>());
	return list;
}
