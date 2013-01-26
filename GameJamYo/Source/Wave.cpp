#include "stdafx.h"
#include "main.hpp"
#include "Wave.h"
#include "mathFunctions.h"

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
		this->beat[i].draw(RED);
		this->beatCol[i].draw(GREEN);
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

void Pulse::updatePulse() {

	waveys[0].xPos -= pSpeed;

	if(waveys[0].xPos + 0.2f < -RES_X / RES_Y) {
		for(int i=0;i<19;i++){
			waveys[i].copyWave(waveys[i+1]);
		}
		for(int i=0;i<20;i++){
			waveys[i].xPos -= pSpeed;
		}
		waveys[19].initWave(-RES_X / RES_Y + 0.2f * 19, 0.2f, calcAmp());
	} else {
		for(int i=1;i<20;i++){
			waveys[i].xPos -= pSpeed;
		}
	}
	for(int i=0;i<20;i++){
		waveys[i].updateWave();
	}
}

void Pulse::drawPulse() {
	for(int i=0;i<20;i++){
		waveys[i].drawWave();
		D.text(waveys[i].xPos, 0.5f, S+i);
	}
}

void Pulse::initPulse() {
	flatNext = false;
	pSpeed = .005f;

	for(int i=0;i<20;i++){
		waveys[i].initWave(-RES_X / RES_Y + 0.2f * i, 0.2f, 0.0f);
	}

}

Flt Pulse::calcAmp() {
	return 0.3f;
}