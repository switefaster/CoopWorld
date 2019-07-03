#pragma once

#ifndef _STATE_MANAGER_H_
#define _STATE_MANAGER_H_

#include "D3DUtilities.h"

class StateManager {
public:
	StateManager(ID3D11Device* device);
	~StateManager() = default;
	StateManager(const StateManager& rhs) = delete;
	StateManager& operator=(const StateManager& rhs) = delete;

public:
	ComPtr<ID3D11SamplerState> LinearSampler;
	ComPtr<ID3D11SamplerState> ComparisonSampler;
	ComPtr<ID3D11RasterizerState> SlopeScaledBiasRasterizer;
};

#endif
