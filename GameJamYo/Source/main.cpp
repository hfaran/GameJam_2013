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
#include <iostream>

Player guy;
Image bg;
Flt diff;
Pulse pPulse;
bool coll;
bool theEnd = false;
int collEdge;
int score=0;
int stage=0;
NPC heart;
dropArray drops;
int time;

BackgroundLoader eLogo;
Flt RES_Y =  768.0f;
Flt RES_X =  1366.0f;
int origScore;
int forceRes;

float movementSpeed;//set from ini //1.0f
int JUMPTIME;//set from ini //0.0001
float JUMPSPEED;//set from ini //20
int characterID;//set from ini


const float startSpeed = 0.01f;	//set from ini
int stageCount=0;
int stageTime = 20.0f; //set from ini
Flt speed;
float dropVelocity = 0.005f;//set from ini

Sound BGM;

void InitPre()
{
	App.name("Image");
	Paks.add("_Assets.pak");
	Paks.add("engine.pak");

	FILE* f = fopen("settings.ini", "r");
	fscanf(f, "%f", &RES_X);
	fscanf(f, "%f", &RES_Y);
	fscanf(f, "%d", &origScore);
	fscanf(f, "%d", &forceRes);

	if(forceRes == 1)
		RES_Y = 9.0/16.0*RES_X;

	fclose(f);

	D.mode(RES_X,RES_Y);
}

Bool Init()
{


	bg.load("HeartGame/gfx/background00.gfx"); // load bg
	guy.initPlayer(53,106,movementSpeed,JUMPTIME,JUMPSPEED,11,1.0/10.0,characterID);
	heart.initNPC(1524,700,46,0.1f, stage);
	guy.initBucket(43,25);

	diff = 0.1f;
	speed = .02f;
	pPulse.pSpeed = speed;
	pPulse.initPulse();
	drops.initDropArray(500, stage);

	eLogo.eLogo.load("HeartGame/gfx/logo.gfx");

	eLogo.start(); // create background loader

	return true;
}

void Shut()
{
}

Bool Update()
{
	if(Kb.bp(KB_ESC))return false;
	if(theEnd) { 

	} else {
		switch (stage) {

		case 0:
			if(Round(Time.frame()/60.0f) >= 2) {
				stage++;
				stageCount = 0;
			} else {
				//speed += stage / 40.0f / 10000.0f;
			}
			break;

		case 1:
			if(score <= - 1 * pow(3.14, (stage)) || Round(Time.frame()/60.0f) >=  stageTime) {
				stage++;
				stageCount = 0;
			} else {
				//speed += stage / 40.0f / 10000.0f;
			}
		case 2:
			if(score <= - 1 * pow(3.14, (stage)) || Round(Time.frame()/60.0f) >=  stageTime * 3) {
				stage++;
				stageCount = 0;
			} else {
				//speed += stage / 40.0f / 10000.0f;
			}
		case 3:
			if(score <= - 1 * pow(3.14, (stage)) || Round(Time.frame()/60.0f) >=  stageTime * 7) {
				stage++;
				stageCount = 0;
			} else {
				//speed += stage / 40.0f / 10000.0f;
			}
		case 4:
			if(score <= - 1 * pow(3.14, (stage)) || Round(Time.frame()/60.0f) >=  stageTime * 15) {
				stage++;
				stageCount = 0;
			} else {
				//speed += stage / 40.0f / 10000.0f;
			}
		case 5:
			//speed += stage / 40.0f / 10000.0f;
			break;
		}
		speed = startSpeed+stage/500.0f+stageCount/750000.0f;
		pPulse.pSpeed = speed;
		stageCount++;

		collEdge = checkCollisionEdge(pPulse,guy.collBox);
		score += checkDropsCollision(drops, guy.b_collBox);

		guy.playerUpdate(KB_UP,KB_LEFT,KB_RIGHT, pPulse, collEdge);
		pPulse.updatePulse(drops, stage);
		heart.updateNPC(pPulse, drops, stage, theEnd);

		guy.updateBucket();
		drops.updateDropArray(500, stage, score, dropVelocity);

		if(!BGM.playing()) {
			BGM.play("HeartGame/sound/Emergency Room - OST.ogg", true);
			BGM.volume(0.1f);
		}

		if(guy.gameOver || score <= -50)
			stage = 6;
	}
	return true;
}

void Draw()
{
	if(eLogo.draw()) {
		return;
	} 
	if(theEnd) { 
		D.clear(WHITE);
		TextStyle endGameTs;
		endGameTs.color = BLACK;
		endGameTs.scale*=1.5f;
		D.text(endGameTs,0.0f, 0.05f,S+"Total Score: "+(time + score));
		D.text(endGameTs,0.0f, -0.05f,S+"Press ESC to return to Launcher");

		int finalScore = time + score;

		FILE* f = fopen("settings.ini", "w");
		fprintf(f, "%f\n", RES_X);
		fprintf(f, "%f\n", RES_Y);
		if(finalScore >= origScore) {
			fprintf(f, "%d\n", finalScore);
			D.text(endGameTs,0.0f, 0.3f,S+"YOU BEAT YOUR HIGH SCORE!");
		}
		else
			fprintf(f, "%d\n", origScore);
		fprintf(f, "%d\n", forceRes);

		fclose(f);

	} else {
		D.clear(BLACK);
		bg.draw(Rect((Flt) -RES_X/RES_Y, -1.0f, (Flt) RES_X/RES_Y, 1.0f));
		heart.drawNPC();
		pPulse.drawPulse();
		guy.drawBucket();
		guy.drawPlayer();
		drops.drawDrops();

		D.text(-1.6f,0.96f,S+"FPS: "+Time.fps());
		time = Round(Time.frame()/60.0f);
		D.text(1.5, 0.96f,S+"Time: "+time);
		D.text(0.0f, 0.96f,S+"Blood Loss: "+(-score));
	}




}