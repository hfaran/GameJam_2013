#include "stdafx.h"
#include "Player.h"
#include "main.h"
#include "BackgroundLoader.h"
#include "customPhysics.h"
#include "mathFunctions.h"
#include <iostream>
#include <string>
#include <sstream>

//Use this to enable or disable BGM
#define BGM_ENABLED 0

//Invisible Assets
MusicTheme mtIdle;
// Objects
Image bg; // background image object
Player chip;
Rect ground, platform[3];
BackgroundLoader esenthelLogo;

//---------------------Engine-Functions--------------------------------//

void InitPre()
{
	App.name("Digirama");
	Paks.add("_Assets/engine.pak");
	D.mode(RES_X,RES_Y);
}

Bool Init()
{
	//Load the background
	bg.load("_Assets/ChipGame/gfx/lololol.gfx"); 
	//Initialize Chip, the player
	chip.initPlayer(93, 134);
	//Initialize the platforms in the level
	initPlatform( 1 );

	//Load Esenthel Logo for startup
	esenthelLogo.eLogo.load("_Assets/ChipGame/gfx/logo.gfx");

	// Display the Esenthel logo
	esenthelLogo.start(); 

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

	if(Kb.bp(KB_ESC))return false;

	chip.playerUpdate( KB_UP, KB_LEFT, KB_RIGHT );
	Music.play(mtIdle);

	return true;
}

void Draw()
{
	if(esenthelLogo.draw())return; //Allows the E logo to show

	D.clear(BLACK);

	// draw background
	bg.draw(Rect((Flt) -RES_X/RES_Y, -1.0f, (Flt) RES_X/RES_Y, 1.0f));

	chip.drawPlayer( );
}

//---------------Non-engine Functions-----------------------------//

void initPlatform( int Level )
{
	switch (Level) {
	case 1:
		ground = Rect(convertX(0), convertY(664), convertX(1366), convertY(768));
		platform[0] = Rect(convertX(469), convertY(497), convertX(296), convertY(543));
		platform[1] = Rect(convertX(1366), convertY(357), convertX(1019), convertY(405));
		platform[2] = Rect(convertX(1366), convertY(497), convertX(727), convertY(543));
		break;
	}

}

