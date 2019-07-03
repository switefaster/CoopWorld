#include "ForwardPass.h"

std::string ForwardPass::GetGBufferName()
{
	return "forward";
}

void ForwardPass::DoRender(ID3D11DeviceContext* context, const std::vector<RenderItem*>& renderItems, Renderer* renderer)
{
	float black[4] = { 0.0f,0.0f,0.0f,0.0f };
	ID3D11RenderTargetView* rtv[] = { renderer->GetApp()->GetRtv() };
	context->ClearRenderTargetView(rtv[0], black);
	context->ClearDepthStencilView(renderer->GetApp()->GetDsv(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	context->OMSetRenderTargets(1, rtv, renderer->GetApp()->GetDsv());
	StateManager* stateMan = renderer->GetStateMan();
	BasicEffect* basicFX = renderer->GetFXMan()->BasicFX.get();
	context->RSSetState(nullptr);
	context->PSSetSamplers(0, 1, stateMan->LinearSampler.GetAddressOf());
	const Camera* cam = renderer->GetCamera();
	Scene* scene = renderer->GetScene();
	ShadowPass* sPass = static_cast<ShadowPass*>(renderer->GetPass("shadow"));
	for (auto& v : renderItems)
	{
		XMMATRIX W = v->BuildWorldMatrix();
		XMMATRIX VP = cam->ViewProj();
		basicFX->SetWorld(XMMatrixTranspose(W));
		basicFX->SetWorldViewProj(XMMatrixMultiplyTranspose(W, VP));
		basicFX->SetWorldInvTranspose(XMMatrixTranspose(D3DUtilities::InvTranspose(W)));
		basicFX->SetEyePos(cam->GetPosition());
		basicFX->SetFogColor(scene->FogColor);
		basicFX->SetFogRange(scene->FogRange);
		basicFX->SetFogStart(scene->FogStart);
		basicFX->SetLights(scene->Lights, scene->LightCount.x);
		basicFX->SetMaterial(v->GetMesh()->GetMaterial());
		if (v->GetMesh()->HasTexture())
		{
			basicFX->SetShaderResourceView(v->GetMesh()->GetTexture()->GetShaderResourceView());
		}
		basicFX->SetShadowMap(sPass->GetSRV());
		basicFX->SetShadowTransform(XMMatrixTranspose(XMLoadFloat4x4(&sPass->GetShadowTransform())));
		basicFX->SetTexTransform(v->GetTexTransform());
		basicFX->Apply(context);
		v->GetMesh()->Apply(context);
	}
}

bool ForwardPass::ShareBuffer()
{
	return false;
}
