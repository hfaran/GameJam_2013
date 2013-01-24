#include "stdafx.h"
#include "Player.h"
#include <iostream>
#include <string>
#include <sstream>

#define LOAD_TIME 2000 //The time the esenthel logo (and other logos) show in ms

Image eLogo; //esenthel logo

struct BackgroundLoader
{
   // static functions
   static Bool BackgroundLoad(Thread &thread) // this function will be called in the secondary thread to load the initial world data
   {
	  Time.wait(LOAD_TIME); // wait 2 seconds just for the needs of tutorial to make the loading screen not disappear too quickly
	  return false; // don't continue the thread
   }

   // members
   Thread thread;

   // methods
   void del() // delete background loader
   {
	  thread.del();
   }
   void start() // start background loader
   {
	  thread.create(BackgroundLoad); // start loader thread
   }
   Bool update() // update background loader
   {
	  if(!thread.created())return false; // not loading anything
	  customUpdate(); // call custom updating method
	  return true; // loading
   }
   Bool draw() // draw background loader
   {
	  if(!thread.created())return false; // not loading anything
	  customDraw(); // call custom drawing method
	  if(!thread.active()) // thread is created but finished processing
	  {
		 thread.del(); // delete the thread
		 Renderer.setFade(1.0f); // enable screen fading from loading screen to the game
	  }
	  return true; // loading or finishing
   }

   void customUpdate() // you can modify this method and perform custom updating of the loading screen
   {
	  Time.wait(1000/24); // limit main thread speed to 24 fps, to give more cpu power for background thread
   }
   void customDraw() // you can modify this method and perform custom drawing of the loading screen
   {
		D.clear(BLACK);
		eLogo.draw(Rect((Flt) -RES_X/RES_Y, -1.0f, (Flt) RES_X/RES_Y, 1.0f));
   }
}BL;

// Function prototypes
Flt convertY(Flt y);
Flt convertX(Flt x);
void initPlatform( );
bool collision(Rect A, Rect B);
bool onTop(Rect A, Rect B);

//Invisible Assets
MusicTheme mtIdle;

// Objects
Image bg; // background image object
Player chip;
Rect ground, platform[3];

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
	eLogo.load("_Assets/ChipGame/gfx/logo.gfx");


	if(!mtIdle.songs()){
	  mtIdle+="_Assets/ChipGame/sound/LXTronic.ogg"; 
	}

	chip.initPlayer(93, 134);
	initPlatform();

	BL.start(); // create background loader

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
	if(BL.draw())return; //Allows the E logo to show

	D.clear(BLACK);

	// draw background
	bg.draw(Rect((Flt) -RES_X/RES_Y, -1.0f, (Flt) RES_X/RES_Y, 1.0f));

	chip.drawPlayer( );
}

//---------------Non-engine Functions-----------------------------//

bool collision(Rect A, Rect B)
{
	if(A.max.y>B.max.y && A.min.y<B.max.y)
		if(A.max.x>B.max.x && A.min.x<B.max.x)
			return true;
	if(A.max.y>B.min.y && A.min.y<B.min.y)
		if(A.max.x>B.min.x && A.min.x<B.min.x)
			return true;
	if(A.max.y>B.min.y && A.min.y<B.min.y)
		if(A.max.x>B.max.x && A.min.x<B.max.x)
			return true;
	if(A.max.y>B.max.y && A.min.y<B.max.y)
		if(A.max.x>B.min.x && A.min.x<B.min.x)
			return true;

	return false;
}

bool onTop(Rect A, Rect B)
{
	if(B.max.y>A.max.y)
		return true;
	else
		return false;
}

Flt convertY(Flt y)
{
	return (384.0-y)/384.0;
}

Flt convertX(Flt x)
{
	return (x-683.0)/384.0;
}