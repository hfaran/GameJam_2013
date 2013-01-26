#include "stdafx.h"
#include "Wave.h"
#include "dropArray.hpp"


#ifndef NPC_HPP
#define NPC_HPP

struct NPC {
	Flt x;
	Flt y;
	int pixelX;
	int pixelY;

	Image *currentFrame;
	int numFrames;
	Flt frameCounter;
	Image *frame;
	Flt animSpeed;
	Rect drawBox;

	int numStages;
	int Stage;
	int nextFrame;

	void initNPC( int pX, int pY, int nFrames, Flt animSpd, int stage );

	void updateNPC(Pulse & pPulse, dropArray & dArray);
	void drawNPC( );
};

#endif