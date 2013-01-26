#include "stdafx.h"
#include "main.hpp"
#include "NPC.hpp"

void NPC::initNPC( int pX, int pY, int nFrames, Flt animSpd )
{
	this->x = -RES_X/RES_Y * 0.9f; //Start at the ~edge of the screen
	this->y = 1.0f-pY/RES_Y;

	this->pixelX = pX;
	this->pixelY = pY;
	this->numFrames = nFrames;

	this->frame = new Image [numFrames];
	this->animSpeed = animSpd;

	this->Stage=0;
	this->numStages=4;

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
}

void NPC::updateNPC( /*int score, int time*/ )
{
	if(this->frameCounter<(this->numFrames/numStages)-1)
				this->frameCounter += this->animSpeed;
			else
				this->frameCounter=0;

	this->currentFrame = &this->frame[(int) this->frameCounter + Stage*3];

	this->drawBox = Rect(this->x, this->y,0.9f*RES_X/RES_Y, 1.0);


}

void NPC::drawNPC( )
{
	currentFrame->draw(drawBox);
}