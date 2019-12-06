#include "DefferedEffect.h"

#include "BasicEffect.h"

DefferedEffect::DefferedEffect(ID3D11Device* device) :
	Effect(device, L"deffered_shader.hlsl", nullptr, BasicEffect::InputLayout)
{
	D3D11_BUFFER_DESC cbDesc;
	ZeroMemory(&cbDesc, sizeof(cbDesc));
	cbDesc.Usage = D3D11_USAGE_DEFAULT;
	cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbDesc.CPUAccessFlags = 0;
	cbDesc.MiscFlags = 0;
	cbDesc.StructureByteStride = 0;
	cbDesc.ByteWidth = D3DUtilities::AlignConstantBufferByte(sizeof(BasicEffect::CBObject));
	ThrowIfFailed(device->CreateBuffer(&cbDesc, nullptr, &mConstantBuffers[0]));
}

void DefferedEffect::SetWorld(FXMMATRIX M)
{
	XMStoreFloat4x4(&mCBObject.World, M);
}

void DefferedEffect::SetWorldInvTranspose(FXMMATRIX M)
{
	XMStoreFloat4x4(&mCBObject.WorldInvTranspose, M);
}

void DefferedEffect::SetWorldViewProj(FXMMATRIX M)
{
	XMStoreFloat4x4(&mCBObject.WorldViewProj, M);
}

void DefferedEffect::SetTexTransform(FXMMATRIX M)
{
	XMStoreFloat4x4(&mCBObject.TexTransform, M);
}

void DefferedEffect::UpdateConstantBuffers(ID3D11DeviceContext* context)
{
	context->UpdateSubresource(mConstantBuffers[0].Get(), 0, nullptr, &mCBObject, 0, 0);
}
