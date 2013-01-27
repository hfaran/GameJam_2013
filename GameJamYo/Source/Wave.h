#include "stdafx.h"
#include "dropArray.hpp"

#ifndef WAVE_H
#define WAVE_H

struct Wave {

	Edge2 beat[5]; // array of edges that form a pulse
	Tri2 beatCol[5]; // array of edges that form a pulse
	Flt t; //period of the pulse part (beat) of the wave
	Flt amplitude; //max height of the pulse

	Flt yPos; //
	Flt xPos; //

	void initWave( Flt xPosition, Flt period=0.2, Flt Amp=0.4 );
	void updateWave();
	void copyWave(Wave & waveA);
	void drawWave();

	
};


struct Pulse {

	Flt pSpeed; //Speed of the pulse in -x direction
	Wave waveys[20];
	bool flatNext;
	int waveCount;
	
	void updatePulse(dropArray & dArray, int stage);
	void drawPulse();
	void initPulse();
	Flt calcAmp(int stage);

	bool waveType;
};



#endif