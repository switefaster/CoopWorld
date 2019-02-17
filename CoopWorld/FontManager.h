#pragma once

#include "D3DUtilities.h"
#include <FW1FontWrapper.h>
#include <unordered_map>

class FontManager {
    public:
        FontManager( ID3D11Device* device );
        ~FontManager();

        void DrawFont( ID3D11DeviceContext* context, const std::wstring& font, const std::wstring& text, float size, float x, float y, UINT32 color, UINT flags = FW1_RESTORESTATE);

    private:
		ID3D11Device* mD3DDevice;
        ComPtr<IFW1Factory> mFW1Factory;
        std::unordered_map<std::wstring, ComPtr<IFW1FontWrapper>> mWrappers;
};

