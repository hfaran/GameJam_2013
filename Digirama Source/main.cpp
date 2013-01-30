#include "stdafx.h"
#include "main.h"
#include "Player.h"
#include "BackgroundLoader.h"
#include "customPhysics.h"
#include "mathFunctions.h"
#include "World.hpp"
#include <iostream>
#include <string>
#include <sstream>

//Use this to enable or disable BGM
#define BGM_ENABLED 0
//Initialize Resolution to something
const float RES_X = 1366.0f;
const float RES_Y = 768.0f;

//Invisible Assets
MusicTheme mtIdle;
// Objects
BackgroundLoader esenthelLogo;
World world;


//---------------------Engine-Functions--------------------------------//

void InitPre()
{
	App.name("Digirama");
	Paks.add("_Assets/engine.pak");
	D.mode(RES_X,RES_Y);
}

Bool Init()
{
	{
	//Load Esenthel Logo for startup
	esenthelLogo.eLogo.load("_Assets/ChipGame/gfx/logo.gfx");
	// Display the Esenthel logo
	esenthelLogo.start(); 
	}

	//Initialize the world
	world.InitWorld(93/1.5, 134/1.5, 1);

	//Play BGM
	if(BGM_ENABLED)
		if(!mtIdle.songs())
			mtIdle+="_Assets/ChipGame/sound/LXTronic.ogg"; 

	return true;
}

void Shut()
{
}

Bool Update()
{
	//If Escape is pressed, quit
	if(Kb.bp(KB_ESC))return false;

	//Update the world
	world.UpdateWorld();
	//Keep the BGM going
	Music.play(mtIdle);

	return true;
}

void Draw()
{
	//Allows the E logo to show
	if(esenthelLogo.draw())return; 

	//Draw the world
	world.DrawWorld();
}



