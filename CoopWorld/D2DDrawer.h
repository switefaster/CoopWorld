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

	void DrawText(const std::wstring& fontFamily, const D2D1_RECT_F& layout, const D2D1_COLOR_F& color, float fontSize, const std::wstring& content);
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
