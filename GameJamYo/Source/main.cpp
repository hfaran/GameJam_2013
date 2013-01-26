#include "stdafx.h"
#include "Player.h"
#include "main.hpp"
#include "BackgroundLoader.h"
#include "customPhysics.h"
#include "mathFunctions.h"
#include "Wave.h"
#include "NPC.hpp"
#include <iostream>

Player guy;
Image bg;
Flt diff;
Pulse pPulse;
bool coll;
int collEdge;
int score=0;
NPC heart;

void InitPre()
{
	App.name("Image");
	Paks.add("_Assets/engine.pak");
	D.mode(RES_X,RES_Y);
}

Bool Init()
{
	//bg.load("_Assets/ChipGame/gfx/lololol.gfx"); // load bg

	guy.initPlayer(53,106,0.7f,.0001,20,11,1.0/10.0);
	heart.initNPC(1600,580,12,1.0/10.0);
	guy.initBucket(43,25);


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

	heart.updateNPC( );
	guy.playerUpdate(KB_UP,KB_LEFT,KB_RIGHT, pPulse, collEdge);
	pPulse.updatePulse();
	guy.updateBucket();

	return true;
}

void Draw()
{
	

	D.clear(BLACK);
	//bg.draw(Rect((Flt) -RES_X/RES_Y, -1.0f, (Flt) RES_X/RES_Y, 1.0f));
	heart.drawNPC();
	pPulse.drawPulse();
	guy.drawBucket();
	guy.drawPlayer();


	if(guy.gameOver)
		D.text(0,0,S+"GAME OVER!");

	D.text(-1.6f,0.96f,S+"FPS: "+Time.fps());
	D.text(1.5, 0.96f,S+"kiloFrames: "+Time.frame()/1000.0f);
	D.text(0.0f, 0.96f,S+"Score: "+score);




}