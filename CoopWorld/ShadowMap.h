#pragma once

#ifndef _SHADOW_MAP_H_
#define _SHADOW_MAP_H_

#include "D3DUtilities.h"

class ShadowMap {
    public:
        ShadowMap( ID3D11Device* device, UINT width, UINT height );
		~ShadowMap() = default;
        ShadowMap( const ShadowMap& rhs ) = delete;
        ShadowMap& operator=( const ShadowMap& rhs ) = delete;

        ID3D11ShaderResourceView* DepthMapSRV();

        void BindDsvAndNullRtv( ID3D11DeviceContext* context );

    private:
        UINT mWidth;
        UINT mHeight;

        ComPtr<ID3D11ShaderResourceView> mDepthSrv;
        ComPtr<ID3D11DepthStencilView> mDepthDsv;

        D3D11_VIEWPORT mViewport;
};

#endif
