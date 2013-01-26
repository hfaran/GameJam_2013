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