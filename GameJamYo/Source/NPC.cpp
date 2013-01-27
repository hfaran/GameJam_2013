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

	this->Stage=0;
	this->numStages=6;
	this->nextFrame = 0;

	this->transitionFrameCounter=18;

	this->frame[0].load("_Assets/HeartGame/gfx/beatA00.gfx");
	this->frame[1].load("_Assets/HeartGame/gfx/beatA01.gfx");
	this->frame[2].load("_Assets/HeartGame/gfx/beatA02.gfx");
	this->frame[3].load("_Assets/HeartGame/gfx/beatB00.gfx");
	this->frame[4].load("_Assets/HeartGame/gfx/beatB01.gfx");
	this->frame[5].load("_Assets/HeartGame/gfx/beatB02.gfx");
	this->frame[6].load("_Assets/HeartGame/gfx/beatC00.gfx");
	this->frame[7].load("_Assets/HeartGame/gfx/beatC01.gfx");
	this->frame[8].load("_Assets/HeartGame/gfx/beatC02.gfx");
	this->frame[9].load("_Assets/HeartGame/gfx/beatD00.gfx");
	this->frame[10].load("_Assets/HeartGame/gfx/beatD01.gfx");
	this->frame[11].load("_Assets/HeartGame/gfx/beatD02.gfx");
	this->frame[12].load("_Assets/HeartGame/gfx/beatE00.gfx");
	this->frame[13].load("_Assets/HeartGame/gfx/beatE01.gfx");
	this->frame[14].load("_Assets/HeartGame/gfx/beatE02.gfx");
	this->frame[15].load("_Assets/HeartGame/gfx/beatF00.gfx");
	this->frame[16].load("_Assets/HeartGame/gfx/beatF01.gfx");
	this->frame[17].load("_Assets/HeartGame/gfx/beatF02.gfx");

	this->frame[18].load("_Assets/HeartGame/gfx/beatAT00.gfx");
	this->frame[19].load("_Assets/HeartGame/gfx/beatAT01.gfx");
	this->frame[20].load("_Assets/HeartGame/gfx/beatAT02.gfx");
	this->frame[21].load("_Assets/HeartGame/gfx/beatAT01.gfx");
	this->frame[22].load("_Assets/HeartGame/gfx/beatBT00.gfx");
	this->frame[23].load("_Assets/HeartGame/gfx/beatBT01.gfx");
	this->frame[24].load("_Assets/HeartGame/gfx/beatBT02.gfx");
	this->frame[25].load("_Assets/HeartGame/gfx/beatBT01.gfx");
	this->frame[26].load("_Assets/HeartGame/gfx/beatCT00.gfx");
	this->frame[27].load("_Assets/HeartGame/gfx/beatCT01.gfx");
	this->frame[28].load("_Assets/HeartGame/gfx/beatCT02.gfx");
	this->frame[29].load("_Assets/HeartGame/gfx/beatCT01.gfx");
	this->frame[30].load("_Assets/HeartGame/gfx/beatDT00.gfx");
	this->frame[31].load("_Assets/HeartGame/gfx/beatDT01.gfx");
	this->frame[32].load("_Assets/HeartGame/gfx/beatDT02.gfx");
	this->frame[33].load("_Assets/HeartGame/gfx/beatDT01.gfx");
	this->frame[34].load("_Assets/HeartGame/gfx/beatET00.gfx");
	this->frame[35].load("_Assets/HeartGame/gfx/beatET01.gfx");
	this->frame[36].load("_Assets/HeartGame/gfx/beatET02.gfx");
	this->frame[37].load("_Assets/HeartGame/gfx/beatET01.gfx");
}

void NPC::updateNPC( Pulse & pPulse, dropArray & dArray, int stage )
{
	if(stage > Stage)
	{
			if(this->transitionFrameCounter<18+(Stage)*4)
				this->transitionFrameCounter += animSpeed;
			else {
				//this->transitionFrameCounter=0;
				Stage = stage;
			}
			this->currentFrame = &this->frame[(int) this->transitionFrameCounter];
	}
	else {
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
		this->currentFrame = &this->frame[(int) this->frameCounter + Stage*3];
	}
	/*if(this->frameCounter<=(this->numFrames/numStages)-1 && pPulse.waveys[pPulse.waveCount-1].amplitude != 0.0f)
	this->frameCounter += this->animSpeed;
	else
	this->frameCounter=0;*/



	this->drawBox = Rect(this->x, this->y,0.0f+pixelX/RES_Y/2.0f, 1.0);


}

void NPC::drawNPC( )
{
	currentFrame->draw(drawBox);
	//D.text(0,0,S+(int)frameCounter);
}