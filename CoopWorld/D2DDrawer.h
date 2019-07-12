#pragma once

#ifndef _D2D_DRAWER_H_
#define _D2D_DRAWER_H_

#include "D3DUtilities.h"

#ifdef DrawText
#undef DrawText
#endif

class D2DDrawer
{
public:
	D2DDrawer(ID2D1Factory* factory, ID2D1RenderTarget* renderTarget);

	void ResizedResources(ID2D1RenderTarget* renderTarget);

	void DrawText(const std::wstring& fontFamily, const D2D1_COLOR_F& color, const D2D1_RECT_F& layout, const std::wstring& content, float fontSize, DWRITE_FONT_WEIGHT weight = DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE style = DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH stretch = DWRITE_FONT_STRETCH_NORMAL);
	void FillRectangle(const D2D1_RECT_F& rect, const D2D1_COLOR_F& color);

private:
	ID2D1SolidColorBrush* GetSolidColorBrush(const D2D1_COLOR_F& color);

private:
	ComPtr<IDWriteFactory> mDWFactory;
	ID2D1Factory* mD2DFactory = nullptr;
	ID2D1RenderTarget* mD2DRenderTarget = nullptr;

	ComPtr<ID2D1SolidColorBrush> mSolidColorBrush;
};

#endif
