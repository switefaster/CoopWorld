#pragma once

#ifndef _EFFECT_H_
#define _EFFECT_H_

#include "D3DUtilities.h"
#include <vector>
#include <unordered_map>
#include <memory>
#include <boost/functional/hash.hpp>

struct IL_VECTOR_HASH
{
    std::size_t operator()( const std::vector<D3D11_INPUT_ELEMENT_DESC>& v ) const
    {
        std::size_t hash = 0;

        for ( std::size_t i = 0; i < v.size(); i++ )
        {
            boost::hash_combine( hash, boost::hash_value( i ) );
            boost::hash_combine( hash, boost::hash_value( v[i].SemanticName ) );
            boost::hash_combine( hash, boost::hash_value( v[i].AlignedByteOffset ) );
        }

        return hash;
    }
};

class Effect {
    private:
        static std::unordered_map<std::vector<D3D11_INPUT_ELEMENT_DESC>, ComPtr<ID3D11InputLayout>, IL_VECTOR_HASH> InputLayoutCache;
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
        ID3D11InputLayout* mInputLayout;
        std::unordered_map<int, ComPtr<ID3D11Buffer>> mConstantBuffers;
};

#endif
