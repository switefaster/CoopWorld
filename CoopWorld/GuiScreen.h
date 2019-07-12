#pragma once

#ifndef _GUI_SCREEN_H_
#define _GUI_SCREEN_H_

#include "Gui.h"
#include <functional>

class Button
{
public:
	Button(D2DDrawer* d2ddrawer, std::function<void(float, float, Button*)> render, std::function<void(float, float, Button*)> onclick, float x, float y, float width, float height);

	std::function<void(float, float, Button*)> Render;
	std::function<void(float, float, Button*)> OnClick;

	bool IsMouseInBox(float mouseX, float mouseY);
	void DrawInnerText(const std::wstring& text, const std::wstring& fontFamily, float fontSize, const D2D1_COLOR_F& color);

	D2D1_RECT_F GetSelfRect();

protected:
	D2DDrawer* mD2DDrawer;

	float mX;
	float mY;
	float mWidth;
	float mHeight;
};

class GuiScreen :
	public Gui
{
public:
	GuiScreen(D2DDrawer* d2ddrawer);
	virtual ~GuiScreen() = default;
	virtual void Render(float x, float y) override;
	virtual void OnClick(float x, float y) override;

protected:
	virtual void DrawBackground();
	virtual void DrawForeground() = 0;

	std::vector<std::unique_ptr<Button>> mButtonList;
};

#endif
