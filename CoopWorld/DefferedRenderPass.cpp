#include "DefferedRenderPass.h"

void DefferedRenderPass::DoRender(ID3D11DeviceContext* context, const std::vector<RenderItem*>& renderItems)
{

}

std::string DefferedRenderPass::GetID() const
{
	return "deffered";
}
