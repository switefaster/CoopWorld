#pragma once

#ifndef _BASIC_EFFECT_H_
#define _BASIC_EFFECT_H_

#include "Effect.h"
#include "Mesh.h"

struct DirectionalLight
{
    XMFLOAT4 Ambient;
    XMFLOAT4 Diffuse;
    XMFLOAT4 Specular;
    XMFLOAT3 Direction;
    float pad;
};

struct PointLight
{
    XMFLOAT4 Ambient;
    XMFLOAT4 Diffuse;
    XMFLOAT4 Specular;

    XMFLOAT3 Position;
    float Range;

    XMFLOAT3 Att;
    float pad;
};

struct SpotLight
{
    XMFLOAT4 Ambient;
    XMFLOAT4 Diffuse;
    XMFLOAT4 Specular;

    XMFLOAT3 Position;
    float Range;

    XMFLOAT3 Direction;
    float Spot;

    XMFLOAT3 Att;
    float pad;
};

/*
	pair<CB, Dirty>
*/
template<typename T>
using CBWrapper = std::pair<T, bool>;

template<typename T>
inline void SetDirty( CBWrapper<T>& wrapper, bool dirty )
{
    wrapper.second = dirty;
}

class BasicEffect : public Effect {
    public:
        static std::vector<D3D11_INPUT_ELEMENT_DESC> InputLayout;
        struct Vertex
        {
            XMFLOAT3 Pos;
            XMFLOAT3 Norm;
            XMFLOAT2 Tex;
        };
        struct CBFrame
        {
            DirectionalLight DirectionalLights[3];
            XMFLOAT4X4 ShadowTransform;
            XMFLOAT3 EyePos;
            float FogRange;
            float FogStart;
            XMFLOAT3 FogColor;
            XMFLOAT3 LightCount;
            float pad;
        };
        struct CBObject
        {
            XMFLOAT4X4 World = Identity4x4;
            XMFLOAT4X4 WorldInvTranspose = Identity4x4;
            XMFLOAT4X4 WorldViewProj = Identity4x4;
            XMFLOAT4X4 TexTransform = Identity4x4;
            Material Material;
        };
    public:
        BasicEffect( ID3D11Device* device );

        void SetWorld( FXMMATRIX M );
        void SetWorldInvTranspose( FXMMATRIX M );
        void SetWorldViewProj( FXMMATRIX M );
        void SetTexTransform( FXMMATRIX M );
        void SetMaterial( const Material& mat );

        void SetLights( DirectionalLight lights[3], UINT count );
        void SetShadowTransform( FXMMATRIX M );
        void SetEyePos( const XMFLOAT3& pos );
        void SetFogRange( float range );
        void SetFogStart( float start );
        void SetFogColor( const XMFLOAT3& color );

        void SetShadowMap( ID3D11ShaderResourceView* srv );
        void SetShaderResourceView( ID3D11ShaderResourceView* srv );

    protected:
        void UpdateConstantBuffers( ID3D11DeviceContext* context ) override;

    private:
        CBWrapper<CBFrame> mCBFrame;
        CBWrapper<CBObject> mCBObject;
        ID3D11ShaderResourceView* mShadowMap;
        ID3D11ShaderResourceView* mTextureView;
};

#endif
