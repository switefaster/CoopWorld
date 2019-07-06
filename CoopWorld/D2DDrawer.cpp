#include "D2DDrawer.h"

D2DDrawer::D2DDrawer(ID2D1Factory* factory, ID2D1RenderTarget* renderTarget) :
	mD2DFactory(factory),
	mD2DRenderTarget(renderTarget)
{
	ThrowIfFailed(DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(mDWFactory.ReleaseAndGetAddressOf())));
}

void D2DDrawer::ResizedResources(ID2D1RenderTarget* renderTarget)
{
	mD2DRenderTarget = renderTarget;
}

void D2DDrawer::DrawText(const std::wstring& fontFamily, const D2D1_RECT_F& layout, const D2D1_COLOR_F& color, float fontSize, const std::wstring& content)
{
	mD2DRenderTarget->BeginDraw();
	ComPtr<IDWriteTextFormat> textFormat;
	ThrowIfFailed(mDWFactory->CreateTextFormat(fontFamily.c_str(), nullptr, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, fontSize, L"en-US", &textFormat));
	mD2DRenderTarget->DrawTextW(content.c_str(), content.length(), textFormat.Get(), layout, GetSolidColorBrush(color));
	ThrowIfFailed(mD2DRenderTarget->EndDraw());
}

void D2DDrawer::FillRectangle(const D2D1_RECT_F& rect, const D2D1_COLOR_F& color)
{
	mD2DRenderTarget->BeginDraw();
	mD2DRenderTarget->FillRectangle(rect, GetSolidColorBrush(color));
	ThrowIfFailed(mD2DRenderTarget->EndDraw());
}

ID2D1SolidColorBrush* D2DDrawer::GetSolidColorBrush(const D2D1_COLOR_F& color)
{
	if (!mSolidColorBrush)
	{
		ThrowIfFailed(mD2DRenderTarget->CreateSolidColorBrush(color, &mSolidColorBrush));
	}
	else
	{
		mSolidColorBrush->SetColor(color);
	}
	return mSolidColorBrush.Get();
}
