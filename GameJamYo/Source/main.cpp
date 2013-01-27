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
#include <cmath>

Player guy;
Image bg;
Flt diff;
Pulse pPulse;
bool coll;
int collEdge;
int score=0;
int stage=0;
NPC heart;
dropArray drops;
Flt speed;

Sound BGM;

void InitPre()
{
	App.name("Image");
	Paks.add("_Assets/engine.pak");
	D.mode(RES_X,RES_Y);
}

Bool Init()
{
	bg.load("_Assets/HeartGame/gfx/background00.gfx"); // load bg

	guy.initPlayer(53,106,1.2f,.0001,20,11,1.0/10.0);
	heart.initNPC(1524,700,38,0.1f, stage);
	guy.initBucket(43,25);

	diff = 0.1f;
	speed = .02f;
	pPulse.pSpeed = speed;
	pPulse.initPulse();
	drops.initDropArray(500, stage);

	return true;
}

void Shut()
{
}

Bool Update()
{
	if(Kb.bp(KB_ESC))return false;

	switch (stage) {

	case 0:
		if(Round(Time.frame()/60.0f) >= 2) {
			stage++;
			speed = .02f;
		} else {
			speed += stage / 40.0f / 10000.0f;
		}
		break;

	case 1:
	case 2:
	case 3:
	case 4:
		if(score <= - 1 * pow(3.14, (stage)) || Round(Time.frame()/60.0f) >= stage * 60.0f) {
			stage++;
			speed = .02f;
		} else {
			speed += stage / 40.0f / 10000.0f;
		}
	case 5:
		speed += stage / 40.0f / 10000.0f;
		break;
	}
	pPulse.pSpeed = speed;

	collEdge = checkCollisionEdge(pPulse,guy.collBox);
	score += checkDropsCollision(drops, guy.b_collBox);

	guy.playerUpdate(KB_UP,KB_LEFT,KB_RIGHT, pPulse, collEdge);
	pPulse.updatePulse(drops, stage);
	heart.updateNPC(pPulse, drops, stage);

	guy.updateBucket();
	drops.updateDropArray(500, stage, score);

	BGM.speed(50.0f*speed);
	BGM.volume(0.1f);
	if(!BGM.playing())
		BGM.play("_Assets/HeartGame/sound/Emergency Room - OST.ogg", true);

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
	D.text(1.5, 0.96f,S+"Time: "+Round(Time.frame()/60.0f));
	D.text(0.0f, 0.96f,S+"Score: "+score);




}