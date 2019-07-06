#include "DemoGuiScreen.h"

DemoGuiScreen::DemoGuiScreen(D2DDrawer* d2ddrawer) :
	GuiScreen(d2ddrawer)
{
	mButtonList.emplace_back(std::make_unique<Button>(mD2DDrawer,
		[&](float x, float y) {
			if (x >= 0 && y >= 0 && x <= 100 && y <= 100)
			{
				mD2DDrawer->FillRectangle(D2D1::RectF(0.0f, 0.0f, 100.0f, 100.0f), D2D1::ColorF(0.4f, 0.4f, 0.4f, 1.0f));
			}
			else
			{
				mD2DDrawer->FillRectangle(D2D1::RectF(0.0f, 0.0f, 100.0f, 100.0f), D2D1::ColorF(1.0f, 1.0f, 1.0f, 1.0f));
			}
			mD2DDrawer->DrawText(L"Arial", D2D1::RectF(0.0f, 0.0f, 100.0f, 100.0f), D2D1::ColorF(D2D1::ColorF::Black), 10, L"Button");
		},
		[](float x, float y) {
			if (x >= 0 && y >= 0 && x <= 100 && y <= 100)
			{
				MessageBox(nullptr, L"You clicked me!", L"Button", MB_OK);
			}
		}));
}

void DemoGuiScreen::DrawForeground()
{
}
