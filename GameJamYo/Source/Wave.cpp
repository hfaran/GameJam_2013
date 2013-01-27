#include "stdafx.h"
#include "main.hpp"
#include "Wave.h"
#include "mathFunctions.h"
#include <cstdlib>
#include <cmath>

#define Tt 0.5f
#define PULSE_SPEED .005f

void Wave::initWave( Flt xPosition, Flt period, Flt Amp )
{
	xPos=xPosition;
	yPos = -0.5f;
	t=period;
	amplitude=Amp;

	beat[0] = Edge2(xPos, yPos, xPos+t/4.0, yPos+amplitude);
	beatCol[0] = Tri2(Vec2(xPos,yPos),Vec2(xPos+t/4.0, yPos+amplitude), Vec2(xPos+t/4.0, yPos));
	beat[1] = Edge2(xPos+t/4.0, yPos+amplitude, xPos+t/2.0, yPos+amplitude*-1);
	beatCol[1] = Tri2(Vec2(xPos+t/4.0, yPos+amplitude),Vec2(xPos+t/2.0, yPos+amplitude*-1),Vec2(xPos+t/4.0,yPos+amplitude*-1));
	beat[2] = Edge2(xPos+t/2.0, yPos+amplitude*-1, xPos+t/4.0*3.0, yPos+amplitude*0.33333);
	beatCol[2] = Tri2(Vec2(xPos+t/2.0, yPos+amplitude*-1),Vec2(xPos+t/4.0*3.0, yPos+amplitude*0.33333),Vec2(xPos+t/4.0*3.0,yPos+amplitude*-1));
	beat[3] = Edge2(xPos+t/4.0*3.0, yPos+amplitude*0.33333, xPos+t/4.0*3.5, yPos+amplitude*-0.33333);
	beatCol[3] = Tri2(Vec2(xPos+t/4.0*3.0, yPos+amplitude*0.33333),Vec2(xPos+t/4.0*3.5, yPos+amplitude*-0.33333), Vec2(xPos+t/4.0*3.0,yPos+amplitude*-0.33333));
	beat[4] = Edge2(xPos+t/4.0*3.5, yPos+amplitude*-0.33333, xPos+t, yPos);
	beatCol[4] = Tri2(Vec2(xPos+t/4.0*3.5, yPos+amplitude*-0.33333),Vec2(xPos+t, yPos), Vec2(xPos+t,yPos+amplitude*-0.33333));


}

void Wave::updateWave()
{
	beat[0] = Edge2(xPos, yPos, xPos+t/4.0, yPos+amplitude);
	beatCol[0] = Tri2(Vec2(xPos,yPos),Vec2(xPos+t/4.0, yPos+amplitude), Vec2(xPos+t/4.0, yPos));
	beat[1] = Edge2(xPos+t/4.0, yPos+amplitude, xPos+t/2.0, yPos+amplitude*-1);
	beatCol[1] = Tri2(Vec2(xPos+t/4.0, yPos+amplitude),Vec2(xPos+t/2.0, yPos+amplitude*-1),Vec2(xPos+t/4.0,yPos+amplitude*-1));
	beat[2] = Edge2(xPos+t/2.0, yPos+amplitude*-1, xPos+t/4.0*3.0, yPos+amplitude*0.33333);
	beatCol[2] = Tri2(Vec2(xPos+t/2.0, yPos+amplitude*-1),Vec2(xPos+t/4.0*3.0, yPos+amplitude*0.33333),Vec2(xPos+t/4.0*3.0,yPos+amplitude*-1));
	beat[3] = Edge2(xPos+t/4.0*3.0, yPos+amplitude*0.33333, xPos+t/4.0*3.5, yPos+amplitude*-0.33333);
	beatCol[3] = Tri2(Vec2(xPos+t/4.0*3.0, yPos+amplitude*0.33333),Vec2(xPos+t/4.0*3.5, yPos+amplitude*-0.33333), Vec2(xPos+t/4.0*3.0,yPos+amplitude*-0.33333));
	beat[4] = Edge2(xPos+t/4.0*3.5, yPos+amplitude*-0.33333, xPos+t, yPos);
	beatCol[4] = Tri2(Vec2(xPos+t/4.0*3.5, yPos+amplitude*-0.33333),Vec2(xPos+t, yPos), Vec2(xPos+t,yPos+amplitude*-0.33333));


}


void Wave::drawWave( )
{
	for( int i=0; i<5; i++ ) {
		this->beat[i].draw(RED, 0.015f);
		//this->beatCol[i].draw(GREEN);
	}
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

void Pulse::updatePulse(dropArray & dArray, int stage) {

	waveys[0].xPos -= pSpeed;

	if(waveys[0].xPos + Tt < -RES_X / RES_Y) {
		for(int i=0;i<waveCount-1;i++){
			waveys[i].copyWave(waveys[i+1]);
		}
		for(int i=0;i<waveCount;i++){
			waveys[i].xPos -= pSpeed;
		}
		dArray.newDrop = true;
		waveys[waveCount-1].initWave(-RES_X / RES_Y + Tt * (waveCount - 1), Tt, calcAmp(stage));
	} else {
		for(int i=1;i<waveCount;i++){
			waveys[i].xPos -= pSpeed;
		}
	}
	for(int i=0;i<waveCount;i++){
		waveys[i].updateWave();
	}
}

void Pulse::drawPulse() {
	for(int i=0;i<waveCount;i++){
		waveys[i].drawWave();
		//D.text(waveys[i].xPos, 0.5f, S+i);
	}
}

void Pulse::initPulse() {
	flatNext = false;
	waveCount = RES_X / RES_Y * 2 / Tt + 3;
	pSpeed = PULSE_SPEED;
	waveType = false;

	for(int i=0;i<waveCount;i++){
		waveys[i].initWave(-RES_X / RES_Y + Tt * i, Tt, 0.0f);
	}

}

Flt Pulse::calcAmp(int stage) {
	waveType = !waveType;
	if(waveType)
		return 0.15f * sqrt((double) stage);
	else
		return 0.0f;
		
}