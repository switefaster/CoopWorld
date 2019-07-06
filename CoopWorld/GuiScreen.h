#pragma once

#ifndef _GUI_SCREEN_H_
#define _GUI_SCREEN_H_

#include "Gui.h"
#include <functional>

class Button
{
public:
	Button(D2DDrawer* d2ddrawer, std::function<void(float, float)> render, std::function<void(float, float)> onclick);

	std::function<void(float, float)> Render;
	std::function<void(float, float)> OnClick;

private:
	D2DDrawer* mD2DDrawer;
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
