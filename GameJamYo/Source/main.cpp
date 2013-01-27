#include "stdafx.h"
#include "Player.h"
#include "main.hpp"
#include "BackgroundLoader.h"
#include "customPhysics.h"
#include "mathFunctions.h"
#include "Wave.h"
#include "dropArray.hpp"
#include "NPC.hpp"
#include <iostream>

Player guy;
Image bg;
Flt diff;
Pulse pPulse;
bool coll;
int collEdge;
int score=0;
int stage=1;
NPC heart;
dropArray drops;
Flt speed;

void InitPre()
{
	App.name("Image");
	Paks.add("_Assets/engine.pak");
	D.mode(RES_X,RES_Y);
}

Bool Init()
{
	bg.load("_Assets/HeartGame/gfx/background00.gfx"); // load bg

	guy.initPlayer(53,106,0.7f,.0001,20,11,1.0/10.0);
	heart.initNPC(1524,700,42,0.1f, stage);
	guy.initBucket(43,25);

	diff = 0.1f;
	speed = .02f;
	pPulse.initPulse();
	pPulse.pSpeed = speed;
	drops.initDropArray(500, stage);

	return true;
}

void Shut()
{
}

Bool Update()
{
	if(Kb.bp(KB_ESC))return false;

	if (stage != 5)
	{
		if( stage != 0)
			stage=(int) score/10;
		else
			stage = (int) Time.frame()/300.0f;
	}

	collEdge = checkCollisionEdge(pPulse,guy.collBox);
	score += checkDropsCollision(drops, guy.b_collBox);

	guy.playerUpdate(KB_UP,KB_LEFT,KB_RIGHT, pPulse, collEdge);
	pPulse.updatePulse(drops);
	heart.updateNPC(pPulse, drops, stage);



	guy.updateBucket();
	drops.updateDropArray(500, stage);

	return true;
}

void Draw()
{


	D.clear(BLACK);
	bg.draw(Rect((Flt) -RES_X/RES_Y, -1.0f, (Flt) RES_X/RES_Y, 1.0f));
	heart.drawNPC();
	pPulse.drawPulse();
	guy.drawBucket();
	guy.drawPlayer();
	drops.drawDrops();

	if(guy.gameOver)
		D.text(0,0,S+"GAME OVER!");

	D.text(-1.6f,0.96f,S+"FPS: "+Time.fps());
	D.text(1.5, 0.96f,S+"kiloFrames: "+Time.frame()/1000.0f);
	D.text(0.0f, 0.96f,S+"Score: "+score);




}