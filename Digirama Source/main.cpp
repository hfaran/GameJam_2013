#include "stdafx.h"
#include "Player.h"
#include "main.h"
#include "BackgroundLoader.h"
#include "customPhysics.h"
#include "mathFunctions.h"
#include <iostream>
#include <string>
#include <sstream>

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
	bg.load("_Assets/ChipGame/gfx/lololol.gfx"); // load bg

	chip.initPlayer(93, 134);
	initPlatform( 1 );

	{ //Load frames for chip
		chip.frame[0].load("_Assets/ChipGame/gfx/frame1.gfx");
		chip.frame[1].load("_Assets/ChipGame/gfx/frame2.gfx");
		chip.frame[2].load("_Assets/ChipGame/gfx/frame3.gfx");
		chip.frame[3].load("_Assets/ChipGame/gfx/frame4.gfx");
		chip.frame[4].load("_Assets/ChipGame/gfx/frame5.gfx");
		chip.frame[5].load("_Assets/ChipGame/gfx/frame6.gfx");
		chip.frame[6].load("_Assets/ChipGame/gfx/frame7.gfx");
		chip.frame[7].load("_Assets/ChipGame/gfx/frame8.gfx");
		chip.frame[8].load("_Assets/ChipGame/gfx/frame9.gfx");
		chip.frame[9].load("_Assets/ChipGame/gfx/frame10.gfx"); }

	//Load Esenthel Logo for startup
	esenthelLogo.eLogo.load("_Assets/ChipGame/gfx/logo.gfx");

//	if(!mtIdle.songs())
//		mtIdle+="_Assets/ChipGame/sound/LXTronic.ogg"; 

	esenthelLogo.start(); // create background loader

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

