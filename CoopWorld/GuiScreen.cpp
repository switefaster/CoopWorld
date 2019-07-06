#include "GuiScreen.h"

#include "D3DApplication.h"

GuiScreen::GuiScreen(D2DDrawer* d2ddrawer) :
	Gui(d2ddrawer)
{
}

void GuiScreen::Render(float x, float y)
{
	DrawBackground();
	for (auto& v : mButtonList)
	{
		v->Render(x, y);
	}
	DrawForeground();
}

void GuiScreen::OnClick(float x, float y)
{
	for (auto& v : mButtonList)
	{
		v->OnClick(x, y);
	}
}

void GuiScreen::DrawBackground()
{
	mD2DDrawer->FillRectangle(D3DApplication::GetInstance()->GetScreenRect(), D2D1::ColorF(1.0f, 1.0f, 1.0f, 0.2f));
}

Button::Button(D2DDrawer* d2ddrawer, std::function<void(float, float)> render, std::function<void(float, float)> onclick) :
	mD2DDrawer(d2ddrawer),
	Render(render),
	OnClick(onclick)
{
}
