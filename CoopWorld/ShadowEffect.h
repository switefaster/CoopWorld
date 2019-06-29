#pragma once

#include "BasicEffect.h"

class ShadowEffect : public Effect {
    public:
        ShadowEffect( ID3D11Device* device );
        ~ShadowEffect() = default;

        void SetLightWVP( FXMMATRIX M );
        void SetTexTransform( FXMMATRIX M );
        void SetTexture( ID3D11ShaderResourceView* srv );

	protected:
		void UpdateConstantBuffers(ID3D11DeviceContext* context) override;

    public:
        struct CBFrame
        {
            XMFLOAT4X4 LightWVP;
            XMFLOAT4X4 TexTransform;
            UINT HasTexture;
            XMFLOAT3 Pad;
        };

    private:
        CBFrame mCBFrame;
        ID3D11ShaderResourceView* mDiffuseMap;
};

