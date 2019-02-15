#pragma once

#include "D3DUtilities.h"
#include <vector>
#include <unordered_map>
#include <memory>

class Effect {
    public:
        Effect( const Effect& rhs ) = delete;
        Effect& operator=( const Effect& rhs ) = delete;

        virtual void Apply( ID3D11DeviceContext* context );

        ID3D11VertexShader* GetVS();
        ID3D11PixelShader* GetPS();

    protected:
        Effect( ID3D11Device* device, const std::wstring& fileName, const D3D_SHADER_MACRO* defines, const std::vector<D3D11_INPUT_ELEMENT_DESC>& inputLayout );
        virtual void UpdateConstantBuffers( ID3D11DeviceContext* context );

    protected:
        ComPtr<ID3D11VertexShader> mVertexShader;
        ComPtr<ID3D11PixelShader> mPixelShader;
        ComPtr<ID3D11InputLayout> mInputLayout;
        std::unordered_map<int, ComPtr<ID3D11Buffer>> mConstantBuffers;
};
