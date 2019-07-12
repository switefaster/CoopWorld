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
		v->Render(x, y, v.get());
	}
	DrawForeground();
}

void GuiScreen::OnClick(float x, float y)
{
	for (auto& v : mButtonList)
	{
		v->OnClick(x, y, v.get());
	}
}

void GuiScreen::DrawBackground()
{
	mD2DDrawer->FillRectangle(D3DApplication::GetInstance()->GetScreenRect(), D2D1::ColorF(1.0f, 1.0f, 1.0f, 0.2f));
}

Button::Button(D2DDrawer* d2ddrawer, std::function<void(float, float, Button*)> render, std::function<void(float, float, Button*)> onclick, float x, float y, float width, float height) :
	mD2DDrawer(d2ddrawer),
	Render(render),
	OnClick(onclick),
	mX(x),
	mY(y),
	mWidth(width),
	mHeight(height)
{
}

bool Button::IsMouseInBox(float mouseX, float mouseY)
{
	return mouseX >= mX && mouseY >= mY && mouseX <= (mX + mWidth) && mouseY <= (mY + mHeight);
}

void Button::DrawInnerText(const std::wstring& text, const std::wstring& fontFamily, float fontSize, const D2D1_COLOR_F& color)
{
	mD2DDrawer->DrawText(fontFamily, color, GetSelfRect(), text, fontSize);
}

D2D1_RECT_F Button::GetSelfRect()
{
	return D2D1::RectF(mX, mY, mX + mWidth, mY + mHeight);
}
