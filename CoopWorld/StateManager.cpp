#include "StateManager.h"

StateManager::StateManager(ID3D11Device* device)
{
	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(sampDesc));
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	ThrowIfFailed(device->CreateSamplerState(&sampDesc, &LinearSampler));
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
	sampDesc.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_LINEAR_MIP_POINT;
	sampDesc.BorderColor[0] = 1.0f;
	sampDesc.BorderColor[1] = 1.0f;
	sampDesc.BorderColor[2] = 1.0f;
	sampDesc.BorderColor[3] = 1.0f;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_LESS_EQUAL;
	ThrowIfFailed(device->CreateSamplerState(&sampDesc, &ComparisonSampler));
	D3D11_RASTERIZER_DESC rastDesc;
	ZeroMemory(&rastDesc, sizeof(rastDesc));
	rastDesc.FillMode = D3D11_FILL_SOLID;
	rastDesc.CullMode = D3D11_CULL_BACK;
	rastDesc.SlopeScaledDepthBias = 10;
	rastDesc.DepthBiasClamp = 0.0f;
	rastDesc.DepthBias = 1.0f;
	rastDesc.DepthClipEnable = true;
	ThrowIfFailed(device->CreateRasterizerState(&rastDesc, &SlopeScaledBiasRasterizer));
}
