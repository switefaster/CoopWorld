#pragma once
#include "GuiScreen.h"
class DemoGuiScreen :
	public GuiScreen
{
public:
	DemoGuiScreen(D2DDrawer* d2ddrawer);
private:
	virtual void DrawForeground() override;
};

