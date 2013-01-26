#include "stdafx.h"


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

	void initNPC( int pX, int pY, int nFrames, Flt animSpd );

	void updateNPC( );
	void drawNPC( );
};

#endif