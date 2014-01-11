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
	Flt transitionFrameCounter;
	Image *frame;
	Flt animSpeed;
	Rect drawBox;

	int numStages;
	int Stage;
	int nextFrame;

	Sound beat;

	void initNPC( int pX, int pY, int nFrames, Flt animSpd, int stage );

	void updateNPC(Pulse & pPulse, dropArray & dArray, int stage, bool & theEnd);
	void drawNPC( );
};

#endif