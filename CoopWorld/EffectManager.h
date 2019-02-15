#pragma once

#include "D3DUtilities.h"
#include "ColorEffect.h"
#include "BasicEffect.h"

class EffectManager {
    public:
        EffectManager() = default;
        ~EffectManager() = default;

        void Initialize( ID3D11Device* device );

        std::unique_ptr<ColorEffect> ColorFX;
		std::unique_ptr<BasicEffect> BasicFX;
};

