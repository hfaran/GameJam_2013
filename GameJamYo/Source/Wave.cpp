#include "stdafx.h"
#include "main.hpp"
#include "Wave.h"
#include "mathFunctions.h"

void Wave::initWave( Flt xPosition, Flt yPosition, Flt period, Flt Amp )
{
	xPos=xPosition;
	yPos = yPosition;
	t=period;
	amplitude=Amp;

	this->beat[0] = Edge2(this->xPos, this->yPos, 
		this->xPos+this->t/4.0, this->yPos+this->amplitude);
	this->beat[1] = Edge2(this->xPos+this->t/4.0, this->yPos+this->amplitude,
		this->xPos+this->t/2.0, this->yPos+this->amplitude*-1);
	this->beat[2] = Edge2(this->xPos+this->t/2.0, this->yPos+this->amplitude*-1,
		this->xPos+this->t/4.0*3.0, this->yPos+this->amplitude*0.33333);
	this->beat[3] = Edge2(this->xPos+this->t/4.0*3.0, this->yPos+this->amplitude*0.33333, this->xPos+this->t/4.0*3.5, this->yPos+this->amplitude*-0.33333);
	this->beat[4] = Edge2(this->xPos+this->t/4.0*3.5, this->yPos+this->amplitude*-0.33333, this->xPos+t, this->yPos);


}

void Wave::updateWave( Flt period, Flt Amp )
{
	t=period;
	amplitude=Amp;

	this->beat[0] = Edge2(this->xPos, this->yPos, 
		this->xPos+this->t/4.0, this->yPos+this->amplitude);
	this->beat[1] = Edge2(this->xPos+this->t/4.0, this->yPos+this->amplitude,
		this->xPos+this->t/2.0, this->yPos+this->amplitude*-1);
	this->beat[2] = Edge2(this->xPos+this->t/2.0, this->yPos+this->amplitude*-1,
		this->xPos+this->t/4.0*3.0, this->yPos+this->amplitude*0.33333);
	this->beat[3] = Edge2(this->xPos+this->t/4.0*3.0, this->yPos+this->amplitude*0.33333, this->xPos+this->t/4.0*3.5, this->yPos+this->amplitude*-0.33333);
	this->beat[4] = Edge2(this->xPos+this->t/4.0*3.5, this->yPos+this->amplitude*-0.33333, this->xPos+t, this->yPos);


}


void Wave::drawWave( )
{
	for( int i=0; i<5; i++ )
		this->beat[i].draw(RED);
}

void Wave::copyWave(Wave & waveB) {

	for(int i=0;i<5;i++){
		this->beat[i] = waveB.beat[i];
	}

	this->t = waveB.t;
	this->amplitude = waveB.amplitude;
	this->yPos = waveB.yPos;
	this->xPos = waveB.xPos;
}

void Pulse::updatePulse() {

	waveys[0].xPos -= pSpeed;

	if(waveys[0].xPos + 0.2f < -RES_X / RES_Y) {
		for(int i=0;i<19;i++){
			waveys[i].copyWave(waveys[i+1]);
		}
		for(int i=0;i<20;i++){
			waveys[i].xPos -= pSpeed;
		}
		waveys[19].initWave(-RES_X / RES_Y + 0.2f * 19, 0.0f, 0.2f, 0.4f);
	} else {
		for(int i=1;i<20;i++){
			waveys[i].xPos -= pSpeed;
		}
	}
	for(int i=1;i<20;i++){
		waveys[i].updateWave(0.2f, 0.2f);
	}
}

void Pulse::drawPulse() {
	for(int i=0;i<20;i++){
		waveys[i].drawWave();
	}
}

void Pulse::initPulse() {
	flatNext = false;
	pSpeed = .01f;

	for(int i=0;i<20;i++){
		waveys[i].initWave(-RES_X / RES_Y + 0.2f * i, 0.0f, 0.2f, 0.0f);
	}

}