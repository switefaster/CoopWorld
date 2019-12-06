#pragma once

#ifndef _EFFECT_MANAGER_H_
#define _EFFECT_MANAGER_H_

#include "D3DUtilities.h"
#include "ColorEffect.h"
#include "BasicEffect.h"
#include "ShadowEffect.h"
#include "DefferedEffect.h"

class EffectManager {
    public:
        EffectManager( ID3D11Device* device );
        ~EffectManager() = default;

        std::unique_ptr<ColorEffect> ColorFX;
        std::unique_ptr<BasicEffect> BasicFX;
		std::unique_ptr<ShadowEffect> ShadowFX;
		std::unique_ptr<DefferedEffect> DefferedFX;
};

#endif
