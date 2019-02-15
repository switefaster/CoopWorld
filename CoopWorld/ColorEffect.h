#pragma once

#include "Effect.h"

class ColorEffect : public Effect {
    public:
        static std::vector<D3D11_INPUT_ELEMENT_DESC> InputLayout;
        struct Vertex
        {
            XMFLOAT3 Pos;
            XMFLOAT4 Color;
        };
        struct CBuffer
        {
			XMMATRIX WorldMatrix = XMMatrixIdentity();
        };
    public:
        ColorEffect( ID3D11Device* device );

        void SetWorld( FXMMATRIX M );

    protected:
        void UpdateConstantBuffers( ID3D11DeviceContext* context ) override;

    private:
        CBuffer mWorldMatrix;
};

