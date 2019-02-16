#include "EffectManager.h"

EffectManager::EffectManager( ID3D11Device* device )
{
    ColorFX = std::make_unique<ColorEffect>( device );
    BasicFX = std::make_unique<BasicEffect>( device );
}
