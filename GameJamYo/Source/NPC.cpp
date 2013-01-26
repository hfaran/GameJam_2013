#include "stdafx.h"
#include "main.hpp"
#include "NPC.hpp"
#include "Wave.h"

void NPC::initNPC( int pX, int pY, int nFrames, Flt animSpd, int stage )
{
	this->x = 0.0f-pX/RES_Y/2.0f; //Start at the ~edge of the screen
	this->y = 1.0f-pY/RES_Y;

	this->pixelX = pX;
	this->pixelY = pY;
	this->numFrames = nFrames;

	this->frame = new Image [numFrames];
	this->animSpeed = animSpd;

	this->Stage=stage;
	this->numStages=6;
	this->nextFrame = 0;

	this->frame[0].load("_Assets/HeartGame/gfx/heart00.gfx");
	this->frame[1].load("_Assets/HeartGame/gfx/heart01.gfx");
	this->frame[2].load("_Assets/HeartGame/gfx/heart02.gfx");
	this->frame[3].load("_Assets/HeartGame/gfx/heart00.gfx");
	this->frame[4].load("_Assets/HeartGame/gfx/heart01.gfx");
	this->frame[5].load("_Assets/HeartGame/gfx/heart02.gfx");
	this->frame[6].load("_Assets/HeartGame/gfx/heart00.gfx");
	this->frame[7].load("_Assets/HeartGame/gfx/heart01.gfx");
	this->frame[8].load("_Assets/HeartGame/gfx/heart02.gfx");
	this->frame[9].load("_Assets/HeartGame/gfx/heart00.gfx");
	this->frame[10].load("_Assets/HeartGame/gfx/heart01.gfx");
	this->frame[11].load("_Assets/HeartGame/gfx/heart02.gfx");
	this->frame[12].load("_Assets/HeartGame/gfx/heart00.gfx");
	this->frame[13].load("_Assets/HeartGame/gfx/heart01.gfx");
	this->frame[14].load("_Assets/HeartGame/gfx/heart02.gfx");
	this->frame[15].load("_Assets/HeartGame/gfx/heart00.gfx");
	this->frame[16].load("_Assets/HeartGame/gfx/heart01.gfx");
	this->frame[17].load("_Assets/HeartGame/gfx/heart02.gfx");
}

void NPC::updateNPC( Pulse & pPulse, dropArray & dArray )
{
	if(pPulse.waveys[pPulse.waveCount-3].amplitude == 0.0f){
		nextFrame = 0;
	} else if(pPulse.waveys[pPulse.waveCount-3].xPos+pPulse.waveys[pPulse.waveCount-3].t < RES_X/RES_Y){
		nextFrame = 0;
	} else if(pPulse.waveys[pPulse.waveCount-3].xPos+pPulse.waveys[pPulse.waveCount-3].t/4.0*3.0 < RES_X/RES_Y){
		nextFrame = 1;
	} else if(pPulse.waveys[pPulse.waveCount-3].xPos+pPulse.waveys[pPulse.waveCount-3].t/2.0f < RES_X/RES_Y){
		nextFrame = 2;
	} else if(pPulse.waveys[pPulse.waveCount-3].xPos < RES_X/RES_Y){
		nextFrame = 1;
	}

	frameCounter = nextFrame;

	/*if(this->frameCounter<=(this->numFrames/numStages)-1 && pPulse.waveys[pPulse.waveCount-1].amplitude != 0.0f)
		this->frameCounter += this->animSpeed;
	else
		this->frameCounter=0;*/

	this->currentFrame = &this->frame[(int) this->frameCounter + Stage*3];

	this->drawBox = Rect(this->x, this->y,0.0f+pixelX/RES_Y/2.0f, 1.0);


}

void NPC::drawNPC( )
{
	currentFrame->draw(drawBox);
	//D.text(0,0,S+(int)frameCounter);
}