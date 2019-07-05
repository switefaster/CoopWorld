#include "ForwardPass.h"

#include "D3DApplication.h"

void ForwardPass::DoRender(ID3D11DeviceContext* context, const std::vector<RenderItem*>& renderItems)
{
	float black[4] = { 0.0f,0.0f,0.0f,0.0f };
	ID3D11RenderTargetView* rtv[] = { D3DApplication::GetInstance()->GetRtv() };
	context->ClearRenderTargetView(rtv[0], black);
	context->ClearDepthStencilView(D3DApplication::GetInstance()->GetDsv(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	context->OMSetRenderTargets(1, rtv, D3DApplication::GetInstance()->GetDsv());
	StateManager* stateMan = D3DApplication::GetInstance()->GetRenderer()->GetStateManager();
	BasicEffect* basicFX = D3DApplication::GetInstance()->GetRenderer()->GetFXManager()->BasicFX.get();
	context->RSSetState(nullptr);
	context->PSSetSamplers(0, 1, stateMan->LinearSampler.GetAddressOf());
	context->PSSetSamplers(1, 1, stateMan->ComparisonSampler.GetAddressOf());
	context->RSSetViewports(1, &D3DApplication::GetInstance()->GetViewport());
	const Camera* cam = D3DApplication::GetInstance()->GetRenderer()->GetCamera();
	Scene* scene = D3DApplication::GetInstance()->GetRenderer()->GetScene();
	ShadowPass* sPass = static_cast<ShadowPass*>(D3DApplication::GetInstance()->GetRenderer()->GetPass("shadow"));
	basicFX->SetEyePos(cam->GetPosition());
	basicFX->SetFogColor(scene->FogColor);
	basicFX->SetFogRange(scene->FogRange);
	basicFX->SetFogStart(scene->FogStart);
	basicFX->SetLights(scene->Lights, scene->LightCount.x);
	basicFX->SetShadowMap(sPass->GetSRV());
	basicFX->SetShadowTransform(XMMatrixTranspose(XMLoadFloat4x4(&sPass->GetShadowTransform())));
	XMMATRIX VP = cam->ViewProj();
	for (auto& v : renderItems)
	{
		XMMATRIX W = v->BuildWorldMatrix();
		basicFX->SetWorld(XMMatrixTranspose(W));
		basicFX->SetWorldViewProj(XMMatrixMultiplyTranspose(W, VP));
		basicFX->SetWorldInvTranspose(XMMatrixTranspose(D3DUtilities::InvTranspose(W)));
		basicFX->SetMaterial(v->GetMesh()->GetMaterial());
		if (v->GetMesh()->HasTexture())
		{
			basicFX->SetShaderResourceView(v->GetMesh()->GetTexture()->GetShaderResourceView());
		}
		basicFX->SetTexTransform(v->GetTexTransform());
		basicFX->Apply(context);
		v->GetMesh()->Apply(context);
	}
	ID3D11SamplerState* samps[] = { nullptr };
	ID3D11ShaderResourceView* views[] = { nullptr };
	context->PSSetSamplers(1, 1, samps);
	context->PSSetShaderResources(1, 1, views);
}

std::string ForwardPass::GetID() const
{
	return "forward";
}
