#include "stdafx.h"

#ifndef WAVE_H
#define WAVE_H

struct Wave {

	Edge2 beat[5]; // array of edges that form a pulse
	Flt t; //period of the pulse part (beat) of the wave
	Flt amplitude; //max height of the pulse

	Flt yPos; //
	Flt xPos; //

	void initWave( Flt xPosition, Flt yPosition, Flt period=0.2, Flt Amp=0.4 );
	void updateWave(Flt period, Flt Amp );
	void copyWave(Wave & waveA);
	void drawWave();
};


struct Pulse {

	Flt pSpeed; //Speed of the pulse in -x direction
	Wave waveys[20];
	bool flatNext;
	
	void updatePulse();
	void drawPulse();
	void initPulse();
};



#endif