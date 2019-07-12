#include "DemoGuiScreen.h"

#include "D3DApplication.h"

DemoGuiScreen::DemoGuiScreen(D2DDrawer* d2ddrawer) :
	GuiScreen(d2ddrawer)
{
	mButtonList.emplace_back(std::make_unique<Button>(mD2DDrawer,
		[&](float x, float y, Button* self) {
			if (self->IsMouseInBox(x, y))
			{
				mD2DDrawer->FillRectangle(self->GetSelfRect(), D2D1::ColorF(0.4f, 0.4f, 0.4f, 1.0f));
			}
			else
			{
				mD2DDrawer->FillRectangle(self->GetSelfRect(), D2D1::ColorF(1.0f, 1.0f, 1.0f, 1.0f));
			}
			self->DrawInnerText(L"Click Me!", L"Arial", 25, D2D1::ColorF(D2D1::ColorF::Black));
		},
		[](float x, float y, Button* self) {
			if (self->IsMouseInBox(x, y))
			{
				MessageBox(nullptr, L"You clicked me!", L"Button", MB_OK);
			}
		}, 100, 100, 150, 70));
}

void DemoGuiScreen::DrawForeground()
{
	D2D1_RECT_F rect = D3DApplication::GetInstance()->GetScreenRect();
	mD2DDrawer->DrawText(L"Arial", D2D1::ColorF(1.0f, 1.0f, 1.0f, 1.0f), rect, L"≤‚ ‘", 36);
}
