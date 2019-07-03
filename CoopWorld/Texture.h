#pragma once

#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include "D3DUtilities.h"
#include <unordered_map>

class Texture {
    public:
        Texture( const std::wstring& fileName, ID3D11Device* device );
        ~Texture();

        ID3D11ShaderResourceView* GetShaderResourceView();

    private:
        ComPtr<ID3D11ShaderResourceView> mShaderResourceView;
};

class TextureManager {
    public:
        TextureManager( ID3D11Device* device );

        Texture* GetTexture( const std::wstring& file );

    private:
        ID3D11Device* mD3DDevice;
        std::unordered_map<std::wstring, std::unique_ptr<Texture>> mTextureMap;
};

#endif
