#include "ShadowPass.h"

ShadowPass::ShadowPass(ID3D11Device* device, UINT width, UINT height) :
	mShadowMap(std::make_unique<ShadowMap>(device, width, height))
{
}

std::string ShadowPass::GetGBufferName()
{
	return "shadow";
}

void ShadowPass::DoRender(ID3D11DeviceContext* context, const std::vector<RenderItem*>& renderItems, Renderer* renderer)
{
	mShadowMap->BindDsvAndNullRtv(context);
	ShadowEffect* shadowFX = renderer->GetFXMan()->ShadowFX.get();
	StateManager* stateMan = renderer->GetStateMan();
	context->RSSetState(stateMan->SlopeScaledBiasRasterizer.Get());
	context->PSSetSamplers(0, 1, stateMan->ComparisonSampler.GetAddressOf());
	for (auto& v : renderItems)
	{
		XMMATRIX W = v->BuildWorldMatrix();
		XMMATRIX VP = XMLoadFloat4x4(&mLightVP);
		shadowFX->SetLightWVP(XMMatrixMultiplyTranspose(W, VP));
		shadowFX->SetTexTransform(XMMatrixTranspose(v->GetTexTransform()));
		if (v->GetMesh()->HasTexture())
		{
			shadowFX->SetTexture(v->GetMesh()->GetTexture()->GetShaderResourceView());
		}
		shadowFX->Apply(context);
		v->GetMesh()->Apply(context);
	}
}

XMFLOAT4X4 ShadowPass::GetLightVP() const
{
	return mLightVP;
}

XMFLOAT4X4 ShadowPass::GetShadowTransform() const
{
	return mShadowTransform;
}

void ShadowPass::BuildShadowTransform(Renderer* renderer)
{
	Scene scene = *renderer->GetScene();
	if (scene.LightCount.x > 0)
	{
		XMVECTOR lightDir = XMLoadFloat3(&scene.Lights[0].Direction);
		XMVECTOR lightPos = -2.0f * scene.Bounds.Radius * lightDir;
		XMVECTOR targetPos = XMLoadFloat3(&scene.Bounds.Center);
		XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
		XMMATRIX V = XMMatrixLookAtLH(lightPos, targetPos, up);
		XMFLOAT3 sphereCenterLS;
		XMStoreFloat3(&sphereCenterLS, XMVector3TransformCoord(targetPos, V));
		float l = sphereCenterLS.x - scene.Bounds.Radius;
		float b = sphereCenterLS.y - scene.Bounds.Radius;
		float n = sphereCenterLS.z - scene.Bounds.Radius;
		float r = sphereCenterLS.x + scene.Bounds.Radius;
		float t = sphereCenterLS.y + scene.Bounds.Radius;
		float f = sphereCenterLS.z + scene.Bounds.Radius;
		XMMATRIX P = XMMatrixOrthographicOffCenterLH(l, r, b, t, n, f);
		XMMATRIX T(
			0.5f, 0.0f, 0.0f, 0.0f,
			0.0f, -0.5f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.5f, 0.5f, 0.0f, 1.0f);
		XMMATRIX VP = V * P;
		XMMATRIX S = VP * T;
		XMStoreFloat4x4(&mLightVP, VP);
		XMStoreFloat4x4(&mShadowTransform, S);
		return;
	}

	mLightVP = mShadowTransform = Identity4x4;
}

bool ShadowPass::ShareBuffer()
{
	return false;
}

ID3D11ShaderResourceView* ShadowPass::GetSRV() const
{
	return mShadowMap->DepthMapSRV();
}
