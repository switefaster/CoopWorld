#pragma once

#include "D3DUtilities.h"
#include "ColorEffect.h"
#include "BasicEffect.h"

class EffectManager {
    public:
        EffectManager( ID3D11Device* device );
        ~EffectManager() = default;

        std::unique_ptr<ColorEffect> ColorFX;
        std::unique_ptr<BasicEffect> BasicFX;
};

