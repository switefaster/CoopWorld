#include "Texture.h"

#include <DirectXTex.h>

Texture::Texture( const std::wstring& fileName, ID3D11Device* device )
{
    TexMetadata info;
    auto image = std::make_unique<ScratchImage>();
    ThrowIfFailed( LoadFromDDSFile( fileName.c_str(), DDS_FLAGS_NONE, &info, *image ) );
    ThrowIfFailed( CreateShaderResourceView( device, image->GetImages(), image->GetImageCount(), info, &mShaderResourceView ) );
}

Texture::~Texture()
{
}

ID3D11ShaderResourceView* Texture::GetShaderResourceView()
{
    return mShaderResourceView.Get();
}

TextureManager::TextureManager( ID3D11Device* device ) :
    mD3DDevice( device )
{
}

Texture* TextureManager::GetTexture( const std::wstring& file )
{
    if ( !( mTextureMap.count( file ) > 0 ) )
    {
        std::unique_ptr<Texture> tex = std::make_unique<Texture>( file, mD3DDevice );
        mTextureMap[file] = std::move( tex );
    }

    return mTextureMap[file].get();
}
