#include "FontManager.h"



FontManager::FontManager( ID3D11Device* device ) :
    mD3DDevice( device )
{
    ThrowIfFailed( FW1CreateFactory( FW1_VERSION, &mFW1Factory ) );
}

FontManager::~FontManager()
{
}

void FontManager::DrawFont( ID3D11DeviceContext* context, const std::wstring& font, const std::wstring& text, float size, float x, float y, UINT32 color, UINT flags )
{
    IFW1FontWrapper* wrapper;

    if ( mWrappers.count( font ) <= 0 )
    {
        ComPtr<IFW1FontWrapper> newWrapper;
        mFW1Factory->CreateFontWrapper( mD3DDevice, font.c_str(), &newWrapper );
        wrapper = newWrapper.Get();
        mWrappers[font] = wrapper;
    }
    else
    {
        wrapper = mWrappers[font].Get();
    }

    wrapper->DrawString( context, text.c_str(), size, x, y, color, flags );
}
