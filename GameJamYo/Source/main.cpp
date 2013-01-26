#include "stdafx.h"
#include "Player.h"
#include "main.hpp"
#include "BackgroundLoader.h"
#include "customPhysics.h"
#include "mathFunctions.h"
#include "Wave.h"
#include <iostream>
#include <string>
#include <sstream>

Player guy;
Image bg;
Wave wavey;

void InitPre()
{
	App.name("Image");
	Paks.add("_Assets/engine.pak");
	D.mode(RES_X,RES_Y);
}

Bool Init()
{
	bg.load("_Assets/ChipGame/gfx/lololol.gfx"); // load bg

	wavey.initWave(-1.0f, 0.0f);

	return true;
}

void Shut()
{
}

Bool Update()
{
	if(Kb.bp(KB_ESC))return false;
	return true;
}

void Draw()
{
	D.clear(BLACK);

	// draw image
	//bg.draw(Rect((Flt) -RES_X/RES_Y, -1.0f, (Flt) RES_X/RES_Y, 1.0f));

	wavey.drawWave();
}

