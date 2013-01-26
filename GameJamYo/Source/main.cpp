#include "stdafx.h"
#include "Player.h"
#include "main.hpp"
#include "BackgroundLoader.h"
#include "customPhysics.h"
#include "mathFunctions.h"
#include "Wave.h"
#include <iostream>

Player guy;
Image bg;
Flt diff;
Pulse pPulse;
bool coll;
int collEdge;

void InitPre()
{
	App.name("Image");
	Paks.add("_Assets/engine.pak");
	D.mode(RES_X,RES_Y);
}

Bool Init()
{
	bg.load("_Assets/ChipGame/gfx/lololol.gfx"); // load bg

	guy.initPlayer(37.5,86,0.7f,.0001,20,11,1.0/10.0);

	guy.frame[0].load("_Assets/HeartGame/gfx/mofo01.gfx");
	guy.frame[1].load("_Assets/HeartGame/gfx/mofo00.gfx");
	guy.frame[2].load("_Assets/HeartGame/gfx/mofo01.gfx");
	guy.frame[3].load("_Assets/HeartGame/gfx/mofo00.gfx");
	guy.frame[4].load("_Assets/HeartGame/gfx/mofo01.gfx");
	guy.frame[5].load("_Assets/HeartGame/gfx/mofo00.gfx");
	guy.frame[6].load("_Assets/HeartGame/gfx/mofo01.gfx");
	guy.frame[7].load("_Assets/HeartGame/gfx/mofo00.gfx");
	guy.frame[8].load("_Assets/HeartGame/gfx/mofo01.gfx");
	guy.frame[9].load("_Assets/HeartGame/gfx/mofo00.gfx");
	guy.frame[10].load("_Assets/HeartGame/gfx/mofo00.gfx");

	diff = 0.1f;

	pPulse.initPulse();

	return true;
}

void Shut()
{
}

Bool Update()
{
	if(Kb.bp(KB_ESC))return false;

	collEdge = checkCollisionEdge(pPulse,guy.collBox);

	guy.playerUpdate(KB_UP,KB_LEFT,KB_RIGHT, pPulse, collEdge);
	pPulse.updatePulse();

	return true;
}

void Draw()
{
	D.clear(BLACK);
	//bg.draw(Rect((Flt) -RES_X/RES_Y, -1.0f, (Flt) RES_X/RES_Y, 1.0f));
	pPulse.drawPulse();
	guy.drawPlayer();


	
	D.text(-0.9f,0.9f,S+Time.fps());


	
}