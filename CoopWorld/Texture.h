#pragma once

#include "D3DUtilities.h"

class Texture {
    public:
        Texture( const std::wstring& fileName, ID3D11Device* device );
        ~Texture();

        ID3D11ShaderResourceView* GetShaderResourceView();

    private:
        ComPtr<ID3D11ShaderResourceView> mShaderResourceView;
};

