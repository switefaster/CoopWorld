#pragma once

#ifndef _GUI_H_
#define _GUI_H_

#include "D3DUtilities.h"
#include "D2DDrawer.h"

class Gui
{
public:
	Gui(D2DDrawer* d2ddrawer);
	virtual ~Gui();

	virtual void Render(float x, float y) = 0;
	virtual void OnClick(float x, float y) = 0;

protected:
	D2DDrawer* mD2DDrawer;
};

#endif
